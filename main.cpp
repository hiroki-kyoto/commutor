// main.cpp
/* project header */
#include "base_def.h"
#include "udp_sender.h"
#include "udp_receiver.h"
#include "get_local_ip.h"
/* linux system header */
#include <pthread.h>
/* c++ standard library */
#include <iostream>

using namespace std;

#ifndef SENDER_DELAY
#define SENDER_DELAY 3
#endif

#ifndef RECEIVER_DELAY
#define RECEIVER_DELAY 5
#endif

#ifndef IP_LENGTH
#define IP_LENGTH	20
#endif

/*************************
 * Sender send bytes in 
 * fixed length of bytes.
 * Say, D_LEN = 1024 Byte
 ************************/

// message sender thread handler
void *thread_sender(void *arg)
{
	pthread_mutex_lock(&mutex_snd);
	memset(message_send, 0, SOCK_READ_BUF);
//	strcpy(message_send, "Hello, I am sender!");
	char ip_str[IP_LENGTH];
	while(get_local_ip(ip_str)!=0)
	{
		cout<<"Failed to get local ip address."<<endl;
		//strcat(message_send, "Error of getting IPv4 address");
		sleep(1);
	}
	strcat(message_send, ip_str);
	strcat(message_send, "#");
	strcat(message_send, CLOUD_ID);
	pthread_mutex_unlock(&mutex_snd);
	//cout<<message_send<<endl;
	while(1)
	{
	        pthread_mutex_lock(&mutex_stt);
	        // according to program state and react
                if(!strcmp(STATE, "OK"))
                {
                        // nothing to do
                }
                else if(!strcmp(STATE, "PAUSE"))
                {
                        pthread_mutex_unlock(&mutex_stt);
                        usleep(PAUSE_DELAY);
                        continue;
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
                UDP_SEND();
                usleep(SENDER_DELAY);
        }

	string str_ret = "sender thread exit normally.";
	char *ret = new char[str_ret.length() + 1];
	memcpy(ret, str_ret.c_str(), str_ret.length()+1); 

	return ret;
}

void *thread_receiver(void *arg)
{
	UDP_RECEIVE();
	
	string str_ret = "receiver thread exit normally.";
	char *ret = new char[str_ret.length() + 1];
	memcpy(ret, str_ret.c_str(), str_ret.length() + 1);
	
	return ret;
}

int main (int argc, const char * argv[])
{
	// initialization job
	if(argc<2)
	{
		cout<<"Usage: commutor cloud_id(eg: N1, meanning: the first namenode)"<<endl;
		return -1;
	}

	if(strlen(argv[1])>7)
	{
		cout<<"[CLOUD ID] exceeds limit length of 7!"<<endl;
		return -2;
	}
	
	memset(CLOUD_ID, 0, sizeof(CLOUD_ID));
	strcpy(CLOUD_ID, argv[1]);

	// update state of program
	pthread_mutex_lock(&mutex_stt);
	memset(STATE, 0, sizeof(STATE));
	strcpy(STATE, "fuck");
	pthread_mutex_unlock(&mutex_stt);

	void	*ret;
	char	msg[100] = "xiang chao"; // test only
	int	errno; // error code

	pthread_t sender;
	pthread_t receiver;
	
	errno = pthread_create(&sender, NULL, thread_sender, (void *)msg);

	// thread may not be created successfully
	if (errno != 0)
	{
		printf( "Thread [sender] creation failed.\n");
		return 1; // 1 stands for sender creation failure
	}

	cout << "Waiting for thread [sender] to finish." << endl;

	errno = pthread_create(&receiver, NULL, thread_receiver, (void *)msg);

	if (errno != 0)
	{
		printf( "Thread [receiver] creation failed.\n");
		return 2; // 2 stands for receiver thread creation failure
	}

	cout << "waiting for thread [receiver] to finish." << endl;

	// joining threads
	errno = pthread_join(sender, &ret);
	if (errno != 0)
	{
		printf("Failed to join thread [sender]\n");
		return 3; // 3 stands for sender joining failure
	}
	//else it will return code and message
	cout << "sender returned : " << (char *)ret << endl;

	// the returned pointer owns a patch of memory, has to delete it
	delete[] (char*)ret;

	errno = pthread_join(receiver, &ret);
	if (errno != 0)
	{
		printf("Failed to join thread [receiver] \n");
		return 4;
	}
	// get its return message
	cout << "receiver returned : " << (char *)ret << endl;

	// release memory created within thread receiver
	delete[] (char*)ret;
	ret = NULL;

	return 0;
}



