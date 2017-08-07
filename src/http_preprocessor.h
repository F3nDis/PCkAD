//
//  http_preprocessor.h
//  PCKAD-v2
//
//  Created by Luciano on 18/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef http_preprocessor_h
#define http_preprocessor_h

#include "pktrepr.h"

#define HTTP_LCNF_OK 1
#define HTTP_LCNF_FAIL 2
#define HTTP_LCNF_OUT_OF_MEM 0

#define HTTP_PRPROC_OK 1
#define HTTP_PRPROC_UNINTERESTING 2
#define HTTP_PRPROC_MALFORMED 3
#define HTTP_PRPROC_OUT_OF_MEM 0

/*
 Initialises the HTTP preprocessing library.
 
 @cnfs_path: Configuration file path.
 
 returns: code representing in the operation result.
 */
int http_module_init(const char *cnfs_path);

/*
 Processes an HTTP payload, represented by the input parameter payload, in order to
 extract its relevant payload and derive a PCkAD packet. Two types of HTTP payloads are supported so far,
 i.e. GET and POST payloads.
 When onlyparams = 1 then PCkAD considers only the URL parameters (body) as the relevant payload of 
 a GET (POST) packet, otherwise the URL is also considered part of the relevant payload.
 
 @ppkt: Structure representing a PCkAD packet.
 @payload: Contains an HTTP payload.
 @plen: The payload length.
 
 returns: code representing the operation result.
 */
int http_preprocess (pckad_pkt *ppkt, unsigned char *payload, unsigned int plen);

/*
 Frees the memory reserved for the HTTP preprocessing library.
 */
void http_module_destr();

#endif /* http_preprocessor_h */
