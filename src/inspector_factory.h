//
//  inspector_factory.h
//  PCKAD-v2
//
//  Created by Luciano on 09/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef inspector_factory_h
#define inspector_factory_h

#include "inspector_if.h"

/*
 Provides the factory function for functions that interpret specific packet formats with the aim of 
 building an internal representation for network packets. It returns the pointer to the function
 implementing the desired strategy based on the input parameter format.
 
 @format: Flag used to choose the strategy for interpreting a specific packet format.
 
 returns: pointer to the function implementing the desired strategy.
 */
inspect_rawd get_inspector(char format);


#endif /* inspector_factory_h */
