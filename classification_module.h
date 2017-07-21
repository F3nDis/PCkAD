//
//  classification_module.h
//  PCKAD-v2
//
//  Created by Luciano on 03/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef classification_module_h
#define classification_module_h

#include "glist.h"
#include "pckad_sysconfig.h"

#define CLMODEL_MISSING 10

typedef struct result_description
{
    unsigned char *content;
    unsigned int len;
    unsigned short port;
    char dir;
}result_description;

/*
 analyse_pkt:
 ============
 esegue l'operazione di classificazione del pacchetto pkt, di cui viene
 specificata la lunghezza plen, basandosi sui modelli models.
 Restituisce un codice numerico rescode per indicare l'esito
 dell'operazione e un numero decimale rappresentante l'indice di anomalia
 del pacchetto.
 
 @pkt: pacchetto di rete (intestazioni + contenuto).
 @plen: lunghezza del pacchetto di rete.
 @models: array contenente i modelli correntemente in costruzione.
 @scnfg: configurazione del sistema.
 @rescode: codice numerico indicante l'esito dell'operazione.
 
 returns: indice di anomalia del pacchetto.
 */
float analyse_pkt (const unsigned char *pkt, unsigned int plen, glist *models, pckad_sysconfig *scnfg, int *rescode, result_description *rd);

result_description* create_result_description();

void free_result_description(result_description *rd);
#endif /* classification_module_h */
