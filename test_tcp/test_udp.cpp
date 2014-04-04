#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ev.h>

#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>

#define DEFAULT_PORT    9999
#define BUF_SIZE        4096

// Lots of globals, what's the best way to get rid of these?
int sd; // socket descriptor
struct sockaddr_in addr;
int addr_len = sizeof(addr);
char buffer[BUF_SIZE];

// This callback is called when data is readable on the UDP socket.
static void udp_cb(EV_P_ ev_io *w, int revents) {
    puts("udp socket has become readable");
    char buffer[1024]={0,};
//    int bytes = recvfrom(sd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*) &addr, &addr_len);
//    // add a null to terminate the input, as we're going to use it as a
//    buffer[bytes] = '\0';

//    printf("udp client said: %s", buffer);

//    // Echo it back.
//    // WARNING: this is probably not the right way to do it with libev.
//    // Question: should we be setting a callback on sd becomming writable
//    //here instead?
//    sendto(sd, buffer, bytes, 0, (struct sockaddr*) &addr, sizeof(addr));


    int len = recvfrom(w->fd,buffer,sizeof(buffer), 0 , (struct sockaddr *)&addr ,(socklen_t *)addr_len);
    /*显示client端的网络地址*/
    //printf("receive from %s\n" , inet_ntoa( addr.sin_addr));
    /*将字串返回给client端*/
    sendto(w->fd,buffer,strlen(buffer),0,(struct sockaddr *)&addr,addr_len);
}

int main45(void) {
    int port = DEFAULT_PORT;
    puts("udp_echo server started...");

    // Setup a udp listening socket.
    sd = socket(PF_INET, SOCK_DGRAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
        perror("bind");

    // Do the libev stuff.
    struct ev_loop *loop = ev_default_loop(0);
    ev_io udp_watcher;
    ev_io_init(&udp_watcher, udp_cb, sd, EV_READ);
    ev_io_start(loop, &udp_watcher);
    ev_loop(loop, 0);

    // This point is never reached.
    //close(sd);
    return EXIT_SUCCESS;
}

