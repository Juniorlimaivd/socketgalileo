#include "socketGalileo.hpp"


Sockt::Sockt(int domain, int type, int protocol)
{
  this->socktFileDescriptor = socket(domain, type, protocol);
  if(this->socktFileDescriptor < 0)
  {
    printf("Error creating socket.\n");
    exit(EXIT_FAILURE);
  }
}

Sockt::Sockt(int filedescriptor)
{
  this->socktFileDescriptor = filedescriptor;

  if(this->socktFileDescriptor < 0)
  {
    printf("Bad file descriptor passed.\n");
    exit(EXIT_FAILURE);
  }
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

  if(result<0)
  {
    printf("Error trying to connect with socket. Please check ip and port.\n");
    exit(EXIT_FAILURE);
  }else
  {
      this->gate = gate;
      this->ip = ip;
      return result;
  }

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
  if(result<0)
  {
    printf("Error try to bind with port. Please check port disponibility.\n");
    exit(EXIT_FAILURE);
  }

  this->ip = ip;
  this->gate = gate;
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
      if (bytes_written < 0)
      {
          printf("Failed to send all bytes. %ld bytes sended.",total_bytes_written);
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
      printf("Error reading buffer.\n");
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
    exit(EXIT_FAILURE);
  }


}

void Sockt::listen(int conections)
{
  int result = ::listen(this->socktFileDescriptor, conections);
  if(result<0)
  {
    printf("error trying to listen to port %hu.",this->gate);
  }
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
    printf("Error connecting with client.\n");
    exit(EXIT_FAILURE);
  }

}

Sockt::~Sockt()
{
  close(this->socktFileDescriptor);
  
}
