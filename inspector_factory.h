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
 get_inspector:
 ==============
 restituisce il puntatore di una funzione che implementa l'interpretazione
 del formato dei dati richiesto dal sistema, specificato usando il parametro di input
 format.
 
 @format: specifica il formato dei dati grezzi.
 
 returns: il puntatore alla funzione che interpreta il formato specificato in input.
 */
inspect_rawd get_inspector(char format);


#endif /* inspector_factory_h */
