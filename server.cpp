
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "TCPServer.h"
#include <sstream>
#include <time.h>
#include "math.h"

TCPServer::TCPServer()
{
	
}

TCPServer::~TCPServer()
{

}

void TCPServer::setup(int port)
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	// 设置套接字选项避免地址使用错误
	int on = 1;
	if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}
	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(port);
	bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
 	listen(sockfd,5);
}

//通信
string TCPServer::receive()
{
	int n = 1;
	while(1)
	{
		while (1)
		{
			socklen_t sosize = sizeof(clientAddress);
			newsockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &sosize);
			cout<<"连接．．．"<<endl;
			if(newsockfd != 0)break;
		}
		while(n != 0)
		{	
			char msg[MAXPACKETSIZE];
			memset(msg, 0, MAXPACKETSIZE);
			n = recv(newsockfd, msg, MAXPACKETSIZE, 0);
			if(n == 0)
			{
				cout<<"断开连接！！"<<endl;
				break;
			}
			string str = string(msg);
			if(str=="1"){
				cout<<"开始拍照－－－"<<endl;
				String_t serialNumber1 = "23284771";//输入相机的ｎａｍｅ
				Grab grab;
				int retstatus = grab.Carmera(1,serialNumber1);
				send(newsockfd, to_string(retstatus).c_str(), to_string(retstatus).length(), 0);
				cout<<"拍照完成－－－"<<endl;
			}else
			{
				cout<<"信号错误！"<<endl;
				break;
			}
		}
		close(newsockfd);
		n=1;
		newsockfd = 0;
	}
	return "endl";
}

void TCPServer::exit_socket()
{
	close(sockfd);
} 

vector<string> TCPServer::split(const string &s, const string &seperator){
  vector<string> result;
  typedef string::size_type string_size;
  string_size i = 0;
  
  while(i != s.size()){
    //找到字符串中首个不等于分隔符的字母；
    int flag = 0;
    while(i != s.size() && flag == 0){
      flag = 1;
      for(string_size x = 0; x < seperator.size(); ++x)
    	if(s[i] == seperator[x]){
      	++i;
      	flag = 0;
		break;
    	}
    }
    
    //找到又一个分隔符，将两个分隔符之间的字符串取出；
    flag = 0;
    string_size j = i;
    while(j != s.size() && flag == 0){
      for(string_size x = 0; x < seperator.size(); ++x)
    	if(s[j] == seperator[x]){
      	flag = 1;
     	 break;
    	}
      if(flag == 0) 
    	++j;
    }
    if(i != j){
      result.push_back(s.substr(i, j-i));
      i = j;
    }
  }
  return result;
} 