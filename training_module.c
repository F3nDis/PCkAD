//
//  training_module.c
//  PCKAD-v2
//
//  Created by Luciano on 03/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <math.h>

#include "training_module.h"
#include "inspector_factory.h"
#include "pktrepr.h"
#include "preprocessor.h"
#include "pckad_model.h"
#include "build_factory.h"

static void compute_stats(pckad_model *mdl, unsigned short n, unsigned lck, char strategy);

int train_on_pkt(const unsigned char *pkt, unsigned int plen, glist *models, pckad_sysconfig *scnfg)
{
    int retcode = PRPROC_OK;
    //TODO: valutare l'uso di una cache per le due strutture dati sottostanti
    prp_data *prpdt = create_prp_data();
    pckad_pkt *ppkt = pckad_pkt_dinit();
    inspect_rawd inspect;
    build_model build;
    pckad_model *mdl = NULL;
    pkt_data *portion = NULL;
    char found = 0;// usata per la ricerca dei modelli
    char prp_fail = 0;
    
    // STEP: data inspection
    inspect = get_inspector(scnfg->gc->format);

    retcode =  inspect(ppkt, prpdt, pkt, plen, scnfg);
    switch (retcode)
    {
        case INSPECTION_INVALID:
            free_pckad_pkt(ppkt);
            free_prp_data(prpdt);
            return retcode;
            
        case INSPECTION_OMEM:
            free_pckad_pkt(ppkt);
            free_prp_data(prpdt);
            return retcode;
            
        default:
            break;
    }
    
    // STEP: pre processing
    retcode = preprocess(ppkt, prpdt);
    
    switch (retcode)
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
            portion = (pkt_data*) get_data(ppkt->portions, p);
            
            
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
                mdl = create_model(ppkt, p);
                add_last(models, mdl);
            }
            else
            {
                found = 0;
            }
            
            // STEP: training
            portion = (pkt_data*) get_data(ppkt->portions, p);
            build = get_build_model(scnfg->gc->strategy);
            build(mdl, portion, scnfg->gc->n, scnfg->gc->lck);
        }
    }
    
    free_pckad_pkt(ppkt);
    free_prp_data(prpdt);
    return retcode;
}

void end_training (glist *models, pckad_sysconfig *scnfg)
{
    pckad_model *mdl = NULL;
    for ( unsigned int i = 0; i < models->len; i++)
    {
        mdl = (pckad_model*) get_data(models, i);
        compute_stats(mdl, scnfg->gc->n, scnfg->gc->lck, scnfg->gc->strategy);
    }
}

static void compute_stats(pckad_model *mdl, unsigned short n, unsigned lck, char strategy)
{
    float ex2 = 0;
    float e2x = 0;
    
    u_short max_nrcks = ceilf(mdl->max_pkt_len/(float)lck);
    
    
    ngram_data *ngd = NULL;
    ngram_stats *ngs = NULL;
    int *pkt_hmlen = (int*) malloc(sizeof(int));
    int *retcode = (int*) malloc(sizeof(int));
    ngram_obj **pkt_keys = getall_ngrams(mdl->core, pkt_hmlen);
    ngram_obj *key = NULL;
    
    for (int i = 0; i < *pkt_hmlen; i++)//  intero sugli ngram del modello
    {
        key = pkt_keys[i];
        ngd = get_ngrdata(mdl->core, key->ngram, n, retcode);//TODO: controllare codice di ritorno
        
        if(strategy != LS_STR)
        {
            ngs = get_statsofdunit(ngd);
            ex2 = ngs->std_dev/ngs->obsv_pkts;
            e2x = pow((ngs->avg_occ/ngs->obsv_pkts), 2);
            if(ex2 - e2x >= 0)
            {
                ngs->std_dev = sqrt(ex2 - e2x);
            }
            ngs->avg_occ = ngs->avg_occ/ngs->obsv_pkts;
        }
        if(strategy != GS_STR)
        {
            for ( unsigned short j = 0; j < max_nrcks; j++)//TODO: ottimizzazione?
            {
                ngs = get_statsofck(ngd, j);
                if(ngs != NULL)
                {
                    ex2 = ngs->std_dev/ngs->obsv_pkts;
                    e2x = pow((ngs->avg_occ/ngs->obsv_pkts), 2);
                    if(ex2 - e2x >= 0)
                    {
                        ngs->std_dev = sqrt(ex2 - e2x);
                    }
                    ngs->avg_occ = ngs->avg_occ/ngs->obsv_pkts;
                }
            }
        }
        free_chunks(ngd);// ora che ho concluso la fase di training posso rilasciare le celle non utilizzate
    }
    
    free(pkt_hmlen);
    free(retcode);
    free(pkt_keys);
}













