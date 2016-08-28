#include "socketGalileo.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

int main()
{
  Sockt* server = new Sockt(AF_INET,SOCK_STREAM,TCP);

  server->bind("localhost",1249);
  printf("server binded\n");
  server->listen(1);
  printf("server listening\n");
  Sockt* conn = new Sockt();

  server->accept(conn);

  printf("server accepted conection %d\n",conn->socktFileDescriptor);
  if(conn->socktFileDescriptor>=0)
  {
    printf("server accepted conection 2%d\n",conn->socktFileDescriptor);
    conn->send("e aqui?");

    printf("here\n");
    std::string text;
    printf("masoq\n");
    while(true)
    {
      text = conn->recv(256);

      std::cout << text << '\n';
    }

  }else
  {
    printf("error!\n");
  }

  //delete conn;
  //delete server;


  return 0;
}
