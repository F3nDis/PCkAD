//
//  pktrepr.h
//  PCKAD-v2
//
//  Created by Luciano on 27/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PktReprLib_pktrepr_h
#define PktReprLib_pktrepr_h

#include "glist.h"

#define SRC_PORT 0
#define DEST_PORT 1

/*
 Contiene i dati relativi ad una porzione del payload di un
 pacchetto.
 */
typedef struct pkt_data
{
    unsigned char *data;// i dati della porzione
    unsigned int len;// numero di caratteri in data, fatta eccezione per il carattere nullo
    char type;// determina il tipo di porzione del payload
}pkt_data;

/*
 Rappresentazione interna di un pacchetto di rete in PCkAD.
 Si compone di informazioni di controllo e dati utente o payload.
 I dati utente possono essere rappresentati da un numero di porzioni >= 1.
 Il numero di porzioni non deve essere necessariamente specificato in input.
 */
typedef struct pckad_pkt
{
    // dati utente
    glist *portions;// lista contenente le porzioni del pacchetto (pkt_data)
    
    // informazioni di controllo
    unsigned short tcp_port;// porta dell'intestazione TCP
    char dirc;// direzione del pacchetto: per capire se la porta corrisponde alla destinazione o sorgente
}pckad_pkt;

/*
 Inizializza la struttura dati pckad_pkt.
 Restituisce NULL in caso di memoria insufficiente.
 */
pckad_pkt* pckad_pkt_dinit();

/*
 Inizializza la struttura dati pckad_pkt, specificando
 in input il numero di porzioni di cui il pacchetto si compone.
 Restituisce NULL in caso di memoria insufficiente.
 */
pckad_pkt* pckad_pkt_sinit( unsigned int nr);

/*
 Rilascia alla memoria per la struttura dati pckad_pkt.
 */
void free_pckad_pkt(pckad_pkt* ppkt);



#endif
