#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <iostream>
#include <sstream>

#include <stdio.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <netdb.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <string.h>

// custom header
#include "base_def.h"

using namespace std;

int UDP_RECEIVE() 
// rmsg: received message pointer; rsze: message size
{  
	setvbuf(stdout, NULL, _IONBF, 0);   
	fflush(stdout);   

	// bind to broadcast address
	struct sockaddr_in addrto;  
	bzero(&addrto, sizeof(struct sockaddr_in));  
	addrto.sin_family = AF_INET;  
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);  
	addrto.sin_port = htons(PORT);  

	// the broadcast address
	struct sockaddr_in from;  
	bzero(&from, sizeof(struct sockaddr_in));  
	from.sin_family = AF_INET;  
	from.sin_addr.s_addr = htonl(INADDR_ANY);  
	from.sin_port = htons(PORT);  

	int sock = -1;  
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
	{     
		cout<<"receiver: socket error"<<endl;   
		return ( 1 );  
	}     

	const int opt = 1;  

	// set the socket to be non-block mode
	// ifcntl(sock, F_SETFL, O_NONBLOCK);
	// non-block mode is too low in efficiency.

	int nb = 0;  
	nb = setsockopt(
	        sock, 
	        SOL_SOCKET, 
	        SO_BROADCAST, 
	        (char *)&opt, 
	        sizeof(opt)
	        );  
	if(nb == -1)  
	{  
		cout<<"receiver: set socket error..."<<endl;  
		return ( 2 );  
	}  

	if(bind(
	        sock,
	        (struct sockaddr *)&(addrto), 
	        sizeof(struct sockaddr_in)
	        ) == -1)   
	{     
		cout<<"receiver: bind error..."<<endl;  
		return ( 3 );  
	}  

	int len = sizeof(sockaddr_in); 
 
	bool cont = true;
	stringstream ss("");

	string session_id;		// session id
	ULL	patch_id;		// patch index
	ULL	patch_total;		// patch total count
	USI	data_length;		// data field length
	char	cloud_status;		// 'N' OR 'D', Namenode or Datanode
	char	cloud_id[C_ID_SIZE];	// 8 size unique name in cloud
	BYTE	reserved[RESERVED_SIZE];// reserved space for future use
	BYTE	IPv4[IPV4_SIZE];	// IP v4 address
	BYTE	data[GRAM_BODY_LEN];	// data field
	
	
	// under nonblock mode, try for several times and stop 
	while(cont)
	{
	        // check program state
                pthread_mutex_lock(&mutex_stt);
                if(!strcmp(STATE, "OK"))
                {
                        // nothing to do
                }
                else if(!strcmp(STATE, "PAUSE"))
                {
                        // do nothing
                }
                else if(!strcmp(STATE, "STOP"))
                {
                        pthread_mutex_unlock(&mutex_stt);
                        return (0);
                }
                else if(!strcmp(STATE, "RESET"))
                {
                        // wait and see
                }
                pthread_mutex_unlock(&mutex_stt);
	
		pthread_mutex_lock(&mutex_rcv);
		memset(message_receive, 0, SOCK_READ_BUF);
		int ret=recvfrom (
		        sock, 
		        message_receive, 
		        SOCK_READ_BUF, 
		        0, 
		        (struct sockaddr*)&from,
		        (socklen_t*)&len
                );
                pthread_mutex_unlock(&mutex_rcv);
                
                // avoid double twisted mutex lock
                // so unlock mutex ahead of time
		
		if(ret<=0)  
		{  
	    		cout<<"receiver: socket read error: "<<sock<<endl;		
		} 
		else  
		{
			cout<<message_receive<<endl;
			
			int errcode = 1;
	                errcode *= strcmp(message_receive, "STOP");
	                errcode *= strcmp(message_receive, "OK");
	                errcode *= strcmp(message_receive, "PAUSE");
	                errcode *= strcmp(message_receive, "RESET");
			
			if(errcode==0)
			{
				pthread_mutex_lock(&mutex_stt);
				memset(STATE, 0, sizeof(STATE));
				strcpy(STATE, message_receive);
				pthread_mutex_unlock(&mutex_stt);
			}
		}
		// sleep for a while
		usleep(RECEIVER_DELAY);		
	}

	return 0;
}

#endif

