//
//  training_module.h
//  PCKAD-v2
//
//  Created by Luciano on 03/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef training_module_h
#define training_module_h

#include "glist.h"
#include "pckad_sysconfig.h"

/*
 It incrementally builds models representing different types of network traffic.
 Each incremental step involves the processing of a single network packet.
 It returns a numeric code representing the operation result.
 
 @pkt: A network packet, containing both headers and payload.
 @plen: The packet length.
 @models: List containing the models.
 @scnfg: System configuration.
 
 returns: a numeric code representing the operation result.
 */
int train_on_pkt(const unsigned char *pkt, unsigned int plen, glist *models, pckad_sysconfig *scnfg);

/*
 Performs operations needed at the end of the training phase.
 
 @Models: list of the models built by PCkAD.
 @scnfg: System configuration.
*/
void end_training (glist *models, pckad_sysconfig *scnfg);

#endif /* training_module_h */
