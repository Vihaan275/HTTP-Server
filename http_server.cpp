#include <asm-generic/socket.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h> 
#include <cstring> 
#include <iostream> 
#include <chrono>
#include "threadpool.hpp"
#include <thread>
#include <string>
#include <bits/stdc++.h>
#include <fstream>
#include <sys/sendfile.h>
#include <fcntl.h>
std::string file_path = "./index.html";
int file_fd = open("./index.html",O_RDONLY);
std::ifstream file(file_path);

std::string html = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
int file_size = html.size();
std::string file_siz = std::to_string(file_size);

    std:: string message_to_send = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: "+file_siz+"\r\n"
        "Connection: close\r\n"
        "\r\n";
void server_work(int client_fd){

    char message_recieved[4096];
    std::string request;

    while (true){
    int bytes_received =
    recv(client_fd,
         message_recieved,
         sizeof(message_recieved),
         0);

    if (bytes_received < 0)
{
    std::cout << "Error in getting a message" << std::endl;
    break;
}

request.append(message_recieved, bytes_received);

    if (request.find("\r\n\r\n") != std::string::npos){
        break;
    }
    }
    //std::cout<<request<<std::endl;

    
//sending content header first

//    std::cout<<(message_to_send)<<std::endl;
//    std::cout<<html.size()<<std::endl;

    //if (request.substr(0,3)=="GET"){
        if (send(client_fd,message_to_send.c_str(),(message_to_send).size(),0)==-1){
            std::cout<<"Error in sending packets"<<std::endl;
    
        }

    off_t zero = 0;
//sending html file to server
    if (sendfile(client_fd,file_fd,&zero,file_size)==-1){
            std::cout<<"Error with sending file"<<std::endl;
    }
    shutdown(client_fd,SHUT_WR);
    close(client_fd);
    }

int main(){ 

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd==-1){
        std::cout<<"Error"<<std::endl;
        return 1;
    }
    //makes the file descriptor that can be used for the connection between two sockets.


  //line to make sure that the same port gets reused when the server refreshes. 
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  //basically writing out the full address needed for connecting to the ports
  sockaddr_in addr{};//make an address object 
  addr.sin_family = AF_INET;//make sure it is an ipv4 address
  
  addr.sin_port = htons(8080);//give it port 8080, htons does conversion from bigendian to littlendian to make sure there are no problems with sending data
  inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
    
  //binds a socket to the specified address, and cast address into sockaddress because that is the type of struct that the bind function requires
 if ( bind(server_fd,(sockaddr*)&addr,sizeof(addr)) == -1){
    std::cout<<"Server binding failed"<<std::endl;
 }
  
  //makes the server open to connect to requests, can have at most 10 requests in queue
  if (listen(server_fd,SOMAXCONN) == -1){
    std::cout<<"listener failed"<<std::endl;
  }
  
//DETERMINE THREAD COUNT, MASSIVE PERFORMANCE AFFECTOR
  ThreadPool threadpool(16);
  int task=0;
  while (true){

      sockaddr_in client_addr{};
      socklen_t client_len = sizeof(client_addr);



  //accept blocks the server until a client connection is made
  //once made, the connection is handed off to a different socket, which is the client_fd
  int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
  if (client_fd==-1){
      std::cout<<"Couldnt accept connection"<<std::endl;
  }


threadpool.add_task([client_fd]{server_work(client_fd);});
task++;
    }
  threadpool.done_with_tasks();
  }

