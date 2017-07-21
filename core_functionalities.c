//
//  core_functionalities.c
//  PCKAD-v2
//
//  Created by Luciano on 08/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <math.h>

#include "core_functionalities.h"
#include "pktrepr.h"

char check_profile (pckad_model *mdl, pckad_pkt *ppkt, unsigned int prtnidx, unsigned short lck)
{
    pkt_data *pd = NULL;
    pd = (pkt_data*) get_data(ppkt->portions, prtnidx);
    if (mdl->port != ppkt->tcp_port || mdl->spk_id != pd->type || mdl->is_dest != ppkt->dirc)
        return 0;
    
    unsigned short mchunks = ceilf(mdl->max_pkt_len/(float )lck);
    unsigned short pchunks = ceilf(pd->len/(float )lck);
    
    //verifico se c'è un riscontro per quanto riguarda la lunghezza
    if(mchunks != pchunks)
        return 0;
    
    //TODO: dovrei aggiornare le lunghezze minime e massime
    if(pd->len < mdl->min_pkt_len)
    {
        mdl->min_pkt_len = pd->len;
    }
    if(pd->len > mdl->max_pkt_len)
    {
        mdl->max_pkt_len = pd->len;
    }
    
    return 1;
}

unsigned int lookfor_replacement (glist *mdls, pckad_pkt *ppkt, unsigned int prtnidx, unsigned short lck)
{
    pkt_data *pd = NULL;
    pd = (pkt_data*) get_data(ppkt->portions, prtnidx);
    
    unsigned int smidx = 0, gridx = 0;
    pckad_model *curr_trprof = NULL, *sm_simil_prof = NULL, *gr_simil_prof = NULL;
    u_short sm_chunk_diff = 65535, gr_chunk_diff = 65535;// valore massimo consentito per questo tipo
    
    u_short mdlcks = 0, pdcks = ceilf(pd->len/(float)lck);
    
    for (u_short i = 0; i < mdls->len; i++)
    {
        curr_trprof = (pckad_model*) get_data(mdls, i);
        
        // verifica preliminare della compatibilità tra il profilo e il pacchetto
        if (curr_trprof->port != ppkt->tcp_port || curr_trprof->spk_id != pd->type || curr_trprof->is_dest != ppkt->dirc)
            continue;
        
        mdlcks = ceilf(curr_trprof->max_pkt_len/(float)lck);
        
        // calcolo la differenza e aggiorno il profilo più simile, se necessario
        if(mdlcks > pdcks)
        {
            // sto considerando il caso in cui il profilo ha un numero maggiore di chunk
            if(gr_chunk_diff > (mdlcks - pdcks))
            {
                gr_chunk_diff = mdlcks - pdcks;
                gr_simil_prof = curr_trprof;
                gridx = i;
            }
        }
        else
        {
            // sto considerando il caso in cui il profilo ha un numero minore di chunk
            if(sm_chunk_diff > (pdcks - mdlcks))
            {
                sm_chunk_diff = pdcks - mdlcks;
                sm_simil_prof = curr_trprof;
                smidx = i;
            }
        }
    }
    
    //TODO: per ora si limita a restituire il profilo a cui corrisponde la distanza minima
    //TODO: si trova sempre?
    return (gr_chunk_diff < sm_chunk_diff)? gridx : smidx;
}










