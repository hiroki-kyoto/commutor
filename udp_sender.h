// udp_sender.h
#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include <iostream>  
#include <stdio.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <netdb.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <string.h>
#include <pthread.h>
  
using namespace std;  
  
int UDP_SEND()
{       
        setvbuf(stdout, NULL, _IONBF, 0);   
        fflush(stdout);   

        int sock = -1;  
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
        {     
                cout<<"sender: socket error"<<endl;   
                return ( 1 );
        }     

        const int opt = 1;  
        // set up broadcast socket 
        int nb = 0;  
        nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
        if(nb == -1)  
        {  
                cout<<"sender: set socket error..."<<endl;  
                return ( 2 );
        }  

        struct sockaddr_in addrto;  
        bzero(&addrto, sizeof(struct sockaddr_in));
        addrto.sin_family=AF_INET;  
        addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);  
        addrto.sin_port=htons(PORT);  
        int nlen=sizeof(addrto);

        pthread_mutex_lock(&mutex_snd);
        int ret=sendto(sock, message_send, SOCK_READ_BUF, 0, (sockaddr*)&addrto, nlen); 
        if(ret<0)  
        {  
            cout<<"sender: send error : "<<ret<<endl;
        }  
        else  
        {         
            //cout<<"send message: "<<message_send<<endl;
        }
        pthread_mutex_unlock(&mutex_snd); 
  
    return 0;  
}

#endif

