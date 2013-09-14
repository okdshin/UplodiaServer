#pragma once
//Server:20130913
#include <iostream>
#include <vector>
#include <cstdlib>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "RequestHeaderParser.h"

namespace uploadia
{
class Server{
public:
    Server(uint16_t port) 
			: socket_(socket(AF_INET, SOCK_STREAM, 0)){
		bzero(&address_, sizeof(address_));
		if(socket_ < 0){
			std::cout << "socket error" << std::endl;
			exit(EXIT_FAILURE);
		}
		address_.sin_family = AF_INET;
		address_.sin_port = htons(port);
		address_.sin_addr.s_addr = INADDR_ANY;
		int yes = 1;
		setsockopt(socket_, 
			SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
		int is_bind_failed = 
			bind(socket_, (struct sockaddr*)(&address_), sizeof(address_));
		if(is_bind_failed){
			std::cout << "bind error" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
    ~Server(){}

	void Run() {
		int is_listen_failed = listen(socket_, 5);
		if(is_listen_failed){
			std::cout << "listen error" << std::endl;	
			exit(EXIT_FAILURE);
		}
		std::string response(
			"HTTP/1.0 200 OK\r\n"
			"Content-Length: 5\r\n"
			"Content-Type: text/html\r\n""\r\n"
			"HELLO\r\n");
		std::vector<uint8_t> response_vec(response.begin(), response.end());
		while(true){
			struct sockaddr_in client;
			int len = sizeof(client);
			int client_socket = accept(socket_, 
				(struct sockaddr *)&client, (socklen_t*)&len);
			pid_t pid = fork();
			if(pid == -1){
				std::cout << "fork error" << std::endl;	
				exit(EXIT_FAILURE);
			}
			else if(pid == 0){
				close(socket_);
				std::vector<uint8_t> in_buffer(4096, 0);
				int in_size = 
					recv(client_socket, &in_buffer.front(), in_buffer.size(), 0);
				if(in_size < 0){
					exit(EXIT_FAILURE);	
				}
				std::string in_buffer_str(
					in_buffer.begin(), in_buffer.begin()+in_size);
				std::cout << in_buffer_str << std::endl;
				in_buffer.resize(in_size);
				RequestHeader request = request_parser_.Parse(in_buffer);
				std::cout << request << std::endl;
				send(client_socket, &response_vec.front(), response_vec.size(), 0);
				close(client_socket);
				exit(EXIT_SUCCESS);	
			}
		}
	}

private:
	int socket_;
	struct sockaddr_in address_;
	RequestHeaderParser request_parser_;
};
}

