#include "base_def.h"
#include "udp_sender.h"
#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	//check control parameters
	if(argc<2)
	{
		cout<<"Usage: control param<OK|PAUSE|STOP|RESET>"<<endl;
                return -1;
	}
	
	int errcode = 1;
	errcode *= strcmp(argv[1], "STOP");
	errcode *= strcmp(argv[1], "OK");
	errcode *= strcmp(argv[1], "PAUSE");
	errcode *= strcmp(argv[1], "RESET");
	
	if(errcode!=0)
        {
                cout<<"Paramter Illegal! Try: control STOP"<<endl;
                cout<<"Your parameter is: "<<argv[1]<<endl;
                return -2;
        }
        
        pthread_mutex_lock(&mutex_snd);
	memset(message_send, 0, sizeof(message_send));
	strcpy(message_send, argv[1]);
	pthread_mutex_unlock(&mutex_snd);	
	
	int i = CTL_CMD_RPT;
        while(i-->0)
        {
                UDP_SEND();
                usleep(CTL_CMD_DLY);
        }
	
	return 0;
}


