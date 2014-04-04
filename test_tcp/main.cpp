#include <iostream>
#include "uv.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

void after_write(uv_write_t* req, int status)
{
    write_req_t* wr;
    uv_err_t err;

    /* Free the read/write buffer and the request */
    wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);

    if (status == 0)
        return;

    err = uv_last_error(loop);
    fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));

    if (err.code == UV_ECANCELED)
        return;

    ASSERT(err.code == UV_EPIPE);
    uv_close((uv_handle_t*)req->handle, NULL);
}


void onMessage(uv_stream_t* handle, ssize_t nread, uv_buf_t buf)
{
    int i;
    write_req_t *wr;
    uv_shutdown_t* req;

    if (nread < 0) {
        /* Error or EOF */
        ASSERT (uv_last_error(loop).code == UV_EOF);

        if (buf.base) {
            free(buf.base);
        }

        req = (uv_shutdown_t*) malloc(sizeof *req);
        uv_shutdown(req, handle, NULL); //#fix

        return;
    }

    if (nread == 0) {
        /* Everything OK, but nothing read. */
        free(buf.base);
        return;
    }

    /*
   * Scan for the letter Q which signals that we should quit the server.
   * If we get QS it means close the stream.
   */

    for (i = 0; i < nread; i++) {
        if (buf.base[i] == 'Q') {
            if (i + 1 < nread && buf.base[i + 1] == 'S') {
                free(buf.base);
                uv_close((uv_handle_t*)handle, NULL);//#fix
                return;
            } else {
                uv_close(server, NULL); //#fix
            }
        }
    }


    wr = (write_req_t*) malloc(sizeof *wr);

    wr->buf = uv_buf_init(buf.base, nread);
    if (uv_write(&wr->req, handle, &wr->buf, 1, NULL)) { //#fix
        FATAL("uv_write failed");
    }
}

void onConnection(uv_stream_t* server, int status)
{
    uv_stream_t* stream;
    int r;

    if (status != 0) {
        fprintf(stderr, "Connect error %d\n",
                uv_last_error(loop).code);
    }


    stream = (uv_stream_t*)malloc(sizeof(uv_tcp_t));

    r = uv_tcp_init(loop, (uv_tcp_t*)stream);
    ASSERT(r == 0);

    /* associate server with stream */
    stream->data = server;

    r = uv_accept(server, stream);
    ASSERT(r == 0);

    r = uv_read_start(stream, alloc, onMessage);
    ASSERT(r == 0);
}


int main()
{

    cout << "Hello World!" << endl;
    uv_loop_t *loop = uv_default_loop();

    struct sockaddr_in addr = uv_ip4_addr("0.0.0.0", 10241);
    int r;
    uv_tcp_t tcpServer;

    r = uv_tcp_init(loop, &tcpServer);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Socket creation error\n");
        return 1;
    }

    r = uv_tcp_bind(&tcpServer, addr);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Bind error\n");
        return 1;
    }

    r = uv_listen((uv_stream_t*)&tcpServer, 10, onConnection);
    if (r) {
        /* TODO: Error codes */
        fprintf(stderr, "Listen error %s\n",
                uv_err_name(uv_last_error(loop)));
        return 1;
    }


    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}

