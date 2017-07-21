//
//  CnfgReader.h
//  PCKAD-v2
//
//  Created by Luciano on 03/10/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_CnfgReader_h
#define PCKAD_v2_CnfgReader_h

#include <stdio.h>
#include <libxml/xmlreader.h>

#include "pckad_sysconfig.h"

#define CR_OK 0
#define CR_UNABLE_PARSE 1
#define CR_UNABLE_OPEN 2

/*
 stream_gcfile
 -------------
 Legge il contenuto di un file XML, di cui viene specificato il percorso
 assoluto in filename e inizializza i campi della struttura global_cnfg.
 La struttura global_cnfg NON deve avere il campo protocols inizializzato.
 
 @filename: il percorso assoluto del file XML contenente la configurazione globale
 @gcnfg: riferimento alla struttura dati per la configurazione globale
 
 returns: restituisce un intero rappresentante l'esito dell'operazione.
 */
int stream_gcfile(const char *filename, global_cnfg *gcnfg);

/*
 stream_tcfile
 -------------
 Legge il contenuto di un file XML, di cui viene specificato il percorso
 assoluto in filename e inizializza i campi della struttura training_cnfg.
 
 @filename: il percorso assoluto del file XML contenente la configurazione di training
 @gcnfg: riferimento alla struttura dati per la configurazione di training
 
 returns: restituisce un intero rappresentante l'esito dell'operazione.
 */
int stream_tcfile(const char *filename, training_cnfg *tcnfg);

/*
 stream_dcfile
 -------------
 Legge il contenuto di un file XML, di cui viene specificato il percorso
 assoluto in filename e inizializza i campi della struttura detection_cnfg.
 
 @filename: il percorso assoluto del file XML contenente la configurazione di detection
 @gcnfg: riferimento alla struttura dati per la configurazione di detection
 
 returns: restituisce un intero rappresentante l'esito dell'operazione.
 */
int stream_dcfile(const char *filename, detection_cnfg *dcnfg);

#endif
