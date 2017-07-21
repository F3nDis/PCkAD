//
//  txtdata_inspector.h
//  PCKAD-v2
//
//  Created by Luciano on 08/04/17.
//  Copyright © 2017 Luciano Argento. All rights reserved.
//

#ifndef txtdata_inspector_h
#define txtdata_inspector_h

#include "pckad_sysconfig.h"
#include "pktrepr.h"
#include "preprocessor.h"

/*
 inspect_pcap:
 =============
 controlla se il pacchetto txt pkt è conforme ai
 target specificati nel file di configurazione globale (eg. tcp port).
 In caso di esito positivo inizializza la sua rappresentazione interna irep
 e la struttura dati richiesto dal modulo di pre elaborazione.
 Prevede l'allocamento di memoria per alcune variabili della struttura dati
 richiesta dal modulo di pre elaborazione.
 
 @irep: rappresentazione interna del pacchetto.
 @prpd: struttura dati richiesta dal modulo di pre elaborazione.
 @pkt: il pacchetto di rete grezzo (intestazioni + contenuto).
 @plen: lunghezza del pacchetto di rete grezzo.
 @sc: configurazione del sistema.
 
 returns: 1 se valido, 0 altrimenti.
 */
char inspect_txt (pckad_pkt *irep, prp_data *prpd, const unsigned char *pkt, unsigned int plen, pckad_sysconfig *sc);

#endif /* txtdata_inspector_h */
