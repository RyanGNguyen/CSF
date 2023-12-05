#include <pthread.h>
#include <iostream>
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

  ClientInfo(Connection * conn, Server * server)
    : conn(conn), server(server) { }
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

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
    if (conn->receive(login)) {
      if (login.tag == TAG_SLOGIN || login.tag == TAG_RLOGIN) {
        if (!is_valid(login.data)) {
          conn->send(new Message(TAG_ERR, "Invalid username!"));
          return nullptr; 
        } 
        conn->send(new Message(TAG_OK, ""));
      } else {
        conn->send(new Message(TAG_ERR, "Wrong tag for login message!")); 
        return nullptr; 
      }
    } else {
      conn->send(new Message(TAG_ERR, "Failed to receive the login message!"));
      return nullptr; 
    }

    // TODO: depending on whether the client logged in as a sender or
    //       receiver, communicate with the client (implementing
    //       separate helper functions for each of these possibilities
    //       is a good idea)
    User * user = new User(login.data);
    if (login.tag == TAG_SLOGIN) {
      chat_with_sender(conn, info->server, user);
    } else {
      chat_with_receiver(conn, info->server, user);
    }
    return nullptr;
  }
  
  bool is_valid(std::string &str) {
    trim(str);
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

  // trim from start (in place)
  static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
  }

  // trim from end (in place)
  static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
  }

  // trim from both ends (in place)
  static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
  }

  void chat_with_sender(Connection * conn, Server * server, User * user) {
    while (true) {
      Message msg; 
      if (!conn->receive(msg)) {
        conn->send(new Message(TAG_ERR, "Failed to receive the message!"));
        break; 
      } 

      if (msg.tag == TAG_JOIN) {
        if (is_valid(msg.data)) {
          Room * room = server->find_or_create_room(msg.data);
          room->add_member(user);
          user->room_name = msg.data;
          conn->send(new Message(TAG_OK, ""));
        } else {
          conn->send(new Message(TAG_ERR, "Invalid room name!"));
        }
      } else if (msg.tag == TAG_SENDALL) {
        if (user->room_name != "") {
          Room * room = server->find_or_create_room(user->room_name);
          room->broadcast_message(user->username, msg.data);
          conn->send(new Message(TAG_OK, "")); 
        } else {
          conn->send(new Message(TAG_ERR, "You are not in a room!"));
        }
      } else if (msg.tag == TAG_LEAVE) {
        if (user->room_name != "") {
          Room * room = server->find_or_create_room(user->room_name);
          room->remove_member(user);
          user->room_name = ""; 
          conn->send(new Message(TAG_OK, "")); 
        } else {
          conn->send(new Message(TAG_ERR, "You are not in a room!"));
        }
        conn->send(new Message(TAG_OK, ""));
      } else if (msg.tag == TAG_QUIT) {
        conn->send(new Message(TAG_OK, ""));
        break; 
      } else {
        conn->send(new Message(TAG_ERR, "Invalid tag!"));
      }
    }
    conn->close();
    delete(user);
  }

  void chat_with_receiver(Connection * conn, Server * server, User * user) {
    Message msg;
    if (!conn->receive(msg)) {
      conn->send(new Message(TAG_ERR, "Failed to receive the message!"));
    } 

    if (msg.tag == TAG_JOIN) {
      if (!is_valid(msg.data)) {
        conn->send(new Message(TAG_ERR, "Invalid room name!"));
        return; 
      } 
      if (user->room_name != "") {
        Room * old = server->find_or_create_room(user->room_name);
        old->remove_member(user);
      } 
      Room * room = server->find_or_create_room(msg.data);
      room->add_member(user);
      user->room_name = msg.data;
      conn->send(new Message(TAG_OK, ""));
    } else {
      conn->send(new Message(TAG_ERR, "Invalid tag!"));
    }

    while (true) {
      Message * msg = user->mqueue.dequeue();
      if (msg != nullptr) {
        conn->send(msg);
        delete(msg);
      }
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
  pthread_mutex_init(&m_lock, nullptr); // Do not need to add pthread_mutexattr_t due to worker detaching for us
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  m_ssock = open_listenfd(std::to_string(m_port).c_str());
  return m_ssock != -1;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (true) {
    int csock = Accept(m_ssock, nullptr, nullptr);
    if (csock == -1) {
      std::cerr << "Error: Could not accept the client connection!\n";
      continue;               // may need to remove this
    }
    Connection * conn = new Connection(csock);
    struct ClientInfo * info = new ClientInfo(conn, this);
    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, info) != 0) {
      std::cerr << "Error: pthread_create failed!\n";
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard g(m_lock);
  if (m_rooms.find(room_name) == m_rooms.end()) {
    Room *room = new Room(room_name);
    m_rooms.insert({room_name, room});
  } 
  return m_rooms.at(room_name);
}
