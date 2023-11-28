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
  rio_readinitb(&m_fdbuf, m_fd); 
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
  if (Connection::is_open()) {
    Connection::close();
  }
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd > 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  Close(m_fd);
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  rio_writen(m_fd, msg.tag.c_str(), msg.tag.length()); 
  rio_writen(m_fd, ':', 1);
  rio_writen(m_fd, msg.data.c_str(), msg.data.length());
  rio_writen(m_fd, '\0', 1);
  
  // Unsure
  Message reply;
  if (Connection::receive(reply)) { 
    if (reply.tag == TAG_ERR) {
      m_last_result = EOF_OR_ERROR;
      std::cerr << reply.data << "\n";
      if (msg.tag == TAG_SLOGIN or msg.tag == TAG_RLOGIN) {
        exit(EOF_OR_ERROR);
      }
      return false;
    } else {
      m_last_result = SUCCESS;
      return true;
    }
  } else {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  char buf[Message::MAX_LEN] = {'\0'};
  ssize_t n = rio_readlineb(&m_fdbuf, buf, Message::MAX_LEN);
  
  if (n > 0) {
    char* tag;
    char* data = buf;
    tag = strtok_r(data, ":", &data);
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
