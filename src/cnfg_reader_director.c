//
//  cnfg_reader_director.c
//  PCKAD-v2
//
//  Created by Luciano on 15/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <string.h>
#include "cnfg_reader_director.h"

#include "cnfg_reader.h"

#define GLOBAL_FNM "GLOBAL.xml"
#define TRAINING_FNM "TRAINING.xml"
#define DETECTION_FNM "DETECTION.xml"
#define ALERT_FNM "ALERT.xml"

/*
 pckad_sysconfig:
 ================
 legge i file di configurazione, nel formato XML, nel seguente ordine:
 globale; training; detection; alert.
 */
pckad_sysconfig* build_config(const char *root, int *stcode)
{
    char print = 1;
    pckad_sysconfig *psc = create_pckad_sysconfig();
    if(psc == NULL)
    {
        *stcode = BCNF_OUT_OF_MEM;
        return NULL;
    }
    
    
    int ret;// codice di ritorno per le funzioni di lettura
    
    unsigned long plen = strlen(root);
    unsigned long fnmlen = strlen(DETECTION_FNM);
    char full_path[plen + fnmlen + 1];
    memset(full_path, '\0', plen + fnmlen + 1);
    strncpy(full_path, root, plen);//TODO: dovrebbe essere ok plen come parametro
    
    
    if(print)
    {
        printf("INIT: lettura dei file di configurazione in corso...\n");
    }
    
    
    
    /* LETTURA DEL FILE DI CONFIGURAZIONE GLOBALE */
    
    
    strncat (full_path, GLOBAL_FNM, strlen(GLOBAL_FNM));
    
    
    if(print)
    {
        printf("\tfile -> %s\n", full_path);
    }
    
    
    ret = stream_gcfile(full_path, psc->gc);
    if(ret == CR_UNABLE_OPEN)
    {
        fprintf(stderr, "\n%s : failed to open\n", full_path);
        free_pckad_sysconfig(psc);
        *stcode = BCNF_GC_FAIL;
        return NULL;
    }
    else if(ret == CR_UNABLE_PARSE)
    {
        
        fprintf(stderr, "\n%s : failed to parse\n", full_path);
        free_pckad_sysconfig(psc);
        *stcode = BCNF_GC_FAIL;
        return NULL;
    }
    
    
    
    /* LETTURA DEL FILE DI CONFIGURAZIONE TRAINING */
    
    
    
    memset(full_path, '\0', strlen(full_path));
    strncpy(full_path, root, plen);
    
    strncat (full_path, TRAINING_FNM, strlen(TRAINING_FNM));
    printf("\tfile -> %s\n", full_path);
    ret = stream_tcfile(full_path, psc->tc);
    if(ret == CR_UNABLE_OPEN)
    {
        fprintf(stderr, "\n%s : failed to open\n", full_path);
        free_pckad_sysconfig(psc);
        *stcode = BCNF_TC_FAIL;
        return NULL;
    }
    else if(ret == CR_UNABLE_PARSE)
    {
        
        fprintf(stderr, "\n%s : failed to parse\n", full_path);
        free_pckad_sysconfig(psc);
        *stcode = BCNF_TC_FAIL;
        return NULL;
    }
    
    
    
    /* LETTURA DEL FILE DI CONFIGURAZIONE DETECTION */
    
    
    
    memset(full_path, '\0', strlen(full_path));
    strncpy(full_path, root, plen);
    strncat (full_path, DETECTION_FNM, strlen(DETECTION_FNM));
    
    
    if(print)
    {
        printf("\tfile -> %s\n", full_path);
    }
    
    
    ret = stream_dcfile(full_path, psc->dc);
    if(ret == CR_UNABLE_OPEN)
    {
        fprintf(stderr, "\n%s : failed to open\n", full_path);
        free_pckad_sysconfig(psc);
        *stcode = BCNF_DC_FAIL;
        return NULL;
    }
    else if(ret == CR_UNABLE_PARSE)
    {
        
        fprintf(stderr, "\n%s : failed to parse\n", full_path);
        free_pckad_sysconfig(psc);
        *stcode = BCNF_DC_FAIL;
        return NULL;
    }
    
    
    
    if(print)
    {
        printf("INFO: lettura dei file di configurazione terminata con successo.\n");
    }
    
    
    *stcode = BCNF_OK;
    return psc;
}












