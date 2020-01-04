/* vim: set ts=2  noet : */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "raw-sock-helper.h"

#define PCKT_LEN 512


// socket relevant vars
int rsd = 0;
char pkt[PCKT_LEN];
struct iphdr *ip;
struct udphdr *udp;
struct sockaddr_in sin;

unsigned short csum(unsigned short *buf, int nwords)
{
  unsigned long sum;
  for(sum=0; nwords>0; nwords--){
    sum += *buf++;
  }
  sum = (sum >> 16) + (sum &0xffff);
  sum += (sum >> 16);
  
	return (unsigned short)(~sum);
}

void FillUDPHeader(struct udphdr *udp, uint16_t src_port, uint16_t dst_port)
{
  // fabricate the UDP header
  udp->source = htons(src_port);
  // destination port number
  udp->dest = htons(dst_port);
  udp->len = htons(sizeof(struct udphdr));
}

size_t FillIPHeader(struct iphdr *ip, uint32_t src_addr, uint32_t dst_addr)
{
  // fabricate the IP header
  ip->ihl      = 5;
  ip->version  = 4;
  ip->tos      = 16; // low delay
  ip->tot_len  = sizeof(struct iphdr) + sizeof(struct udphdr);
  ip->id       = htons(54321);
  ip->ttl      = 64; // hops
  ip->protocol = 17; // UDP
  
  // source IP address, can use fake address here
  ip->saddr = src_addr;
  ip->daddr = dst_addr;
  
  // calculate the checksum for integrity
  ip->check = csum((unsigned short *)ip,
                   sizeof(struct iphdr) + sizeof(struct udphdr));

	return ip->tot_len;
}

void CreateRawUDPSocket()
{    
  int one = 1;
  const int *val = &one;
	int ret = 0;
  
	// create a raw socket with UDP protocol
  rsd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
  if (rsd < 0) {
    perror("socket() error");
    exit(2);
  }

  // inform the kernel do not fill up the packet structure, we will build our own
	//ret =	setuid(getppid());
	//if( ret != 0){
	//	perror("Request root:");
	//	exit(2);
	//}
	
  if(setsockopt(rsd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
    perror("setsockopt() error");
    exit(2);
  }
  printf("OK: socket option IP_HDRINCL is set.\n");

	//all done
  printf("OK: a raw socket is created.\n");
}

void InitSockAddr(uint16_t dst_port, uint32_t dst_addr)
{
  sin.sin_family = AF_INET;
  sin.sin_port = htons(dst_port);
  sin.sin_addr.s_addr = dst_addr;
}

size_t FillPktHdr(uint16_t src_port, uint16_t dst_port, uint32_t src_addr, uint32_t dst_addr)
{
	size_t tot_len = 0;	

  memset(pkt, 0, PCKT_LEN);
	
  udp = (struct udphdr *) (pkt + sizeof(struct iphdr));
	FillUDPHeader(udp, src_port, dst_port);

  ip = (struct iphdr *) pkt;
	tot_len =	FillIPHeader(ip, src_addr, dst_addr);

	return tot_len;
}

void InitRawSock()
{
	//create a raw socket
	CreateRawUDPSocket();
}

void PerformTrans(uint16_t src_port, uint16_t dst_port, const char * src_addr_str, const char * dst_addr_str)
{
	// convert addr format
	uint32_t src_addr = inet_addr(src_addr_str);
	uint32_t dst_addr = inet_addr(dst_addr_str);
	size_t tot_len = 0;	
	
	////create a raw socket
	//CreateRawUDPSocket();

	//init sock addr
	InitSockAddr(dst_port, dst_addr);

	//Fill pkt headers of all layers
	tot_len = FillPktHdr(src_port, dst_port, src_addr, dst_addr);

	//do send
	if(sendto(rsd, pkt, tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("Sendto error");
		exit(2);
	}	
	printf("Send Done!\n");

	//close(rsd);
}

void CloseRawSock()
{
	close(rsd);
}







