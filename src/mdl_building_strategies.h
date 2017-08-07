//
//  mdl_building_strategies.h
//  PCKAD-v2
//
//  Created by Luciano on 08/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef mdl_building_strategies_h
#define mdl_building_strategies_h

#include "pckad_model.h"

/**
 Implements an incremental construction of a model mdl, by employing the 2LS strategy.
 PCkAD learns the characteristics of mdl by inspecting the relevant payload pd
 of network packets. The learning process is based on the usage of the ngram technique
 and the chunks.
 
 @param mdl The model to build.
 @param pd The relevant payload.
 @param n The ngram length.
 @param lck The chunk length.
 */
void build_2lsmdl (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

/**
 Implements an incremental construction of a model mdl, by employing the 2LS strategy.
 PCkAD learns the characteristics of mdl by inspecting the relevant payload pd
 of network packets. The learning process is based on the usage of the ngram technique
 and the chunks.
 
 @param mdl The model to build.
 @param pd The relevant payload.
 @param n The ngram length.
 @param lck The chunk length.
 */
void build_lsmdl (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

/**
 Implements an incremental construction of a model mdl, by employing the 2LS strategy.
 PCkAD learns the characteristics of mdl by inspecting the relevant payload pd
 of network packets. The learning process is based on only the usage of the ngram technique.
 
 @param mdl The model to build.
 @param pd The relevant payload.
 @param n The ngram length.
 @param lck The chunk length.
 */
void build_gsmdl (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

#endif /* mdl_building_strategies_h */
