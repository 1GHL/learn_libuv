#ifndef FORWSERVER_H
#define FORWSERVER_H


#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "uv.h"


using namespace std;

namespace Dosee
{
  namespace ForwNode
  {
#define MAX_TCP_CONNECTION 200

    static uv_loop_t* loop;

    uv_buf_t read_buffer(uv_handle_t* handle, size_t suggested_size);
    void tcp_on_connection(uv_stream_t* server, int status);
    void tcp_on_message(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);

    void udp_on_message(uv_udp_t* handle,
                        ssize_t nread,
                        uv_buf_t buf,
                        struct sockaddr* addr,
                        unsigned flags);

    void start(int tcp_port,int udp_port);
    void init_tcp(int tcp_port);
    void init_udp(int udp_port);

    //    class ForwServer
    //    {
    //    public:
    //      ForwServer(int tcp_port);
    //      ForwServer();
    //      int start_tcp();
    //      static void onConnection(struct evconnlistener *listener,
    //                               evutil_socket_t fd, struct sockaddr *address, int socklen,
    //                               void *ctx);
    //      static void onAcceptError(struct evconnlistener *listener, void *ctx);
    //      static void onMessage(struct bufferevent *bev, void *ctx);
    //      static void onCloseConn(struct bufferevent *bev, short events, void *ctx);




    //    private:
    //      int tcp_port_;
    //      int udp_port_;
    //    };

  }}
#endif // FORWSERVER_H
