//
//  core_functionalities.h
//  PCKAD-v2
//
//  Created by Luciano on 08/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef core_functionalities_h
#define core_functionalities_h

#include "pckad_model.h"
#include "pktrepr.h"
#include "glist.h"
#include "pckad_sysconfig.h"

/*
 This function pointer abstracts functions that implement the incremental construction of a model mdl, 
 by inspecting the relevant payload of network packets. Indeed, different algorithms can be employed for building a model.
 The process is based on the concepts of the ngrams and chunks.
 
 @mdl: The model to build.
 @pd: The relevant payload of a network packet.
 @n: The ngram length.
 @lck: The chunk length.
 */
typedef void (*build_model) (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

/*
 Checks if the model mdl matches the relevant payload of packet ppkt, indexed by prtnidx.
 
 @mdl: Model representing a specific network traffic.
 @ppkt: A PCkAD packet.
 @prtnidx: Index pointing at the relevant payload to analyse.
 @lck: The chunk length.
 
 returns: 1 if mdl matches the relevant payload, 0 otherwise.
 */
char check_profile (pckad_model *mdl, pckad_pkt *ppkt, unsigned int prtnidx, unsigned short lck);

/*
 Searches for a model that approximately matches the relevant payload of packet ppkt. 
 It returns the index of the model.
 
 @mdls: List containing the models built by PCkAD.
 @ppkt: A PCkAD packet.
 @prtnidx: Index of the relevant payload of packet ppkt.
 @lck: The chunk length.
 
 returns: Index of the model.
 */
unsigned int lookfor_replacement (glist *mdls, pckad_pkt *ppkt, unsigned int prtnidx, unsigned short lck);

/*
 Function pointer that abstracts functions that compute the anomaly score of a given relevant payload pd, 
 by comparing pd with a matching model mdl. indeed, different algorithms can be employed for this task. 
 The computations is based on the concepts of ngrams and chunks.

 @mdl: The matching model, used to compute the anomaly score.
 @pd: The relevant payload for which the function computes the anomaly score.
 @n: The $n$-gram length.
 @lck: The title length.
 @mechanism: Denotes the mechanism used for computing the anomaly score (.g. Z-score).
 
 returns: The anomaly score of pd.
 */
typedef float (*compute_as) (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

#endif /* core_functionalities_h */
