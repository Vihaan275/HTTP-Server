#include <asm-generic/socket.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <cstring> 
#include <iostream> 

int main(){ 

    int client_fd = socket(AF_INET, SOCK_STREAM, 0); 

  sockaddr_in addr{};//make an address object 
  addr.sin_family = AF_INET;//make sure it is an ipv4 address
  addr.sin_addr.s_addr = INADDR_ANY;//give it any ip that is avliable


