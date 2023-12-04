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

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, nullptr); // May add pthread_mutexattr_t for detached threads 
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  m_ssock = open_listenfd(m_port);
  return m_ssock != -1;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (true) {
    int csock = Accept(m_ssock, nullptr, nullptr);
    if (csock == -1) {
      Message msg(TAG_ERROR, "Could not accept connection"); // TODO: Send an error message back to client
      continue;
    }
    /*Create a thread for each accepted client connection using pthread_create(). 
    A struct should be created to pass the Connection object and other required data to the 
    client thread using the aux parameter of pthread_create(), and worker() should be used as 
    the entrypoint for the thread. It may also be a good idea to create a User object in each 
    client thread to track the pending messages, and register it to a Room when the client 
    sends a join request.
    You can test that your server handles more than one connection correctly by spawning
     multiple receivers and senders on the same server, and checking that the messages 
     sent from all senders get correctly delivered to all receivers.
    */
    pthread_t tid;
    pthread_create(&tid, nullptr, find_or_create_room, nullptr);
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  RoomMap::iterator it = m_rooms.find(room_name);
  if (it != m.end()) {
    return it->second;
  } else {
    Room *room = new Room(room_name);
    m_rooms[room_name] = room;
    return room;
  }
}
