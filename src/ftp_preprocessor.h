//
//  ftp_preprocessor.h
//  PCKAD-v2
//
//  Created by Luciano on 18/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef ftp_preprocessor_h
#define ftp_preprocessor_h

#include "pktrepr.h"

#define FTP_LCNF_OK 1
#define FTP_LCNF_FAIL 2
#define FTP_LCNF_OUT_OF_MEM 0

#define FTP_PRPROC_OK 0
#define FTP_PRPROC_OUT_OF_MEM 1

/*
 Initialises the FTP preprocessing library.
 
 @cnfs_path: Configuration file path.
 
 returns: code representing in the operation result.
 */
int ftp_module_init(const char *cnfs_path);

/*
 Processes an FTP payload, represented by the input parameter payload, in order to
 extract its relevant payload and derive a PCkAD packet. PCkAD considers the entire FTP payload as relevant,
 therefore it just build a PCkAD packet.
 
 @ppkt: Structure representing a PCkAD packet.
 @payload: Contains an FTP payload.
 @plen: The payload length.
 
 returns: code representing the operation result.
 */
int ftp_preprocess (pckad_pkt *ppkt, unsigned char *payload, unsigned int plen);

/*
 Frees the memory reserved for the FTP preprocessing library.
 */
void ftp_module_destr();

#endif /* ftp_preprocessor_h */
