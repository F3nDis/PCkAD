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

typedef struct protocol_data
{
    float threshold;
    unsigned short port;
    char prid;// application protocol ID
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
    char storemodels; // flag used to enable the storage of the models. 1: enabled, 0: disabled.
}training_cnfg;

typedef struct detection_cnfg
{
    char mechanism;// The determines the mechanism used for computing the anomaly score
    char enable_supmodels; // flag used to enable approximate models
    char enable_alerts; // flag used to enable the alarm module (not yet supported)
}detection_cnfg;

typedef struct pckad_sysconfig
{
    global_cnfg *gc;
    training_cnfg *tc;
    detection_cnfg *dc;
    //TODO: next: alarm module configuration
}pckad_sysconfig;

/*
 Creates an instance of the pckad_sysconfig structure.
 It returns NULL if there is no sufficient memory.
 
 returns: an instance of the pckad_sysconfig structure. NULL if there is not sufficient memory.
 
 */
pckad_sysconfig* create_pckad_sysconfig();

/*
 Frees the memory reserved for psc. If psc == NULL, it does nothing.
 
 @psc: An instance of the pckad_sysconfig structure.
 
 */
void free_pckad_sysconfig(pckad_sysconfig *psc);

/*
 Checks whether the system configurations is consistent, i.e. all the configuration parameters
 have valid values.
 
 @psc: An instance of the pckad_sysconfig structure.
 
 returns: code representing the operation result.
 */
int check_consistency(pckad_sysconfig *scnfg);

#endif
