#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

void reply(Connection conn) {
  Message msg;
  conn.receive(msg);
  if (msg.tag == TAG_ERR) {
    std::cerr << msg.data;
    exit(1);
  }
  if (msg.tag != TAG_OK) {
    std::cerr << "Error: Unexpected message from the server. No OK received.\n";
    exit(1);
  }
}

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

  // TODO: send slogin message
  conn.send(Message(TAG_SLOGIN, username));
  reply(conn); 

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
      reply(conn); 
    } else if (command == "/leave") {
      conn.send(Message(TAG_LEAVE, ""));
      reply(conn);
    } else if (command == "/quit") {
      conn.send(Message(TAG_QUIT, ""));
      reply(conn);
      break;  
    } else {
      if (command[0] == '/') {
        std::cerr << "Error: Invalid command\n";
        continue;
      }
      std::string message;
      std::getline(iss, message);
      conn.send(Message(TAG_SENDALL, message));
      reply(conn);
    } 
  }
  return 0;
}
