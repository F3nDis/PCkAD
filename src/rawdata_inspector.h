//
//  rawdata_inspector.h
//  PCKAD-v2
//
//  Created by Luciano on 29/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef rawdata_inspector_h
#define rawdata_inspector_h

#include "pckad_sysconfig.h"
#include "pktrepr.h"
#include "preprocessor.h"

/*
 inspect_pcap:
 =============
 controlla se il pacchetto pcap pkt è conforme ai
 target specificati nel file di configurazione globale.
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
char inspect_pcap (pckad_pkt *irep, prp_data *prpd, const unsigned char *pkt, unsigned int plen, pckad_sysconfig *sc);

#endif /* rawdata_inspector_h */
