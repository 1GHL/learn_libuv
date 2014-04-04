#include <iostream>
#include "uv.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>

using namespace std;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

typedef struct _conn_rec{
    uv_tcp_t handle;
    uv_shutdown_t shutdown_req;
} conn_rec;

static uv_tcp_t tcp_server;

static uv_buf_t alloc_cb(uv_handle_t* handle, size_t suggested_size) {
    static char buf[1024];
    return uv_buf_init(buf, sizeof buf);
}

//void after_write(uv_write_t* req, int status)
//{
//    write_req_t* wr;
//    uv_err_t err;

//    /* Free the read/write buffer and the request */
//    wr = (write_req_t*) req;
//    free(wr->buf.base);
//    free(wr);

//    if (status == 0)
//        return;

//    err = uv_last_error(loop);
//    fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));

//    if (err.code == UV_ECANCELED)
//        return;

//    //ASSERT(err.code == UV_EPIPE);
//    uv_close((uv_handle_t*)req->handle, NULL);
//}

//#define container_of(ptr, type, member) ({              \
//const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
//(type *)( (char *)__mptr - offsetof(type,member) );})

void onMessage2(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
{

    //conn_rec* conn;

    int r;

    if (nread == 0)
    {
        return ;
    }else if(nread >0 && nread < buf.len)
    {
        if(buf.base[nread-1]=='\n' && buf.base[nread-2]=='\r')
        {
          //  uv_read_stop(stream);
        }
        printf("ok...\n");
        // uv_read_stop(stream);
        return ;
    }

    // conn = container_of(stream,conn_rec,conn_rec.handle);
    // conn = container_of(stream , conn_rec , handle );

    //r = uv_shutdown(stream->shutdown_req, stream, uv_close);



}

//void onMessage(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
//{

//    conn_rec* conn;
//      int r;

//      if (nread >= 0)
//        return;


//      conn = container_of(stream,conn_rec,handle);

//      r = uv_shutdown(&conn->shutdown_req,stream, NULL);
//      //ASSERT(r == 0);
////    int i;
////    write_req_t *wr;
////    uv_shutdown_t* req;

////    if (nread < 0) {
////        /* Error or EOF */
////        //ASSERT (uv_last_error(loop).code == UV_EOF);

////        if (buf.base) {
////            free(buf.base);
////        }

////        req = (uv_shutdown_t*) malloc(sizeof *req);
////        uv_shutdown(req, handle, NULL); //#fix

////        return;
////    }

////    if (nread == 0) {
////        /* Everything OK, but nothing read. */
////        free(buf.base);
////        return;
////    }

////    /*
////   * Scan for the letter Q which signals that we should quit the server.
////   * If we get QS it means close the stream.
////   */

//////    for (i = 0; i < nread; i++) {
//////        if (buf.base[i] == 'Q') {
//////            if (i + 1 < nread && buf.base[i + 1] == 'S') {
//////                free(buf.base);
//////                uv_close((uv_handle_t*)handle, NULL);//#fix
//////                return;
//////            } else {
//////                uv_close(server, NULL); //#fix
//////            }
//////        }
//////    }


////    wr = (write_req_t*) malloc(sizeof *wr);

////    wr->buf = uv_buf_init(buf.base, nread);
////    if (uv_write(&wr->req, handle, &wr->buf, 1, NULL)) { //#fix
////        //FATAL("uv_write failed");
////    }
//}

void onConnection(uv_stream_t* stream, int status)
{

    conn_rec* conn;
    int r;

    conn = (conn_rec *)malloc(sizeof(conn_rec));

    r = uv_tcp_init(stream->loop, &conn->handle);
    // ASSERT(r == 0);

    r = uv_accept(stream, (uv_stream_t*)&conn->handle);
    //  ASSERT(r == 0);

    r = uv_read_start((uv_stream_t*)&conn->handle, alloc_cb, onMessage2);

}


int main77()
{

    cout << "Hello World!" << endl;
    uv_loop_t *loop = uv_default_loop();

    struct sockaddr_in addr = uv_ip4_addr("0.0.0.0", 9999);
    int r;
    // uv_tcp_t tcpServer;

    r = uv_tcp_init(loop, &tcp_server);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Socket creation error\n");
        return 1;
    }

    r = uv_tcp_bind(&tcp_server, addr);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Bind error\n");
        return 1;
    }

    r = uv_listen((uv_stream_t*)&tcp_server, 10, onConnection);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Listen error %s\n",
                uv_err_name(uv_last_error(loop)));
        return 1;
    }


    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}

