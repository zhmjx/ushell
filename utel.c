/*
UDP telnet.
send while \n received from stdin, print to stdout while \n received from socket.
work just like TCP telnet.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFF_LEN 10240

void *udp_recv(void *msg){
    char buf[BUFF_LEN] = {0};
    int fd = (int)*(void **)msg;
    struct sockaddr *dst = (struct sockaddr *)*((void **)msg+1);
    struct sockaddr_in srv_addr;
    int len = sizeof(struct sockaddr);
    
    while(1)
    {
        int recvlen = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&srv_addr, &len);
        /*reject udp packat from other ip:port*/
        if(srv_addr.sin_addr.s_addr != ((struct sockaddr_in *)dst)->sin_addr.s_addr\ 
        || srv_addr.sin_port != ((struct sockaddr_in *)dst)->sin_port)
            continue;
        write(1, buf, recvlen);
        //printf("%s", buf);//printf() need fflush(stdout) to print immediately or setvbuf() to disable IO cache while redirect.
        memset(buf, 0, sizeof(buf));
    }
}

void *udp_send(void *msg){
    char buf[1024] = {0};
    int fd = (int)*(void **)msg;
    struct sockaddr *dst = (struct sockaddr *)*((void **)msg+1);
    int len = sizeof(*dst);
    
    sendto(fd, "HI!\n", 4, 0, dst, len);
    int readlen = 0;
    while (readlen = read(0, buf, 1024)){
        sendto(fd, buf, readlen, 0, dst, len);
    }
}

void udp_handler(int fd, struct sockaddr* dst)
{   
    pthread_t thread1,thread2;
    void *msg[2] = {(void *)fd,(void *)dst};
    
    int iret1 = pthread_create(&thread1, NULL, udp_send, (void *)msg);
    int iret2 = pthread_create(&thread2, NULL, udp_recv, (void *)msg);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

    return;
}

int utel_main(int argc, char* argv[]/*, char * env[]*/)
{
    if(argc != 4)
    {
        printf("%s -u ip port\n", argv[0]);
        return -1;
    }
    char ip[255] = {0}, port[255] = {0};
    strncpy(ip, argv[2],254);// arg after "ushell -u"
    strncpy(port, argv[3],254);

    /*hide cmd line*/
    /*
    int cmd_len = strlen(argv[0]), arg1_len = strlen(argv[1]), arg2_len = strlen(argv[2]);
    strncpy(argv[0],"ps",cmd_len);
    strncpy(argv[1]," ",arg1_len);
    strncpy(argv[2]," ",arg2_len);
    */
    
    int client_fd;
    struct sockaddr_in srv_addr;
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(ip);
    srv_addr.sin_port = htons(atoi(port));

    udp_handler(client_fd, (struct sockaddr*)&srv_addr);
    close(client_fd);
    return 0;
}

