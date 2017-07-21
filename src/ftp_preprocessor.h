//
//  ftp_preprocessor.h
//  PCKAD-v2
//
//  Created by Luciano on 18/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef ftp_preprocessor_h
#define ftp_preprocessor_h

#include "pktrepr.h"

#define FTP_LCNF_OK 1
#define FTP_LCNF_FAIL 2
#define FTP_LCNF_OUT_OF_MEM 0

#define FTP_PRPROC_OK 0
#define FTP_PRPROC_OUT_OF_MEM 1

/*
 module_init:
 ==================
 esegue l'inizializzazione del modulo di pre elaborazione del protocollo FTP.
 
 @cnfs_path: percorso della cartella contenente i file di configurazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int ftp_module_init(const char *cnfs_path);

/*
 preprocess:
 ===========
 elabora il pacchetto FTP ricevuto in input e deriva la rappresentazione
 interna, oggetto di analisi nelle fasi successive.
 
 @ppkt: struttura dati contenente la rappresentazione interna del pacchetto FTP.
 @pdata: struttura dati contenente i dati richiesti dal modulo di pre elaborazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int ftp_preprocess (pckad_pkt *ppkt, unsigned char *payload, unsigned int plen);

/*
 http_module_destr:
 ==================
 rilascia le risorse allocate per il modulo di pre elaborazione del protocollo FTP.
 */
void ftp_module_destr();

#endif /* ftp_preprocessor_h */
