//
//  cmptas_factory.h
//  PCKAD-v2
//
//  Created by Luciano on 24/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef cmptas_factory_h
#define cmptas_factory_h

#include "core_functionalities.h"

/*
 Provides the factory function of functions that compute the anomaly score of a network packet.
 It takes two input parameters, that are flags used to choose how to compute the anomaly score.
 
 @strategy: Flag for choosing the classification strategy.
 @mechanism: Flag for choosing the mechanism for computing the anomaly score.
 
 returns: Pointer to the function that implements the desired anomaly score.
 */
compute_as get_compute_as(char strategy, char mechanism);

#endif /* cmptas_factory_h */
