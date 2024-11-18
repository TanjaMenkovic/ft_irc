#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>

typedef struct s_input
{
    int port;
    std::string password;
}   t_input;

typedef struct s_sockaddr_in
{

} t_sockaddr_in;