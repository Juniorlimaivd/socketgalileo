#include "socketGalileo.cpp"
#include <iostream>

int main()
{

  Sockt* client = new Sockt(AF_INET,SOCK_STREAM,TCP);

  client->connect("localhost",1235);

  client->send("conected");

  std::string teste = "";

  while(teste.compare("en"))
  {
    std::cin >> teste;

    client->send(teste);
  }

  delete client;
  return 0;
}
