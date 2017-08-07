//
//  build_factory.h
//  PCKAD-v2
//
//  Created by Luciano on 10/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef build_factory_h
#define build_factory_h

#include "core_functionalities.h"

/*
 Provides a factory function for functions that implement the incremental construction of a model, during the training phase.
 
 @strategy: flag used to choose the construction strategy.
 
 returns: A pointer to the function that implements the desired strategy.
 */
build_model get_build_model(char strategy);

#endif /* build_factory_h */
