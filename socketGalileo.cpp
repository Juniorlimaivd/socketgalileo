#include "socketGalileo.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

Sockt::Sockt(int domain, int type, int protocol)
{
  this->socktFileDescriptor = socket(domain, type, protocol);
}

Sockt::Sockt(int filedescriptor)
{
  this->socktFileDescriptor = filedescriptor;
}

Sockt::Sockt()
{

}

int Sockt::connect(std::string ip, port gate)
{
  this->socktAddr.sin_family = AF_INET;
  this->socktAddr.sin_port = htons(gate);

  char* addr = new char[ip.size() + 1];
  std::copy(ip.begin(),ip.end(),addr);
  addr[ip.size()] = '\0';

  inet_pton(AF_INET,addr,&this->socktAddr.sin_addr.s_addr);
  delete[] addr;

  int result;
  result = ::connect(this->socktFileDescriptor, (struct sockaddr*) &this->socktAddr, sizeof(this->socktAddr));

  return result;
}

int Sockt::bind(std::string ip, port gate)
{
  if(ip.compare("localhost"))
  {
    char* addr = new char[ip.size() + 1];
    std::copy(ip.begin(),ip.end(),addr);
    addr[ip.size()] = '\0';

    inet_pton(AF_INET,addr,&this->socktAddr.sin_addr.s_addr);
    delete[] addr;
  }
  else
  {
    this->socktAddr.sin_addr.s_addr = INADDR_ANY;
  }

  int result;
  this->socktAddr.sin_family = AF_INET;
  this->socktAddr.sin_port = htons(gate);
  result = ::bind(this->socktFileDescriptor, (struct sockaddr *) &this->socktAddr, sizeof(this->socktAddr));
  if(result!=0) printf("error binding\n");
  return result;
}

int Sockt::send(std::string data)
{
  char buffer[BUFF_SIZE] = {0};

  std::copy(data.begin(), data.end(), buffer);

  buffer[data.size()] = '\0';

  ssize_t total_bytes_written = 0;
  ssize_t bytes_written;
  while (total_bytes_written != data.size())
  {
      assert(total_bytes_written < 1024);
      bytes_written = write(this->socktFileDescriptor,
                                    &buffer[total_bytes_written],
                                    data.size() - total_bytes_written);
      if (bytes_written == -1)
      {
          printf("Failed to send all bytes. %ld bytes sended.",bytes_written);
          break;
      }
      total_bytes_written += bytes_written;
  }

  if(bytes_written==-1)
  {
      return -1;
  }
  else
  {
      return 0;
  }
}

std::string Sockt::recv(int size)
{
  char buffer[BUFF_SIZE] = {0};
  if(this->socktFileDescriptor>0)
  {
    int res = ::read(this->socktFileDescriptor,buffer, sizeof(buffer));
    if(res<=0)
    {

      return "";
    }
    else
    {
      buffer[size]='\0';
      return std::string(buffer);
    }
  }
  else
  {
    printf("error: bad file descriptor\n");
    return "";
  }


}

void Sockt::listen(int conections)
{
  ::listen(this->socktFileDescriptor, conections);
}

void Sockt::accept(Sockt* clientSockt)
{


  unsigned int supplied_len;
  unsigned int *ip_suppliedlen_op_storedlen;
  supplied_len = sizeof(clientSockt->socktAddr);
  ip_suppliedlen_op_storedlen = &supplied_len;

  clientSockt->socktFileDescriptor = ::accept(this->socktFileDescriptor, (struct sockaddr *) &(clientSockt->socktAddr), ip_suppliedlen_op_storedlen);


  if(clientSockt->socktFileDescriptor<0)
  {
    printf("error connecting with client.\n");
  }

}

Sockt::~Sockt()
{
  close(this->socktFileDescriptor);
  delete this;
}
