#include "forwserver.h"
#include <string.h>

using namespace Dosee::ForwNode ;

namespace Dosee
{
namespace ForwNode
{
//void on_connection(uv_stream_t* server, int status)
void tcp_on_connection(uv_stream_t* server, int status)
{
    if (status == -1) {
        // error!
        return;
    }

    struct sockaddr  peername;
    int namelen,r;
    char req_ip[17];

    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);
    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        uv_read_start((uv_stream_t *) client, read_buffer, tcp_on_message);
    }
    else {
        uv_close((uv_handle_t *) client, NULL);
    }

    //namelen = sizeof sockname;
    // r = uv_tcp_getsockname(( uv_tcp_t *)server, &peername, &namelen);



    namelen = sizeof peername;
    r = uv_tcp_getpeername(( uv_tcp_t *)client, &peername, &namelen);
    //check_sockname(&peername, "127.0.0.1", 9999, "server socket");
    struct sockaddr_in *peer_addr = (struct sockaddr_in*) &peername;
    //  struct sockaddr_in *sin = (struct sockaddr_in *) sa;
    r = uv_ip4_name(peer_addr, (char*) req_ip, sizeof req_ip);
    cout<<peer_addr->sin_addr.s_addr<<" p: "<< req_ip <<" p: "<<ntohs(peer_addr->sin_port) <<endl;


}

uv_buf_t read_buffer(uv_handle_t* handle, size_t suggested_size)
{
    char buff[2048]={0,};
    return uv_buf_init(buff , 2048);
}



void tcp_on_message(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
{

    if(nread == 0)
    {
        return ;
    }
    else if(nread >0 && nread < buf.len)
    {
        if(buf.base[nread-1]=='\n' && buf.base[nread-2]=='\r')
        {
            //  uv_read_stop(stream);
        }
        printf("ok...\n");
        // uv_read_stop(stream);
        return ;
    }
}


void udp_on_message(uv_udp_t* handle,
                    ssize_t nread,
                    uv_buf_t buf,
                    struct sockaddr* addr,
                    unsigned flags)
{
    struct sockaddr sockname;
    int namelen;
    char req_ip[17];
    int r;

    //ASSERT(nread >= 0);
    //free(buf.base);

    if (nread == 0) {
        return;
    }

    memset(&sockname, -1, sizeof sockname);
    namelen = sizeof(sockname);
    r = uv_udp_getsockname(handle, &sockname, &namelen);
    //ASSERT(r == 0);
    struct sockaddr_in *req_info = (struct sockaddr_in *)addr;
    r = uv_ip4_name(req_info, (char*) req_ip, sizeof req_ip);
    cout<<req_ip<<" p :"<< ntohs(req_info->sin_port)<<endl;

    //uv_close((uv_handle_t*) handle, NULL);
}

void start(int tcp_port,int udp_port)
{
    loop=uv_default_loop();
    init_tcp(tcp_port);
    init_udp(udp_port);
    uv_run(loop, UV_RUN_DEFAULT);
}

void init_tcp(int tcp_port)
{



    uv_tcp_init(loop,&tcp_server);

    struct sockaddr_in tcp_addr = uv_ip4_addr("0.0.0.0", tcp_port);

    uv_tcp_bind(&tcp_server,tcp_addr);

    int ret = uv_listen((uv_stream_t *) &tcp_server,200,tcp_on_connection);

}

void init_udp(int udp_port)
{


    uv_udp_init(loop,&udp_server);

    struct sockaddr_in udp_addr = uv_ip4_addr("0.0.0.0", udp_port);

    uv_udp_bind(&udp_server,udp_addr,0);

    int ret = uv_udp_recv_start(&udp_server, read_buffer, udp_on_message);

    //return 0;
}

}}


//ForwServer::ForwServer(int tcp_port,int udp_port)
////    :tcp_port_(tcp_port),
////      udp_port_(udp_port)
//{
//    //start(tcp_port,udp_port);
//}
//ForwServer::ForwServer()
//{

//}


//int ForwServer::start_()
//{


//}


//void ForwServer::onConnection(struct evconnlistener *listener,
//                              evutil_socket_t fd, struct sockaddr *address, int socklen,
//                              void *ctx)
//{

//}


//void ForwServer::onAcceptError(struct evconnlistener *listener, void *ctx)
//{

//}


//void ForwServer::onMessage(struct bufferevent *bev, void *ctx)
//{

//}

//void ForwServer::onCloseConn(struct bufferevent *bev, short events, void *ctx)
//{

//}
