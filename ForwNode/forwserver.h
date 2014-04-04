#ifndef FORWSERVER_H
#define FORWSERVER_H

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;

namespace Dosee
{
namespace ForwNode
{


class ForwServer
{
public:
    ForwServer(int tcp_port);
    ForwServer();
    int start_tcp();
    static void onConnection(struct evconnlistener *listener,
                             evutil_socket_t fd, struct sockaddr *address, int socklen,
                             void *ctx);
    static void onAcceptError(struct evconnlistener *listener, void *ctx);
    static void onMessage(struct bufferevent *bev, void *ctx);
    static void onCloseConn(struct bufferevent *bev, short events, void *ctx);




private:
    int tcp_port_;
    int udp_port_;
};

}}
#endif // FORWSERVER_H
