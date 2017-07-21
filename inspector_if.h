//
//  inspector_if.h
//  PCKAD-v2
//
//  Created by Luciano on 09/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef inspector_if_h
#define inspector_if_h

#include "pckad_sysconfig.h"
#include "pktrepr.h"
#include "preprocessor.h"

#define INSPECTION_INVALID 0
#define INSPECTION_VALID 1
#define INSPECTION_OMEM 2

/*
 inspect_rawd:
 =============
 definisce la firma per le funzioni che devono interpretare uno 
 specifico formato delle unità dati. L'obiettivo di tali funzioni è 
 quello di costruire la rappresentazione interna dell'unità dati pkt,
 richiesta dal sistema, e la struttura dati prpd, richiesta dal pre
 elaboratore.
 
 @irep: rappresentazione interna del pacchetto.
 @prpd: struttura dati richiesta dal modulo di pre elaborazione.
 @pkt: il pacchetto di rete grezzo (intestazioni + contenuto).
 @plen: lunghezza del pacchetto di rete grezzo.
 @sc: configurazione del sistema.
 
 returns: 1 se valido, 0 altrimenti.
 */
typedef char (*inspect_rawd) (pckad_pkt *irep, prp_data *prpd, const unsigned char *pkt, unsigned int plen, pckad_sysconfig *sc);

#endif /* inspector_if_h */
