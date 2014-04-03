#ifndef TINYWEB_H
#define TINYWEB_H

#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <memory.h>


static void tinyweb_on_connection(uv_stream_t* server, int status);
void tinyweb_start(uv_loop_t* loop, const char* ip, int port);
void tinyweb_start_udp(uv_loop_t* loop, const char* ip, int port);
static void after_uv_close(uv_handle_t* handle);
static void after_uv_write(uv_write_t* w, int status);
static void write_uv_data(uv_stream_t* stream, const void* data, unsigned int len, int need_copy_data);
static void tinyweb_on_connection(uv_stream_t* server, int status);

#endif // TINYWEB_H
