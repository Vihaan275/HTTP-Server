#include <asm-generic/socket.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <cstring> 
#include <iostream> 
#include <arpa/inet.h>

int main(){ 

    int client_fd = socket(AF_INET, SOCK_STREAM, 0); 

  sockaddr_in server_addr;//make an address object 
  server_addr.sin_family = AF_INET;//make sure it is an ipv4 address
  server_addr.sin_port = htons(8080);
  inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);

  if (connect(client_fd,(sockaddr*)&server_addr,sizeof(server_addr))==-1){
      std::cout<<"Error connecting to server"<<std::endl;
      close(client_fd);
      return 1;
}
  std::cout<<"Connected to server"<<std::endl;

    char message[] = "HI! WHATS UP!!";
    send(client_fd,message,sizeof(message),0);

  close(client_fd);

  return 0;
  }
