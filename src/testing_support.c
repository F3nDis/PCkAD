//
//  testing_support.c
//  PCKAD-v2
//
//  Created by Luciano on 12/12/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "pckad_sysconfig.h"
#include "testing_support.h"

void print_model (pckad_model *mdl, char strategy, unsigned char n, unsigned short lck)
{
    unsigned short chunks = ceilf (mdl->max_pkt_len/(float)lck);
    int *klen = (int*) malloc(sizeof(int));
    int *retcode = (int*) malloc(sizeof(int));
    ngram_obj **keys = getall_ngrams(mdl->core, klen);
    ngram_obj *key = NULL;
    ngram_data *ngd = NULL;
    ngram_stats *ngs = NULL;
    
    printf("DEBUG: numero di ngram nel modello = %u\n", mdl->nr_of_seqs);
    
    printf("DEBUG: Recupero degli ngram...\n");
    for (int i = 0; i < *klen; i++)
    {
        key = keys[i];
        
        printf("ngram == ");
        for ( unsigned char j = 0; j < n; j++)
        {
            printf("%c", key->ngram [j]);
        }
        printf("\n");
        
        ngd = get_ngrdata(mdl->core, key->ngram, n, retcode);
        if(strategy != LS_STR)
        {
            ngs = get_statsofdunit(ngd);
            printf("\tavg_occ = %f\n", ngs->avg_occ);
            printf("\tstd_dev = %f\n", ngs->std_dev);
            printf("\tobsv_pkts = %u\n", ngs->obsv_pkts);
            printf("\tsupp = %u\n", ngs->supp);
        }
        if(strategy != GS_STR)
        {
            for ( unsigned short j = 0; j < chunks; j++)
            {
                ngs = get_statsofck(ngd, j);
                if(ngs != NULL)
                {
                    printf("\tCK = %d\n", j);
                    printf("\t\tavg_occ = %f\n", ngs->avg_occ);
                    printf("\t\tstd_dev = %f\n", ngs->std_dev);
                    printf("\t\tobsv_pkts = %u\n", ngs->obsv_pkts);
                    printf("\t\tsupp = %u\n", ngs->supp);
                }
            }
        }
    }
    printf("\n");
    
    free(retcode);
    free(klen);
}
