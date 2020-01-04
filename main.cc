/* vim: set ts=2  noet : */
/*
    An example of using raw sockets.
    You can capture packets by tcpdump:
        tcpdump -X -s0 -i eth0 -p udp
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "raw-sock-helper.h"
#include "field-list-helper.h"

#include <vector>


int main(int argc, char const *argv[])
{
	//default config
	const uint16_t default_src_port = 65535, default_dst_port = 65535;
	const char * default_src_ip = "192.168.111.111";  
	const char * default_dst_ip = "192.168.222.222";  
	
	std::vector<uint16_t> src_p_list = CreateFieldList(default_src_port);
	std::vector<uint16_t> dst_p_list = CreateFieldList(default_dst_port);
	std::vector<uint16_t>::iterator src_port, dst_port;

	InitRawSock();
	
	for( src_port = src_p_list.begin(); src_port != src_p_list.end(); src_port++ )
	{
		for( dst_port = dst_p_list.begin(); dst_port != dst_p_list.end(); dst_port++ )
		{
			printf("Src_port=%hu, Dst_port=%hu\n", *src_port, *dst_port);
			PerformTrans(*src_port, *dst_port, default_src_ip, default_dst_ip);		
		}
	}

	CloseRawSock();
  
	return 0;
}
