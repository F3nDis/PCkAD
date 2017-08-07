//
//  inspector_if.h
//  PCKAD-v2
//
//  Created by Luciano on 09/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef inspector_if_h
#define inspector_if_h

#include "pckad_sysconfig.h"
#include "pktrepr.h"
#include "preprocessor.h"

#define INSPECTION_INVALID 0
#define INSPECTION_VALID 1
#define INSPECTION_OMEM 2

/*
 Function pointer that abstracts functions that interpret specific packet formats (e.g pcap and textual formats). 
 The objective of such functions is to derive from a given network packet a PCkAD packet, used for the training and 
 classification phases, and a temporary internal representation which is used by the preprocessing module.
 
 @irep: A PCkAD packet.
 @prpd: An internal representation for a given network packet, used by the preprocessing module.
 @pkt: A network packet containing headers and apayload, regardless of its format.
 @plen: The packet length (headers + payload).
 @sc: The system configuration.
 
 returns: 1 if the packet is valid, 0 otherwise.
 */
typedef char (*inspect_rawd) (pckad_pkt *irep, prp_data *prpd, const unsigned char *pkt, unsigned int plen, pckad_sysconfig *sc);

#endif /* inspector_if_h */
