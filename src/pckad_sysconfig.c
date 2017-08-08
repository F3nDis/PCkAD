//
//  pckad_sysconfig.c
//  PCKAD-v2
//
//  Created by Luciano on 15/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <stdlib.h>

#include "pckad_sysconfig.h"

static void free_detection_cnfg(detection_cnfg* dc)
{
    if(dc != NULL)
    {
        free(dc);
    }
}

static detection_cnfg* create_detection_cnfg()
{
    detection_cnfg *dc = (detection_cnfg*) malloc(sizeof(detection_cnfg));
    if(dc != NULL)
    {
        dc->enable_alerts = 0;
        dc->enable_supmodels = 0;
        dc->mechanism = 0;
    }
    
    return dc;
}

static void free_training_cnfg(training_cnfg* tc)
{
    if(tc != NULL)
    {
        free(tc);
    }
}

static training_cnfg* create_training_cnfg()
{
    training_cnfg *tc = (training_cnfg*) malloc(sizeof(training_cnfg));
    if(tc != NULL)
    {
        tc->storemodels = 0;
    }
    return tc;
}

static void free_global_cnfg(global_cnfg* gc)
{
    if(gc != NULL)
    {
        if(gc->protocols != NULL)
            free(gc->protocols);
        free(gc);
    }
}

static global_cnfg* create_global_cnfg()
{
    global_cnfg *gc = (global_cnfg*) malloc(sizeof(global_cnfg));
    if(gc != NULL)
    {
        gc->lck = 0;
        gc->n = 0;
        gc->nr_protocols = 0;
        gc->strategy = 0;
        gc->format = 0;
        gc->protocols = NULL;
    }
    
    return gc;
}

pckad_sysconfig* create_pckad_sysconfig()
{
    pckad_sysconfig *psc = (pckad_sysconfig*) malloc(sizeof(pckad_sysconfig));
    
    if(psc != NULL)
    {
        psc->dc = create_detection_cnfg();
        psc->tc = create_training_cnfg();
        psc->gc = create_global_cnfg();
        
        if(psc->dc == NULL || psc->tc == NULL || psc->gc == NULL)
        {
            free_global_cnfg(psc->gc);
            free_training_cnfg(psc->tc);
            free_detection_cnfg(psc->dc);
            free(psc);
        }
    }
    
    return psc;
}

void free_pckad_sysconfig(pckad_sysconfig *psc)
{
    if(psc != NULL)
    {
        free_global_cnfg(psc->gc);
        free_training_cnfg(psc->tc);
        free_detection_cnfg(psc->dc);
        free(psc);
    }
}

int check_consistency(pckad_sysconfig *scnfg)
{
    if(scnfg->gc->n <= 0)
    {
        return CONSISTENCY_N;
    }
    if(scnfg->gc->lck <= scnfg->gc->n)
    {
        return CONSISTENCY_LCK;
    }
    if(scnfg->dc->mechanism != DIST_PERMATCH_MECHANISM && scnfg->dc->mechanism != DIST_PARMATCH_MECHANISM)
    {
        return CONSISTENCY_MECHANISM;
    }
    if(scnfg->gc->format != PCAP_FORMAT && scnfg->gc->format != TXT_FORMAT)
    {
        return CONSISTENCY_FORMAT;
    }
    return CONSISTENCY_OK;
}


























