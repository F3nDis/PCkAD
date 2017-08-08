//
//  rawdata_inspector.h
//  PCKAD-v2
//
//  Created by Luciano on 29/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef rawdata_inspector_h
#define rawdata_inspector_h

#include "pckad_sysconfig.h"
#include "pktrepr.h"
#include "preprocessor.h"

/*
 Checks whether a PCAP packet pkt is valid, based on the global configuration file (see the target field).
 If it is then irep and prpd are initialised. It allocates memory for prpd's pkt_payload.
 
 @irep: A PCkAD packet.
 @prpd: An internal representation for a given network packet, used by the preprocessing module.
 @pkt: A network packet containing headers and apayload, regardless of its format.
 @plen: The packet length (headers + payload).
 @sc: The system configuration.
 
 returns: 1 if the packet is valid, 0 otherwise.
 */
char inspect_pcap (pckad_pkt *irep, prp_data *prpd, const unsigned char *pkt, unsigned int plen, pckad_sysconfig *sc);

#endif /* rawdata_inspector_h */
