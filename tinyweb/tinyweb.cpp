#include "tinyweb.h"
#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <memory.h>

uv_tcp_t _server;
uv_tcp_t _client;
uv_loop_t* _loop;
uv_udp_t _server_udp;
uv_udp_t _client_udp;

//loop = uv_default_loop();

//   uv_udp_init(loop, &recv_socket);
//   struct sockaddr_in recv_addr = uv_ip4_addr("0.0.0.0", 68);
//   uv_udp_bind(&recv_socket, recv_addr, 0);
//   uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);

//   uv_udp_init(loop, &send_socket);
//   uv_udp_bind(&send_socket, uv_ip4_addr("0.0.0.0", 0), 0);
//   uv_udp_set_broadcast(&send_socket, 1);

//   uv_udp_send_t send_req;
//   uv_buf_t discover_msg = make_discover_msg(&send_req);

//   struct sockaddr_in send_addr = uv_ip4_addr("255.255.255.255", 67);
//   uv_udp_send(&send_req, &send_socket, &discover_msg, 1, send_addr, on_send);

//   return uv_run(loop, UV_RUN_DEFAULT);

//void tinyweb_start(uv_loop_t* loop, const char* ip, int port) {
//    //char * temp_ip = (ip&&ip[0])?ip:"0.0.0.0" ;
//    _loop = loop;
//    uv_tcp_init(_loop, &_server);
//     struct sockaddr_in bind_addr ;
//             uv_ip4_addr(ip, port,&bind_addr) ;
//    uv_tcp_bind(&_server, &bind_addr,0);
//    uv_listen((uv_stream_t*)&_server, 8, tinyweb_on_connection);
//}

void tinyweb_start(uv_loop_t* loop, const char* ip, int port) {
   _loop = loop;
   uv_tcp_init(_loop, &_server);
   uv_tcp_bind(&_server, uv_ip4_addr(ip&&ip[0]?ip:"0.0.0.0", port));
//   struct sockaddr_in bind_addr = uv_ip4_addr("0.0.0.0", 7000);
//   uv_tcp_bind(&_server, bind_addr);
   uv_listen((uv_stream_t*)&_server, 8, tinyweb_on_connection);
}

void tinyweb_start_udp(uv_loop_t* loop, const char* ip, int port) {
   _loop = loop;
   uv_udp_init(_loop, &_server_udp);
   uv_udp_bind(&_server_udp, uv_ip4_addr(ip&&ip[0]?ip:"0.0.0.0", port),0);
   uv_listen((uv_stream_t*)&_server_udp, 8, tinyweb_on_connection);
}

static void after_uv_close(uv_handle_t* handle) {
    free(handle); //uv_tcp_t* client
}

static void after_uv_write(uv_write_t* w, int status) {
    if(w->data)
        free(w->data);
    uv_close((uv_handle_t*)w->handle, after_uv_close); //close client
    free(w);
}

static void write_uv_data(uv_stream_t* stream, const void* data, unsigned int len, int need_copy_data) {
    if(data == NULL || len == 0) return;
    if(len ==(unsigned int)-1)
        len = strlen((char *)data);

    void* newdata = (void*)data;
    if(need_copy_data) {
        newdata = malloc(len);
        memcpy(newdata, data, len);
    }

    uv_buf_t buf = uv_buf_init((char *)newdata, len);
    uv_write_t* w = (uv_write_t*)malloc(sizeof(uv_write_t));
    w->data = need_copy_data ? newdata : NULL;
    //free w and w->data in after_uv_write()
    uv_write(w, stream, &buf, 1, after_uv_write);
}

static const char* http_respone = "HTTP/1.1 200 OK\r\n"
        "Content-Type:text/html;charset=utf-8\r\n"
        "Content-Length:11\r\n"
        "\r\n"
        "Hello world";

static void tinyweb_on_connection(uv_stream_t* server, int status) {
    assert(server == (uv_stream_t*)&_server);
    if(status == 0) {
        uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
        uv_tcp_init(_loop, client);
        uv_accept((uv_stream_t*)&_server, (uv_stream_t*)client);
        write_uv_data((uv_stream_t*)client, http_respone, -1, 0);
        //close client after uv_write, and free it in after_uv_close()
    }
}

static void tinyweb_on_connection_udp(uv_stream_t* server, int status) {
    assert(server == (uv_stream_t*)&_server);
    if(status == 0) {
        uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
        uv_tcp_init(_loop, client);
        uv_accept((uv_stream_t*)&_server, (uv_stream_t*)client);
        write_uv_data((uv_stream_t*)client, http_respone, -1, 0);
        //close client after uv_write, and free it in after_uv_close()
    }
}


