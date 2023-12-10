#include <pthread.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
struct ClientInfo {

  Connection * conn;
  Server * server;

  ClientInfo(Connection * conn, Server * server): conn(conn), server(server) { }

  ~ClientInfo() { delete conn; }
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

bool is_valid(const std::string &str); 
std::string ltrim(const std::string &s); 
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);
void chat_with_sender(Connection * conn, Server * server, User * user); 
void chat_with_receiver(Connection * conn, Server * server, User * user); 

namespace {
  void *worker(void *arg) {
    pthread_detach(pthread_self());

    // TODO: use a static cast to convert arg from a void* to
    //       whatever pointer type describes the object(s) needed
    //       to communicate with a client (sender or receiver)
    std::unique_ptr<ClientInfo> info(static_cast<ClientInfo *>(arg));

    // TODO: read login message (should be tagged either with
    //       TAG_SLOGIN or TAG_RLOGIN), send response
    Connection * conn = info->conn;
    Message login; 
    std::string username; 
    if (conn->receive(login)) {
      if (login.tag == TAG_SLOGIN || login.tag == TAG_RLOGIN) {
        username = trim(login.data);
        if (!is_valid(username)) {
          Message username_err(TAG_ERR, "Invalid username!");
          conn->send(username_err);
          return nullptr; 
        } else {
          Message ok(TAG_OK, "");
          conn->send(ok);
        }
      } else {
        Message login_err(TAG_ERR,"Wrong tag for login message!"); 
        conn->send(login_err); 
        return nullptr; 
      }
    } else {
      Message not_received(TAG_ERR, "Failed to receive the login message!");
      conn->send(not_received);
      return nullptr; 
    }

    // TODO: depending on whether the client logged in as a sender or
    //       receiver, communicate with the client (implementing
    //       separate helper functions for each of these possibilities
    //       is a good idea)
    User * user = new User(username);
    if (login.tag == TAG_SLOGIN) {
      chat_with_sender(conn, info->server, user);
    } else {
      chat_with_receiver(conn, info->server, user);
    }
    delete user;
    return nullptr;
  }
}

bool is_valid(const std::string &str) {
  if (str.empty() || str.length() > 255) {
    return false;
  }
  for (char c : str) {
    if (!isalnum(c)) {
      return false;
    }
  }
  return true;
}

// trim from start
std::string ltrim(const std::string &s) {
  std::string str = s;
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
      return !std::isspace(ch);
  }));
  return str; 
}

// trim from end (in place)
std::string rtrim(const std::string &s) {
  std::string str = s;
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
      return !std::isspace(ch);
  }).base(), str.end());
  return str;
}

// trim from both ends (in place)
std::string trim(const std::string &s) {
  return rtrim(ltrim(s)); 
}

void chat_with_sender(Connection * conn, Server * server, User * user) {
  while (true) {
    Room * room = nullptr;
    Message msg;
    if (!conn->receive(msg)) {
      Message not_received(TAG_ERR, "Failed to receive the message!");
      conn->send(not_received);
      break; 
    } 

    if (msg.tag == TAG_JOIN) {
      std::string room_name = trim(msg.data); 
      if (is_valid(room_name)) {
        // user->room_name = room_name;
        room = server->find_or_create_room(user->room_name);
        Message ok(TAG_OK, "");
        conn->send(ok);
      } else {
        Message room_name_err(TAG_ERR, "Invalid room name!");
        conn->send(room_name_err);
      }
    } else if (msg.tag == TAG_SENDALL) {
      //if (user->room_name.length() > 0) {
      if (room != nullptr) {
        //Room * room = server->find_or_create_room(user->room_name);
        room->broadcast_message(user->username, msg.data);
        Message ok(TAG_OK, "");
        conn->send(ok); 
      } else {
        Message not_in_room(TAG_ERR, "You are not in a room!");
        conn->send(not_in_room);
      }
    } else if (msg.tag == TAG_LEAVE) {
      // if (user->room_name.length() > 0) {
      if (room != nullptr) {
        // user->room_name = ""; 
        room = nullptr;
        Message ok(TAG_OK, "");
        conn->send(ok); 
      } else {
        Message not_in_room(TAG_ERR, "You are not in a room!");
        conn->send(not_in_room);
      }
    } else if (msg.tag == TAG_QUIT) {
      Message ok(TAG_OK, "");
      conn->send(ok); 
      break; 
    } else {
      Message invalid_tag(TAG_ERR, "Invalid tag!");
      conn->send(invalid_tag);
    }
  }
}

void chat_with_receiver(Connection * conn, Server * server, User * user) {
  Message msg;
  if (!conn->receive(msg)) {
    Message not_received(TAG_ERR, "Failed to receive the message!");
    conn->send(not_received);
    return;
  } 

  Room * room = nullptr;
  if (msg.tag == TAG_JOIN) {
    std::string room_name = trim(msg.data);
    if (!is_valid(room_name)) {
      Message room_name_err(TAG_ERR, "Invalid room name!");
      conn->send(room_name_err);
      return; 
    } 
    room = server->find_or_create_room(room_name);
    //user->room_name = room_name;
    room->add_member(user);
    Message ok(TAG_OK, "");
    conn->send(ok); 
  } else {
    Message invalid_tag(TAG_ERR, "Invalid tag!");
    conn->send(invalid_tag);
    return; 
  }

  while (true) {
    Message * reply = user->mqueue.dequeue();
    if (reply != nullptr) {
      if (!conn->send(*reply)) {
        delete reply;
        room->remove_member(user);
        return;
      }
      delete reply;
    }
  }
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL); // Do not need to add pthread_mutexattr_t due to worker detaching for us
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  m_ssock = open_listenfd(std::to_string(m_port).c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (true) {
    int csock = Accept(m_ssock, NULL, NULL);
    if (csock < 0) {
      std::cerr << "Error: Could not accept the client connection!\n";             
    }
    Connection * conn = new Connection(csock);
    struct ClientInfo * info = new ClientInfo(conn, this);
    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, info) < 0) {
      std::cerr << "Error: pthread_create failed!\n";
      delete info;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard g(m_lock);
  if (m_rooms.find(room_name) == m_rooms.end()) {
    m_rooms.insert({room_name, new Room(room_name)});
  } 
  Room * room = m_rooms.at(room_name); 
  return room;
}
