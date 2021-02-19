#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "Grab.h"

using namespace std;

#define MAXPACKETSIZE 4096

class TCPServer
{
	public:
	  TCPServer();
	  ~TCPServer();
	  int sockfd, newsockfd, n,pid;
	  struct sockaddr_in serverAddress;
	  struct sockaddr_in clientAddress;
	  char msg[MAXPACKETSIZE];
	  void setup(int port);
	  string receive(); //1 标定 0计算
	  void exit_socket();
	  static vector<string> split(const string &s, const string &seperator);

	private:
		int sock ;
};

#endif
