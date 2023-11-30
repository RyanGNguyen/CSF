#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Error: Connection is not open.\n";
    exit(1);
  }

  // TODO: send slogin message
  conn.send(Message(TAG_SLOGIN, username));
  Message slogin_ok;
  conn.receive(slogin_ok);
  if (slogin_ok.tag == TAG_ERR) {
    std::cerr << slogin_ok.data;
    exit(1);
  } 
  if (slogin_ok.tag != TAG_OK) {
    std::cerr << "Error: Unexpected message from the server. No OK received.\n";
    exit(1);
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string command;
    iss >> command; 
    if (command == "/join") {
      std::string room_name;
      iss >> room_name;
      conn.send(Message(TAG_JOIN, room_name));

      Message join_ok;
      if (conn.receive(join_ok)) {
        if (join_ok.tag == TAG_ERR) {
          std::cerr << join_ok.data;
        }
        if (slogin_ok.tag != TAG_OK) {
          std::cerr << "Error: Unexpected message from the server. No OK received.\n";
          exit(1);
        }
      }
    } else if (command == "/leave") {
      conn.send(Message(TAG_LEAVE, ""));
      Message join_ok;
      if (conn.receive(join_ok)) {
        if (join_ok.tag == TAG_ERR) {
          std::cerr << join_ok.data;
        }
        if (slogin_ok.tag != TAG_OK) {
          std::cerr << "Error: Unexpected message from the server. No OK received.\n";
          exit(1);
        }
      }
    } else if (command == "/quit") {
      if (conn.send(Message(TAG_QUIT, ""))) {
        break;
      }
    } else {
      std::string message;
      std::getline(iss, message);
      conn.send(Message(TAG_SENDALL, message));
    } 
  }
  return 0;
}
