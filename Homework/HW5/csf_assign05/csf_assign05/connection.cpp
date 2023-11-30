#include <sstream>
#include <cctype>
#include <cassert>
#include <iostream>
#include <unistd.h>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd); 
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  int fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  if (fd < 0) {
    std::cerr << "Error: could not connect to server\n";
    exit(EOF_OR_ERROR); 
  }
  m_fd = fd;
  m_last_result = SUCCESS;

  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (is_open()) {
    close();
  }
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  Close(m_fd);
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string buffer = msg.tag + ":" + msg.data + "\n";
  ssize_t n = (m_fd, buffer.c_str(), buffer.length()); 
  if (n == buffer.length()) {
    m_last_result = SUCCESS;
    return true;
  } else {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  char buf[Message::MAX_LEN];
  ssize_t n = rio_readlineb(&m_fdbuf, buf, Message::MAX_LEN);
  
  if (n == Message::MAX_LEN) {
    char* tag;
    char* data;
    tag = strtok_r(buf, ":", &data);
    if (tag == NULL || data == NULL) { 
      m_last_result = INVALID_MSG;
      return false;
    } else {
      msg.tag = tag;
      msg.data = data;
      m_last_result = SUCCESS;
      return true;
    }
  } else {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
}