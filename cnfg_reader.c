//
//  CnfgReader.c
//  PCKAD-v2
//
//  Created by Luciano on 03/10/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#include "cnfg_reader.h"

#include <libxml/xmlreader.h>
#include <string.h>

#include "glist.h"

/*Definizioni utili per la variabile discriminante dei file di configurazione*/
#define GLOBAL 0
#define TRAINING 1
#define DETECTION 2

/*Definizione dei nomi dei tag del file di configurazione globale*/
#define N "n"
#define LCK "lck"
#define STRATEGY "strategy"
#define FORMAT "format"
#define PROTOCOL "protocol"
#define PORT "port"
#define THRESHOLD "threshold"

/*Definizione dei nomi dei tag del file di configurazione per il training*/
#define STOREMODELS "storemodels"
#define CLUSTERING "clustering"
#define KP "kp"

/*Definizione dei nomi dei tag del file di configurazione per la detection*/
#define SUPMODELS "supmodels"
#define ALERTS "alerts"
#define MECHANISM "mechanism"

/*Definizione di stringhe per il parsing (legate all'implementazione della libreria usata)*/
#define TEXT "#text"
#define CONTENT_NODE 3

/*
 tcnf_is_tag_valid
 -----------------
 */
static int tcnf_is_tag_valid(const xmlChar *tag)
{
    size_t n = xmlStrlen(tag);
    
    if(strlen(STOREMODELS) == n && memcmp(tag, ((unsigned char*)STOREMODELS), n) == 0)
        return 1;
    
    
    
    if(strlen(CLUSTERING) == n && memcmp(tag, ((unsigned char*)CLUSTERING), n) == 0)
        return 1;
    
    
    
    if(strlen(KP) == n && memcmp(tag, ((unsigned char*)KP), n) == 0)
        return 1;
    
    return 0;
}

/*
 tcnf_set_global_cnfg
 --------------------
 */
static void tcnf_set_global_cnfg(const xmlChar *tag, const xmlChar *value, training_cnfg *tcnfg)
{
    size_t n = xmlStrlen(tag);
    
    if(strlen(STOREMODELS) == n && memcmp(tag, ((unsigned char*)STOREMODELS), n) == 0)
        tcnfg->storemodels = (char) strtol(( char *)value, NULL, 10);
    
    
    
    if(strlen(CLUSTERING) == n && memcmp(tag, ((unsigned char*)CLUSTERING), n) == 0)
    {
        tcnfg->clustering = (char) strtol(( char *)value, NULL, 10);
        return;
    }
    
    
    
    if(strlen(KP) == n && memcmp(tag, ((unsigned char*)KP), n) == 0)
    {
        tcnfg->kp = (float ) strtof(( char *)value, NULL);
        return;
    }
}

/*
 dcnf_is_tag_valid
 -----------------
 */
static int dcnf_is_tag_valid(const xmlChar *tag)
{
    
    size_t n = xmlStrlen(tag);
    if(strlen(SUPMODELS) == n && memcmp(tag, ((unsigned char*)SUPMODELS), n) == 0)
        return 1;
    
    
    
    if(strlen(ALERTS) == n && memcmp(tag, ((unsigned char*)ALERTS), n) == 0)
        return 1;
    
    
    
    if(strlen(MECHANISM) == n && memcmp(tag, ((unsigned char*)MECHANISM), n) == 0)
        return 1;
    
    return 0;
}

/*
 dcnf_set_global_cnfg
 --------------------
 */
static void dcnf_set_global_cnfg(const xmlChar *tag, const xmlChar *value, detection_cnfg *dcnfg)
{
    
    size_t n = xmlStrlen(tag);
    if(strlen(SUPMODELS) == n && memcmp(tag, ((unsigned char*)SUPMODELS), n) == 0)
    {
        dcnfg->enable_supmodels = (char) strtol(( char *)value, NULL, 10);
        return;
    }
    
    
    if(strlen(ALERTS) == n && memcmp(tag, ((unsigned char*)ALERTS), n) == 0)
    {
        dcnfg->enable_alerts = (char) strtol(( char *)value, NULL, 10);
        return;
    }
    
    
    
    if(strlen(MECHANISM) == n && memcmp(tag, ((unsigned char*)MECHANISM), n) == 0)
    {
        dcnfg->mechanism = (char) strtol(( char *)value, NULL, 10);
        return;
    }
}

/*
 gcnf_is_tag_valid
 -----------------
 */
static int gcnf_is_gentags_valid(const xmlChar *tag)
{
    size_t n = xmlStrlen(tag);
    if(strlen(N) == n && memcmp(tag, ((unsigned char*)N), n) == 0)//TODO: l'espressione va bene?
        return 1;
    
    if(strlen(LCK) == n && memcmp(tag, ((unsigned char*)LCK), n) == 0)
        return 1;
    
    if(strlen(STRATEGY) == n && memcmp(tag, ((unsigned char*)STRATEGY), n) == 0)
        return 1;
    
    if(strlen(FORMAT) == n && memcmp(tag, ((unsigned char*)FORMAT), n) == 0)
        return 1;
    return 0;
}

/*
 gcnf_is_tag_valid
 -----------------
 */
static int gcnf_is_prtags_valid(const xmlChar *tag)
{
    size_t n = xmlStrlen(tag);
    
    if(strlen(PROTOCOL) == n && memcmp(tag, ((unsigned char*)PROTOCOL), n) == 0)
        return 1;
    
    if(strlen(PORT) == n && memcmp(tag, ((unsigned char*)PORT), n) == 0)
        return 1;
    
    
    if(strlen(THRESHOLD) == n && memcmp(tag, ((unsigned char*)THRESHOLD), n) == 0)
        return 1;
    return 0;
}

/*
 gcnf_set_global_cnfg
 --------------------
 */
static void gcnf_set_global_cnfg(const xmlChar *tag, const xmlChar *value, global_cnfg *gcnfg)
{
    size_t n = xmlStrlen(tag);
    
    if(strlen(N) == n && memcmp(tag, ((unsigned char*)N), n) == 0)//TODO: l'espressione va bene?
        gcnfg->n = (unsigned char) strtoul((char *)value, NULL, 10);
    
    
    if(strlen(LCK) == n && memcmp(tag, ((unsigned char*)LCK), n) == 0)
        gcnfg->lck = ( unsigned short ) strtoul(( char *)value, NULL, 10);
    
    
    if(strlen(STRATEGY) == n && memcmp(tag, ((unsigned char*)STRATEGY), n) == 0)
        gcnfg->strategy = (char) strtol(( char *)value, NULL, 10);
    
    
    if(strlen(FORMAT) == n && memcmp(tag, ((unsigned char*)FORMAT), n) == 0)
        gcnfg->format = (char) strtol(( char *)value, NULL, 10);
}


/*
 cnf_is_tag_valid
 -----------------
 */
static int cnf_is_tag_valid(const xmlChar *tag, char discr)
{
    if(discr == GLOBAL)
    {
        return gcnf_is_gentags_valid(tag);
    }
    else if(discr == TRAINING)
    {
        return tcnf_is_tag_valid(tag);
    }
    return dcnf_is_tag_valid(tag);
}

/*
 stream_cfile
 -------------
 */
static int stream_cfile(const char *filename, void *cnfg, char discr)
{
    xmlTextReaderPtr reader;
    xmlChar *uri;
    int ret;
    int ndtype;
    size_t n;
    const xmlChar *name, *value = NULL, *tag = NULL;
    
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL)
    {
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
        {
            // ho bisogno di sapere: nome del nodo, contenuto, attributi
            name = xmlTextReaderConstName(reader);
            if (name == NULL)
            {
                name = BAD_CAST "--";
            }
            
            
            if(cnf_is_tag_valid(name, discr))
            {
                // se si tratta di un tag devo controllare la presenza di attributi
                tag = name;
                if (xmlTextReaderNodeType(reader) == 1 && xmlTextReaderAttributeCount(reader) > 0)
                {
                    uri = xmlTextReaderGetAttributeNo(reader, 0); //mi aspetto solo un attributo
                    
                    if(discr == GLOBAL)
                        gcnf_set_global_cnfg(tag, uri, cnfg);//TODO: verificare il const
                    else if(discr == TRAINING)
                        tcnf_set_global_cnfg(tag, uri, cnfg);
                    else if(discr == DETECTION)
                        dcnf_set_global_cnfg(tag, uri, cnfg);
                    xmlFree(uri);
                }
                
            }
            else if(tag != NULL && cnf_is_tag_valid(tag, discr))
            {
                
                ndtype = xmlTextReaderNodeType(reader);
                n = sizeof(name) / sizeof(char);
                if(strlen(TEXT) < n)
                {
                    n = strlen(TEXT);
                }
                if(memcmp(name,(unsigned char*) TEXT, n) == 0 && ndtype == CONTENT_NODE)
                {
                    if (value == NULL)
                    {
                        value = xmlTextReaderConstValue(reader);
                        
                        if(discr == GLOBAL)
                            gcnf_set_global_cnfg(tag, value, cnfg);
                        else if(discr == TRAINING)
                            tcnf_set_global_cnfg(tag, value, cnfg);
                        else if(discr == DETECTION)
                            dcnf_set_global_cnfg(tag, value, cnfg);
                        value = NULL;
                    }
                }
                // nel caso in cui il prossimo name non sia un valido tag devo...
                tag = NULL;
            }
            
            
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
        {
            // failed to parse
//            fprintf(stderr, "%s : failed to parse\n", filename);
            return CR_UNABLE_PARSE;
        }
    }
    else
    {
        // Unable to open
//        fprintf(stderr, "Unable to open %s\n", filename);
        return CR_UNABLE_OPEN;
    }
    return CR_OK;
}

/*
 stream_gcfile
 -------------
 */
int stream_gcfile(const char *filename, global_cnfg *gcnfg)
{
    glist *tmp = new_glist(sizeof(protocol_data*));
    protocol_data *pdata = NULL;
    
    xmlChar *uri;
    xmlTextReaderPtr reader;
    int ret;
    size_t n;
    const xmlChar *name, *value = NULL, *tag = NULL;
    
    int ndtype;
    unsigned char proto = 0;//contatore per il numero di protocolli
    
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL)
    {
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
        {
            name = xmlTextReaderConstName(reader);
            
            
            if(gcnf_is_prtags_valid(name))
            {
                // caso speciale: protocollo
                n = xmlStrlen(name);
                
                if(strlen(PROTOCOL) == n && memcmp(name,  ( unsigned char *) PROTOCOL, n) == 0)
                {
                    proto++;
                    
                    if(proto % 2 != 0)// il parser interpreta sia il tag di inizio che di fine
                    {
                        pdata = (protocol_data*) malloc(sizeof(protocol_data));//TODO: effettuare controlli
                    }
                    else
                    {
                        add_last(tmp, pdata);
                        pdata = NULL;
                    }
                }
                
                tag = name;
                if (xmlTextReaderNodeType(reader) == 1 && xmlTextReaderAttributeCount(reader) > 0)
                {
                    uri = xmlTextReaderGetAttributeNo(reader, 0); //mi aspetto solo un attributo
                    n = xmlStrlen(tag);
                    
                    if(strlen(PROTOCOL) == n && memcmp(name,  ( unsigned char *) PROTOCOL, n) == 0)
                    {
                        pdata->prid = (char) strtol(( char *) uri, NULL, 10);
                        
                    }
                    
                    if(strlen(PORT) == n && memcmp(name,  ( unsigned char *) PORT, n) == 0)
                    {
                        pdata->dirc = (char) strtol(( char *) uri, NULL, 10);
                        
                    }
                    
                    xmlFree(uri);
                }
            }
            else if(tag != NULL)
            {
                
                ndtype = xmlTextReaderNodeType(reader);
                if(memcmp(name,(unsigned char*) TEXT, strlen(TEXT)) == 0 && ndtype == CONTENT_NODE)
                {
                    value = xmlTextReaderConstValue(reader);
                    if(value != NULL)
                    {
                        n = xmlStrlen(tag);
                        if(strlen(PORT) == n && memcmp(tag, ((unsigned char*)PORT), strlen(PORT)) == 0)
                            pdata->port = (unsigned short) strtol(( char *) value, NULL, 10);
                        
                        if(strlen(THRESHOLD) == n && memcmp(tag, ((unsigned char*)THRESHOLD), strlen(THRESHOLD)) == 0)
                            pdata->threshold = (float) strtod(( char *) value, NULL);
                        
                    }
                }
            }
            
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
        {
            // failed to parse
            return CR_UNABLE_PARSE;
        }
    }
    else
    {
        // Unable to open
        return CR_UNABLE_OPEN;
    }
    
    proto /= 2;// il parser interpreta sia il tag di inizio che di fine
    
    gcnfg->protocols = (protocol_data*) malloc(sizeof(protocol_data)*proto);
    gcnfg->nr_protocols = proto;
    
    for ( unsigned int i = 0; i < tmp->len; i++)
    {
        pdata = get_data(tmp, i);
        gcnfg->protocols [i] = *pdata;
        free(pdata);
    }
    
    destroy_glist(tmp);
    
    return stream_cfile(filename, gcnfg, GLOBAL);
}

/*
 stream_tcfile
 -------------
 */
int stream_tcfile(const char *filename, training_cnfg *tcnfg)
{
    return stream_cfile(filename, tcnfg, TRAINING);
}

/*
 stream_dcfile
 -------------
 */
int stream_dcfile(const char *filename, detection_cnfg *dcnfg)
{
    return stream_cfile(filename, dcnfg, DETECTION);
}
