#ifndef _SOCKETGALILEO_H_
#define _SOCKETGALILEO_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>


#define BUFF_SIZE 256
#define TCP 0
#define UDP 1
typedef unsigned short port;

class Sockt
{

  public:
    Sockt(int domain, int type, int protocol);
    Sockt(int filedescriptor);
    Sockt();
    ~Sockt();
    // client.connect()
    int connect(std::string ip, port gate);
    //server.bind()
    int bind(std::string ip, port gate);
    //sock.send()
    int send(std::string data);
    //data = s.recv()
    std::string recv(int size);
    //server.listen()
    void listen(int conections);
    //conn = server.accept()
    void accept(Sockt* client);

    int socktFileDescriptor;
    struct sockaddr_in socktAddr;
  private:
    port gate;
    std::string ip;
};

#endif
