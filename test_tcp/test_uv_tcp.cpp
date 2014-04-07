#include <iostream>
#include "uv.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>

void on_connection(uv_stream_t* server, int status);

static void check_sockname(struct sockaddr* addr, const char* compare_ip,
                           int compare_port, const char* context)
{
    struct sockaddr_in check_addr = *(struct sockaddr_in*) addr;
    struct sockaddr_in compare_addr = uv_ip4_addr(compare_ip, compare_port);
    char check_ip[17];
    int r;

    /* Both addresses should be ipv4 */
    //ASSERT(check_addr.sin_family == AF_INET);
    //ASSERT(compare_addr.sin_family == AF_INET);

    /* Check if the ip matches */
    //  ASSERT(memcmp(&check_addr.sin_addr,
    //         &compare_addr.sin_addr,
    //         sizeof compare_addr.sin_addr) == 0);

    /* Check if the port matches. If port == 0 anything goes. */
    //ASSERT(compare_port == 0 || check_addr.sin_port == compare_addr.sin_port);

    r = uv_ip4_name(&check_addr, (char*) check_ip, sizeof check_ip);
    //ASSERT(r == 0);

    printf("%s: %s:%d\n", context, check_ip, ntohs(check_addr.sin_port));
}

#include <string.h>
void udp_recv(uv_udp_t* handle,
              ssize_t nread,
              uv_buf_t buf,
              struct sockaddr* addr,
              unsigned flags) {
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
    r = uv_ip4_name(&addr, (char*) req_ip, sizeof req_ip);


    //uv_close((uv_handle_t*) handle, NULL);
}


static void udp_send(uv_udp_send_t* req, int status) {

}

void on_recv(uv_udp_t* handle,
             ssize_t nread,
             uv_buf_t buf,
             struct sockaddr* addr,
             unsigned flags)
{
    uv_udp_send_t* req;
    int r;

    // ASSERT(nread > 0);
    //ASSERT(addr->sa_family == AF_INET);

    req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
    //ASSERT(req != NULL);

    printf("%s\n",buf.base);
    //r = uv_udp_send(req, handle, &buf, 1, *(struct sockaddr_in*)addr, on_send);
    // ASSERT(r == 0);
}

uv_buf_t read_buffer(uv_handle_t* handle, size_t suggested_size);

int main()
{
    uv_loop_t* loop=uv_default_loop();
    uv_tcp_t server;
    uv_udp_t udp_server;
    uv_tcp_init(loop,&server);
    uv_udp_init(loop,&udp_server);

    struct sockaddr_in bind_addr = uv_ip4_addr("0.0.0.0", 8888);
    struct sockaddr_in udp_addr = uv_ip4_addr("0.0.0.0", 9999);
    uv_tcp_bind(&server,bind_addr);
    uv_udp_bind(&udp_server,udp_addr,0);

    //    uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb)
    int ret = uv_listen((uv_stream_t *) &server,-1,on_connection);
    ret = uv_udp_recv_start(&udp_server, read_buffer, udp_recv);
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}


void on_message(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);

void on_connection(uv_stream_t* server, int status)
{
    if (status == -1) {
        // error!
        return;
    }

    struct sockaddr sockname, peername;
    int namelen,r;

    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);
    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        //client->accepted_fd

        uv_read_start((uv_stream_t *) client, read_buffer, on_message);

    }
    else {
        uv_close((uv_handle_t *) client, NULL);
    }

    namelen = sizeof sockname;
    r = uv_tcp_getsockname(( uv_tcp_t *)server, &sockname, &namelen);
    check_sockname(&sockname, "0.0.0.0", 9999, "server socket");
    namelen = sizeof peername;
    r = uv_tcp_getpeername(( uv_tcp_t *)client, &peername, &namelen);
    check_sockname(&peername, "127.0.0.1", 9999, "server socket");

}

uv_buf_t read_buffer(uv_handle_t* handle, size_t suggested_size)
{
    char buff[2048]={0,};
    return uv_buf_init(buff , 2048);
}

void on_message(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
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
