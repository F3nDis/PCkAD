//
//  pckad_sysconfig.h
//  PCKAD-v2
//
//  Created by Luciano on 03/10/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_pckad_sysconfig_h
#define PCKAD_v2_pckad_sysconfig_h

#define CONSISTENCY_N 0
#define CONSISTENCY_LCK 5
#define CONSISTENCY_KP 2
#define CONSISTENCY_MECHANISM 3
#define CONSISTENCY_FORMAT 4
#define CONSISTENCY_OK 1

#define PCAP_FORMAT 0
#define TXT_FORMAT 1

#define GS_STR 0
#define LS_STR 1
#define TWOLS_STR 2

#define DIST_PERMATCH_MECHANISM 0
#define DIST_PARMATCH_MECHANISM 1

#define NO_CLUSTERING 0
#define USE_CLUSTERING 1

typedef struct protocol_data
{
    float threshold;
    unsigned short port;
    char prid;// id del protocollo
    char dirc;
}protocol_data;

typedef struct global_cnfg
{
    protocol_data *protocols;
    unsigned short lck;
    unsigned char nr_protocols;
    unsigned char n;
    char strategy;
    char format;
}global_cnfg;

typedef struct training_cnfg
{
    float kp;// percentuale usata per determinare il valore del parametro k, per il k clustering
    char storemodels; // flag per il salvataggio dei modelli
    char clustering;// utilizzata per stabilire se utilizzare il clustering
}training_cnfg;

typedef struct detection_cnfg
{
    char mechanism;// determina il meccanismo utilizzato alla base del processo di classificazione
    char enable_supmodels; // flag per abilitare l'utilizzo di modelli di supporto
    char enable_alerts; // flag per abilitare le notifiche di allarme
}detection_cnfg;

typedef struct pckad_sysconfig
{
    global_cnfg *gc;
    training_cnfg *tc;
    detection_cnfg *dc;
    //TODO: manca la configurazione per il modulo di allarme
}pckad_sysconfig;

/*
 create_pckad_sysconfig:
 =======================
 crea la struttura dati pckad_sysconfig.
 Restituisce NULL nel caso non fosse disponibile sufficiente memoria.
 
 returns: un'istanza della struttura dati pckad_sysconfig. NULL con memoria insufficiente.
 
 */
pckad_sysconfig* create_pckad_sysconfig();

/*
 free_pckad_sysconfig:
 =======================
 rilascia la memoria allocata per la struttura dati pckad_sysconfig.
 Se psc == NULL, non effettua alcuna operazione.
 
 @psc: un'istanza della struttura dati pckad_sysconfig.
 
 */
void free_pckad_sysconfig(pckad_sysconfig *psc);

int check_consistency(pckad_sysconfig *scnfg);

#endif
