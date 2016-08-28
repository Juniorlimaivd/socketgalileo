#include "socketGalileo.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

int main()
{
  Sockt* server = new Sockt(AF_INET,SOCK_STREAM,TCP);

  server->bind("localhost",1236);
  server->listen(1);

  Sockt* conn = new Sockt();
  server->accept(conn);

  printf("server accepted conection\n");
  if(conn->socktFileDescriptor>=0)
  {
    std::string text="";

    while(text.compare("ue"))
    {
      text = conn->recv(256);

      std::cout << text << '\n';
    }

  }else
  {
    printf("error!\n");
  }

  delete conn;
  delete server;

  return 0;
}
