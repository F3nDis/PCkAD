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
 get_build_model:
 ================
 implementa un factory per la funzione di costruzione incrementale
 di un modello, durante la fase di training.
 
 @strategy: discriminante della strategia di apprendimento.
 
 returns: puntatore alla funzione inplementa la strategia desiderata.
 */
build_model get_build_model(char strategy);

#endif /* build_factory_h */
