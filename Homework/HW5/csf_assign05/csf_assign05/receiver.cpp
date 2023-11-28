#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  conn = Connection();
  conn.connect(server_hostname, server_port);

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  conn.send(Message(TAG_RLOGIN, username));
  conn.send(Message(TAG_JOIN, room_name));
  Message reply;

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  while (true) {
    Message msg;
    if (conn.receive(msg)) {
      if (msg.tag == TAG_DELIVERY) {
        char *data = (char *) msg.data.c_str();
        strtok_r(data, ":", &data);   // Room
        std::string sender = strtok_r(data, ":", &data);
        std::string message = strtok_r(data, ":", &data);
        std::cout << sender << ": " << message << "\n";
      } 
    } 
  }
  return 0;
}
