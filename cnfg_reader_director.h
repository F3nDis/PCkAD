//
//  cnfg_reader_director.h
//  PCKAD-v2
//
//  Created by Luciano on 15/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef cnfg_reader_director_h
#define cnfg_reader_director_h

#include <stdio.h>
#include "pckad_sysconfig.h"

#define BCNF_OK 1
#define BCNF_GC_FAIL 2
#define BCNF_TC_FAIL 3
#define BCNF_DC_FAIL 4
#define BCNF_AC_FAIL 5
#define BCNF_OUT_OF_MEM 0

/*
 pckad_sysconfig:
 ================
 carica le configurazione del sistema nella struttura pckad_sysconfig.
 
 @root: percorso della cartella in cui si trovano i file di configurazione.
 @stcode: lo status code; indica l'esito dell'operazione.
 
 returns: la struttura dati contenente le configurazioni
 */
pckad_sysconfig* build_config(const char *root, int *stcode);

#endif /* cnfg_reader_director_h */
