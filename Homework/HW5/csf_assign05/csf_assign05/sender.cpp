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
  conn = Connection();
  conn.connect(server_hostname, server_port);

  // TODO: send slogin message
  conn.send(Message(TAG_SLOGIN, username)); 

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string command;
    iss >> command; 
    if (command == "/quit") {
      if (conn.send(Message(TAG_QUIT, ""))) {
        break;
      }
    } else if (command == "/join") {
      std::string room_name;
      iss >> room_name;
      conn.send(Message(TAG_JOIN, room_name));
    } else if (command == "/leave") {
      std::string room_name;
      iss >> room_name;
      conn.send(Message(TAG_LEAVE, room_name));
    } else {
      std::string message;
      std::getline(iss, message);
      conn.send(Message(TAG_SENDALL, message));
    } 
  }
  return 0;
}
