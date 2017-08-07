//
//  preprocessor.h
//  PCKAD-v2
//
//  Created by Luciano on 27/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_preprocessor_h
#define PCKAD_v2_preprocessor_h

#include "pktrepr.h"

#define PRPROC_OMEM 3
#define PRPROC_INIT_OK 4
#define PRPROC_INIT_FAIL 5
#define PRPROC_MALFORMED 6
#define PRPROC_UNINTERESTING 7 // Types of packets to ignore
#define PRPROC_UNSUPPORTED 8 // Protocol not yet supported
#define PRPROC_OK 9

/*Data structure containing network packet-related data.*/
typedef struct prp_data
{
    unsigned char *pkt_payload;
    unsigned int len;// payload length
    unsigned short port;// TCP port
    char dirc;// packet direction
}prp_data;


/*
 Initialises the module

 @cnfs_path: Configuration file path for the preprocessing modules.
 
 returns: A code representing the operation result.
 */
int preprocessor_init(const char *cnfs_path);

/*
 A network packet payload, contained in pdata, is processed to build a PCkAD packet for the subsequent phases.

 @ppkt: A PCkAD packet, derived from a network packet payload.
 @pdata: A network packet payload to convert to a PCkAD packet.
 
 returns: A code representing the operation result.
 */
int preprocess (pckad_pkt *ppkt, prp_data *pdata);


/*
 Frees the memory reserved for the pre-processing modules.
 */
void preprocessor_destr();


prp_data* create_prp_data();


void free_prp_data(prp_data* prpd);

#endif



