//
//  http_preprocessor.h
//  PCKAD-v2
//
//  Created by Luciano on 18/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef http_preprocessor_h
#define http_preprocessor_h

#include "pktrepr.h"

#define HTTP_LCNF_OK 1
#define HTTP_LCNF_FAIL 2
#define HTTP_LCNF_OUT_OF_MEM 0

#define HTTP_PRPROC_OK 1
#define HTTP_PRPROC_UNINTERESTING 2
#define HTTP_PRPROC_MALFORMED 3
#define HTTP_PRPROC_OUT_OF_MEM 0

/*
 module_init:
 ==================
 esegue l'inizializzazione del modulo di pre elaborazione del protocollo HTTP.
 
 @cnfs_path: percorso della cartella contenente i file di configurazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int http_module_init(const char *cnfs_path);

/*
 preprocess:
 ===========
 elabora il pacchetto HTTP ricevuto in input e deriva la rappresentazione
 interna, oggetto di analisi nelle fasi successive.
 
 @ppkt: struttura dati contenente la rappresentazione interna del pacchetto HTTP.
 @pdata: struttura dati contenente i dati richiesti dal modulo di pre elaborazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int http_preprocess (pckad_pkt *ppkt, unsigned char *payload, unsigned int plen);

/*
 http_module_destr:
 ==================
 rilascia le risorse allocate per il modulo di pre elaborazione del protocollo HTTP.
 */
void http_module_destr();

#endif /* http_preprocessor_h */
