//
//  classification_module.h
//  PCKAD-v2
//
//  Created by Luciano on 03/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef classification_module_h
#define classification_module_h

#include "glist.h"
#include "pckad_sysconfig.h"

#define CLMODEL_MISSING 10

/* Describes the packet payload for which the anomaly score is computed. */
typedef struct result_description
{
    unsigned char *content;
    unsigned int len;
    unsigned short port;
    char dir;
}result_description;

/*
 Computes the anomaly score of a network packet pkt, whose length is specified by plen. The classification is performed based on
 the comparison between pkt's relevant payload and its matching model. The model is retrieved from models.
 It returns a numeric code that represents the operational result. The code is assigned to the input parameter rescode.
 Finally, it returns the anomaly score of pkt.
 
 @pkt: A network packet, containing both headers and payload.
 @plen: The packet length.
 @models: List containing the models.
 @scnfg: System configuration.
 @rescode: Numeric code that represents the operational result.
 
 returns: anomaly score of pkt.
 */
float analyse_pkt (const unsigned char *pkt, unsigned int plen, glist *models, pckad_sysconfig *scnfg, int *rescode, result_description *rd);

/*
 Creates an instance of the result_description structure.
 
 Results: an instance of the result_description structure.
*/
result_description* create_result_description();

/*
 Frees the memory reserved for rd.
 
 @rd: an instance of the result_description structure.
*/
void free_result_description(result_description *rd);
#endif /* classification_module_h */
