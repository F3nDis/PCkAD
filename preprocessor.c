//
//  preprocessor.c
//  PCKAD-v2
//
//  Created by Luciano on 18/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <stdio.h>
#include "preprocessor.h"
#include "protocols_utility.h"
#include "netheaders.h"

#include "http_preprocessor.h"
#include "ftp_preprocessor.h"

int preprocessor_init(const char *cnfs_path)
{
    int ret = http_module_init(cnfs_path);
    if(ret == HTTP_LCNF_OUT_OF_MEM)
    {
        return PRPROC_OMEM;
    }
    else if(ret == HTTP_LCNF_FAIL)
    {
        return PRPROC_INIT_FAIL;
    }
    
    ret = ftp_module_init(cnfs_path);
    if(ret == FTP_LCNF_OUT_OF_MEM)
    {
        return PRPROC_OMEM;
    }
    else if(ret == FTP_LCNF_FAIL)
    {
        return PRPROC_INIT_FAIL;
    }
    
    return PRPROC_INIT_OK;
}


int preprocess (pckad_pkt *ppkt, prp_data *pdata)
{
    int ret = 0;
    
    if(pdata->port == HTTP_PORT)
    {
        ret = http_preprocess(ppkt, pdata->pkt_payload, pdata->len);
        if(ret == HTTP_PRPROC_MALFORMED)
        {
            return PRPROC_MALFORMED;
        }
        else if(ret == HTTP_PRPROC_OUT_OF_MEM)
        {
            return PRPROC_OMEM;
        }
        else if(ret == HTTP_PRPROC_UNINTERESTING)
        {
            return PRPROC_UNINTERESTING;
        }
    }
    else if(pdata->port == FTP_PORT)
    {
        ret = ftp_preprocess(ppkt, pdata->pkt_payload, pdata->len);
        
        if(ret == FTP_PRPROC_OUT_OF_MEM)
        {
            return PRPROC_OMEM;
        }
    }
    else
    {
        return PRPROC_UNSUPPORTED;
    }
    
    return PRPROC_OK;
}

void preprocessor_destr()
{
    http_module_destr();
    ftp_module_destr();
}

prp_data* create_prp_data()
{
    prp_data *prpdt = (prp_data*) malloc(sizeof(prp_data));
    if(prpdt == NULL)
    {
        return NULL;
    }
    prpdt->dirc = 0;
    prpdt->len = 0;
    prpdt->pkt_payload = NULL;
    prpdt->port = 0;
    
    return prpdt;
}

void free_prp_data(prp_data* prpd)
{
    if(prpd->pkt_payload != NULL)
    {
        free(prpd->pkt_payload);
    }
    
    free(prpd);
}











