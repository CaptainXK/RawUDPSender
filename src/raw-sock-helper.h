#ifndef _RAW_SOCK_H_
#define _RAW_SOCK_H_

#include <inttypes.h>

void InitRawSock();
void PerformTrans(uint16_t src_port, uint16_t dst_port, const char * src_addr_str, const char * dst_addr_str);
void CloseRawSock();

#endif
