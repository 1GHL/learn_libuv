#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "uv.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

static uv_loop_t* loop;

static int server_closed;
static stream_type serverType;
static uv_tcp_t tcpServer;
static uv_udp_t udpServer;
static uv_pipe_t pipeServer;
static uv_handle_t* server;

 int run_helper_tcp4_echo_server();
 int run_helper_udp4_echo_server();

 void after_write(uv_write_t* req, int status);
 void after_read(uv_stream_t*, ssize_t nread, uv_buf_t buf);
 void on_close(uv_handle_t* peer);
 void on_server_close(uv_handle_t* handle);
 void on_connection(uv_stream_t*, int status);
 uv_buf_t echo_alloc(uv_handle_t* handle, size_t suggested_size);
 void on_send(uv_udp_send_t* req, int status);
 int tcp4_echo_start(int port);
 int tcp6_echo_start(int port);
 int udp4_echo_start(int port);
 int pipe_echo_start(char* pipeName);


#endif // ECHOSERVER_H
