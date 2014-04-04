#include "forwserver.h"
#include <string.h>

using namespace Dosee::ForwNode ;

ForwServer::ForwServer(int tcp_port,int udp_port)
    :tcp_port_(tcp_port),
      udp_port_(udp_port)
{

}
ForwServer::ForwServer()
{

}


int ForwServer::start_tcp()
{
    struct event_base *base_;
    struct evconnlistener *listener_;

    if (tcp_port_<=0 || tcp_port_>65535) {
        puts("user default");
        tcp_port_ = 19876;
    }

    base_ = event_base_new();
    if (!base_) {
        puts("Couldn't open event base");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    sin.sin_port = htons(tcp_port_);

    struct sockaddr_in sin2;
    memset(&sin2, 0, sizeof(sin2));
    /* This is an INET address */
    sin2.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin2.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    sin2.sin_port = htons(udp_port_);

    listener_ = evconnlistener_new_bind(base_, &ForwServer::onConnection, NULL,
                                        LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
                                        (struct sockaddr*)&sin, sizeof(sin));
    if (!listener_) {
        perror("Couldn't create listener");
        return 1;
    }

    evconnlistener_set_error_cb(listener_, &ForwServer::onAcceptError);

    event_base_dispatch(base_);

}


void ForwServer::onConnection(struct evconnlistener *listener,
                              evutil_socket_t fd, struct sockaddr *address, int socklen,
                              void *ctx)
{
    /* We got a new connection! Set up a bufferevent for it. */
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(
                base, fd, BEV_OPT_CLOSE_ON_FREE);

    sockaddr_in sin;
    ::memcpy(&sin, address, sizeof(sin));

    cout<< inet_ntoa(sin.sin_addr)<<":"<<ntohs(sin.sin_port)<<endl;

    bufferevent_setcb(bev, ForwServer::onMessage, NULL, ForwServer::onCloseConn, NULL);

    bufferevent_enable(bev, EV_READ|EV_WRITE);
}


void ForwServer::onAcceptError(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    //    int err = EVUTIL_SOCKET_ERROR();
    //    fprintf(stderr, "Got an error %d (%s) on the listener. "
    //            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}


void ForwServer::onMessage(struct bufferevent *bev, void *ctx)
{
    /* This callback is invoked when there is data to read on bev. */
    struct evbuffer *input = bufferevent_get_input(bev);
    struct evbuffer *output = bufferevent_get_output(bev);
    int read_length=evbuffer_get_length(input);
    cout<<"input lenght:"<<read_length<<endl;
    void * data=malloc(read_length);
    evbuffer_remove(input,data,read_length);


    /* Copy all the data from the input buffer to the output buffer. */
    evbuffer_add_buffer(output, input);
}

void ForwServer::onCloseConn(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR)
        perror("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
}
