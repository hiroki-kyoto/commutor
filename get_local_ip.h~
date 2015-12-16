#ifndef GET_LOCAL_IP_H
#define GET_LOCAL_IP_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#define ETH_NAME        "eth0"
#define WLAN_NAME	"wlan0"

int get_local_ip(char *ip_str) // ip_str has length of 20
{
        int sock;
        struct sockaddr_in sin;
        struct ifreq ifr;
        
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == -1)
        {
                printf("socket error\n");
                return -1;                
        }
        
        strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = 0;
        
        if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
        {
                printf("call [ioctl] error\n");
		printf("cannot get eth address, getting wlan instead.\n");
		
		strncpy(ifr.ifr_name, WLAN_NAME, IFNAMSIZ);
		ifr.ifr_name[IFNAMSIZ - 1] = 0;
		if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
		{
			printf("call [ioctl] error\n");
			printf("unable to get ipv4 address.\n");
			return -2;
		}
        }

        memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
        sprintf(ip_str, "%s", inet_ntoa(sin.sin_addr));
        
        return 0;
}

#endif

