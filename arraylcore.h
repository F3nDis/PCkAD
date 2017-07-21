//
//  arraylcore.h
//  PCKAD-v2
//
//  Created by Luciano on 27/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_arraylcore_h
#define PCKAD_v2_arraylcore_h


typedef struct glist
{
    unsigned int size;// indica la lunghezza della lista
    size_t data_size;// indica la dimensione dei dati dei nodi
    unsigned int len;// punta alla prima cella libera e fornisce un'indicazione del numero di celle occupate
    void **data;
}glist;

#endif
