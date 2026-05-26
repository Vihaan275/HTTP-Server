#include <asm-generic/socket.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <cstring> 
#include <iostream> 

int main(){ 

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    //makes the file descriptor that can be used for the connection between two sockets.


  //line to make sure that the same port gets reused when the server refreshes. 
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  //basically writing out the full address needed for connecting to the ports
  sockaddr_in addr{};//make an address object 
  addr.sin_family = AF_INET;//make sure it is an ipv4 address
  addr.sin_addr.s_addr = INADDR_ANY;//give it any ip that is avliable
  addr.sin_port = htons(8080);//give it port 8080, htons does conversion from bigendian to littlendian to make sure there are no problems with sending data
    
  //binds a socket to the specified address, and cast address into sockaddress because that is the type of struct that the bind function requires
  bind(server_fd,(sockaddr*)&addr,sizeof(addr));
  
  //makes the server open to connect to requests, can have at most 10 requests in queue
  listen(server_fd,10);
  
  while (true){

      sockaddr_in client_addr{};
      socklen_t client_len = sizeof(client_addr);



  //accept blocks the server until a client connection is made
  //once made, the connection is handed off to a different socket, which is the client_fd
  int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
  std::cout<<"Connection established"<<std::endl;
  close(client_fd);
  }
}

