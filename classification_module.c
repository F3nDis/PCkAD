//
//  classification_module.c
//  PCKAD-v2
//
//  Created by Luciano on 03/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include "classification_module.h"
#include "pktrepr.h"
#include "pckad_model.h"
#include "preprocessor.h"
#include "inspector_factory.h"
#include "inspector_if.h"
#include "core_functionalities.h"
#include "cmptas_factory.h"

float analyse_pkt (const unsigned char *pkt, unsigned int plen, glist *models, pckad_sysconfig *scnfg, int *rescode, result_description *rd)
{
    float as = -1;
    *rescode = PRPROC_OK;
    unsigned int replidx = 0;// indice utilizzato per modelli simili
    //TODO: valutare l'uso di una cache per le due strutture dati sottostanti
    prp_data *prpdt = create_prp_data();
    pckad_pkt *ppkt = pckad_pkt_dinit();
    inspect_rawd inspect;
    compute_as getas;// puntatore alla funzione che calcola l'indice di anomalia
    pckad_model *mdl = NULL;
    pkt_data *portion = NULL;
    char found = 0;// usata per la ricerca dei modelli
    char prp_fail = 0;
    
    // STEP: data inspection
    inspect = get_inspector(scnfg->gc->format);
    
    *rescode =  inspect(ppkt, prpdt, pkt, plen, scnfg);
    switch (*rescode)
    {
        case INSPECTION_INVALID:
            free_pckad_pkt(ppkt);
            free_prp_data(prpdt);
            return as;
            
        case INSPECTION_OMEM:
            free_pckad_pkt(ppkt);
            free_prp_data(prpdt);
            return as;
            
        default:
            break;
    }
    
    // STEP: pre processing
    *rescode = preprocess(ppkt, prpdt);
    
    switch (*rescode)
    {
        case PRPROC_MALFORMED:
            prp_fail = 1;
            break;
            
        case PRPROC_UNINTERESTING:
            
            prp_fail = 1;
            break;
            
        case PRPROC_UNSUPPORTED:
            prp_fail = 1;
            break;
            
        case PRPROC_OMEM:
            prp_fail = 1;
            break;
            
        default:
            break;
    }
    
    if(!prp_fail)
    {
        // MACRO STEP: portions training
        for ( unsigned  int p = 0; p < ppkt->portions->len; p++)
        {
            // STEP: check profile
            for ( unsigned int i = 0; i < models->len; i++)
            {
                mdl = (pckad_model*) get_data(models, i);
                if(check_profile(mdl, ppkt, p, scnfg->gc->lck))
                {
                    found = 1;
                    break;
                }
            }
            
            if(!found)
            {
                if(scnfg->dc->enable_supmodels)
                {
                    replidx = lookfor_replacement(models, ppkt, p, scnfg->gc->lck);
                    mdl = (pckad_model*) get_data(models, replidx);
                }
                else
                {
                    *rescode = CLMODEL_MISSING;
                    continue;//TODO: gestione errata in presenza di un numero di porzioni superiore ad uno
                }
            }
            else
            {
                found = 0;
            }
            
            // STEP: classification
            portion = (pkt_data*) get_data(ppkt->portions, p);
            
            
            rd->content = ( unsigned char*) malloc((portion->len+1) * sizeof( unsigned char));
            for ( unsigned int i = 0; i < portion->len; i++)
            {
                rd->content [ i ] = portion->data [ i ];
            }
            rd->len = portion->len;
            
            
            getas = get_compute_as(scnfg->gc->strategy, scnfg->dc->mechanism);
            as = getas(mdl, portion, scnfg->gc->n, scnfg->gc->lck, scnfg->dc->mechanism);
            
        }
        
        rd->dir = ppkt->dirc;
        rd->port = ppkt->tcp_port;
    }
    
    free_pckad_pkt(ppkt);
    free_prp_data(prpdt);
    return as;
}

result_description* create_result_description()
{
    result_description *rd = ( result_description*) malloc(sizeof( result_description));
    
    if(rd != NULL)
    {
        rd->content = NULL;
        rd->len = 0;
        rd->dir = 0;
        rd->port = 0;
    }
    return rd;
}

void free_result_description(result_description *rd)
{
    if(rd->content != NULL)
    {
        free(rd->content);
    }
    free(rd);
}


















