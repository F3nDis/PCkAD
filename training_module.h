//
//  training_module.h
//  PCKAD-v2
//
//  Created by Luciano on 03/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef training_module_h
#define training_module_h

#include "glist.h"
#include "pckad_sysconfig.h"

/*
 train_on_pkt:
 =============
 esegue una costruzione incrementale dei modelli models di PCkAD,
 a partire dal pacchetto pkt, di cui viene specificata la lunghezza
 plen. Restituisce un codice numerico rappresentante l'esito
 dell'operazione.
 
 @pkt: pacchetto di rete (intestazioni + contenuto)
 @plen: lunghezza del pacchetto di rete.
 @models: lista contenente i modelli correntemente in costruzione.
 @scnfg: configurazione del sistema.
 
 returns: codice numerico indicante l'esito dell'operazione.
 */
int train_on_pkt(const unsigned char *pkt, unsigned int plen, glist *models, pckad_sysconfig *scnfg);

void end_training (glist *models, pckad_sysconfig *scnfg);

#endif /* training_module_h */
