//
//  preprocessor.h
//  PCKAD-v2
//
//  Created by Luciano on 27/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_preprocessor_h
#define PCKAD_v2_preprocessor_h

#include "pktrepr.h"

#define PRPROC_OMEM 3
#define PRPROC_INIT_OK 4
#define PRPROC_INIT_FAIL 5
#define PRPROC_MALFORMED 6
#define PRPROC_UNINTERESTING 7 // per pacchetti di cui non è richiesta l'elaborazione
#define PRPROC_UNSUPPORTED 8 // per protocolli non ancora supportati dal sistema
#define PRPROC_OK 9

/*Struttura contenente i dati richiesti dal modulo di pre elaborazione.*/
typedef struct prp_data
{
    unsigned char *pkt_payload;
    unsigned int len;// lunghezza del contenuto di un pacchetto
    unsigned short port;// porta dell'intestazione TCP
    char dirc;//consente di capire se la porta corrisponde alla destinazione o sorgente
}prp_data;


/*
 preprocessor_init:
 ==================
 esegue l'inizializzazione del modulo di pre elaborazione.
 
 @cnfs_path: percorso della cartella dei file di configurazione per i moduli di pre
 elaborazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int preprocessor_init(const char *cnfs_path);

/*
 preprocess:
 ===========
 elabora il pacchetto ricevuto in input e deriva la rappresentazione
 interna, oggetto di analisi nelle fasi successive.
 
 @ppkt: struttura dati contenente la rappresentazione interna del pacchetto.
 @pdata: struttura dati contenente i dati richiesti dal modulo di pre elaborazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int preprocess (pckad_pkt *ppkt, prp_data *pdata);


/*
 preprocessor_destr:
 ===================
 rilascia le risorse alloccate per i moduli di pre elaborazione.
 */
void preprocessor_destr();


prp_data* create_prp_data();


void free_prp_data(prp_data* prpd);

#endif



