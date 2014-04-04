#include <iostream>
#include "uv.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>

void on_connection(uv_stream_t* server, int status);

//class SocketNet
//{

//public:
//    static void on_connection(uv_stream_t* server, int status)
//    {


//    }
//    void start()
//    {
//        uv_loop_t* loop=uv_default_loop();
//        uv_tcp_t server;
//        uv_tcp_init(loop,&server);

//        struct sockaddr_in bind_addr = uv_ip4_addr("0.0.0.0", 9999);
//        uv_tcp_bind(&server,bind_addr);

//        //    uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb)
//        int ret = uv_listen((uv_stream_t *) &server,-1,&SocketNet::on_connection);
//    }


//};

int main()
{
    uv_loop_t* loop=uv_default_loop();
    uv_tcp_t server;
    uv_tcp_init(loop,&server);

    struct sockaddr_in bind_addr = uv_ip4_addr("0.0.0.0", 9999);
    uv_tcp_bind(&server,bind_addr);

    //    uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb)
    int ret = uv_listen((uv_stream_t *) &server,-1,on_connection);

    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
uv_buf_t read_buffer(uv_handle_t* handle, size_t suggested_size);

void on_message(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);

void on_connection(uv_stream_t* server, int status)
{
    if (status == -1) {
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);
    if (uv_accept(server, (uv_stream_t *) client) == 0) {
//client->accepted_fd
        uv_read_start((uv_stream_t *) client, read_buffer, on_message);
    }
    else {
        uv_close((uv_handle_t *) client, NULL);
    }

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
