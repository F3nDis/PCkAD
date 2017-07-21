//
//  pckad_if.h
//  PCKAD-v2
//
//  Created by Luciano on 25/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef pckad_if_h
#define pckad_if_h

#include "glist.h"
#include "pckad_sysconfig.h"
#include "classification_module.h"

#define CLASS_OK 1
#define CLASS_MALFORMED 2
#define CLASS_UNINTERESTING 3
#define CLASS_UNSUPPORTED 4
#define CLASS_NOTPROCESSED 5
#define CLASS_OMEM 0

#define TRAIN_OK 1
#define TRAIN_MALFORMED 2
#define TRAIN_UNINTERESTING 3
#define TRAIN_UNSUPPORTED 4
#define TRAIN_OMEM 0


typedef struct PCkAD
{
    pckad_sysconfig *scnfg;
    glist *models;
    glist *aprx_models;
    int *retcode;// memorizza il codice di ritorno di un'operazione
    result_description *rd;
}PCkAD;

PCkAD *createPCkAD();

void init_PCkAD(PCkAD *pckad, const char *config_path);

void load_pckad_models(PCkAD *pckad, const char *models_path);

void save_pckad_models(PCkAD *pckad, const char *models_path);

void free_PCkAD(PCkAD *pckad);

void start_training_phase(PCkAD *pckad);

void training_step(const unsigned char *data, unsigned int len, PCkAD *pckad);

void end_training_phase(PCkAD *pckad);

float classify_data(const unsigned char *data, unsigned int len, PCkAD *pckad);

#endif /* pckad_if_h */
