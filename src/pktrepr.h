//
//  pktrepr.h
//  PCKAD-v2
//
//  Created by Luciano on 27/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PktReprLib_pktrepr_h
#define PktReprLib_pktrepr_h

#include "glist.h"

#define SRC_PORT 0
#define DEST_PORT 1

/*
 Structure describing a relevant payload..
 */
typedef struct pkt_data
{
    unsigned char *data;// The relevant payload
    unsigned int len;// the payload length (does not take into account the null character)
    char type;// denotes the type of payload (see protocol codes)
}pkt_data;

/*
 Internal representation of the network packet, here referred to as PCkAD packet. 
 It contains both header and payload data derived from a network packet. 
 It has been designed to contain more than one relevant payload. 
 The relevant payloads are stored in a linked list (glist), therefore there is no need to 
 know the number of payloads before constructing a PCkAD packet.
 */
typedef struct pckad_pkt
{
    // user data
    glist *portions;// List containing the relevanta payloads (pkt_data)
    
    // control data
    unsigned short tcp_port;// TCP port
    char dirc;// packet direction
}pckad_pkt;

/*
 Initialises an instance of the pckad_pkt structure. 
 Portions is initialised with default size. It returns NULL if there is not enough memory.
 
 returns: an instance of the pckad_pkt structure. NULL if there is not enough memory.
 */
pckad_pkt* pckad_pkt_dinit();

/*
 Initialises an instance of the pckad_pkt structure. 
 Portions is initialised with size nr. It returns NULL if there is not enough memory.
 
 returns: an instance of the pckad_pkt structure. NULL if there is not enough memory.
 */
pckad_pkt* pckad_pkt_sinit( unsigned int nr);

/*
 Frees the memory reserved for ppkt
 
 @ppkt: PCkAD packet.
 */
void free_pckad_pkt(pckad_pkt* ppkt);



#endif
