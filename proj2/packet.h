#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdio.h>
#include <stdlib.h>

#define MAC_LEN	6

struct ethernetHeader {
	u_char destAddr[MAC_LEN];
	u_char sourceAddr[MAC_LEN];
	u_short etherType;
};

#endif
