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
 get_compute_as:
 ===============
 implementa un factory per la funzione di calcolo dell'indice
 di anomalia di un'unità dati.
 
 @strategy: discriminante della strategia di apprendimento.
 @mechanism: discriminante dell'algoritmo impiegato per il calcolo dell'indice di anomalia.
 
 returns: puntatore alla funzione inplementa la strategia desiderata.
 */
compute_as get_compute_as(char strategy, char mechanism);

#endif /* cmptas_factory_h */
