//
//  pckad_if.c
//  PCKAD-v2
//
//  Created by Luciano on 25/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "pckad_if.h"
#include "pckad_subject.h"
#include "pckad_observer.h"
#include "cnfg_reader_director.h"
#include "preprocessor.h"
#include "as_computation_strategies.h"
#include "training_module.h"
#include "classification_module.h"
#include "inspector_if.h"

#define PCKAD_INIT "INFO: inizializzazione di PCKAD conclusa con successo!"
#define PCKAD_LOAD1 "INFO: lettura dei modelli da %s in corso..."
#define PCKAD_LOAD "INFO: modelli caricati con successo!"
#define PCKAD_STORE "INFO: modelli salvati con successo!"
#define PCKAD_MDL_STATS "END TRAINING: costruzione dei modelli conclusa con successo!"

#define DOT_DIR "."
#define DDOT_DIR ".."
#define DS_STORE_DIR ".DS_Store"

glist *observers;

static char filter_file (const char* fn, char* badn);
static char filter_dirfiles (const char* fn);

static char filter_file (const char* fn, char* badn)
{
    unsigned short i = 0;
    char match = 1;
    while (fn[i])
    {
        if (badn[i])
        {
            if (fn [i] != badn[i])
            {
                match = 0;
                break;
            }
        }
        else if (badn[i])
        {
            match = 0;
            break;
        }
        i++;
    }
    
    return match;
}

/*
 filter_dirfiles:
 ================
 Filtra il nome fn di un file utilizzando nomi predefiniti di file.
 
 @fn: il nome del file da analizzare.
 
 returns: 1 se il file deve essere filtrato, 0 altrimenti.
 */
static char filter_dirfiles (const char* fn)
{
    return filter_file(fn, DOT_DIR) || filter_file(fn, DDOT_DIR) || filter_file(fn, DS_STORE_DIR);
}

PCkAD *createPCkAD()
{
    PCkAD *pckad = (PCkAD*) malloc(sizeof(PCkAD));
    if(pckad != NULL)
    {
        //TODO: gestire la robustezza
        pckad->models = new_glist(0);
        pckad->retcode = (int*) malloc(sizeof(int));
        *pckad->retcode = 0;
        pckad->scnfg = NULL;
        pckad->aprx_models = NULL;
        pckad->rd = create_result_description();
        observers = new_glist(0);
    }
    return pckad;
}

void attach (pckad_observer *obsv)
{
    add_last(observers, obsv);
}

void detach (pckad_observer *obsv)
{
    pckad_observer *item = NULL;
    unsigned int r = 0;
    for ( unsigned int i = 0; i < observers->len; i++)
    {
        item = (pckad_observer*) get_data(observers, i);
        if(obsv == item)//TODO: basata su i puntatori
        {
            r = i;
            break;
        }
    }
    remove_data(observers, r);
}


void init_PCkAD(PCkAD *pckad, const char *config_path)
{
    pckad_observer *obsv = NULL;
    
    pckad->scnfg = build_config(config_path, pckad->retcode);
    //TODO: controllare il codice di ritorno
    
    // inizializzazione dei moduli (se richiesta)
    preprocessor_init(config_path);
    cmptas_init(config_path);
    
    for ( unsigned int i = 0; i < observers->len; i++)
    {
        obsv = (pckad_observer*) get_data(observers, i);
        obsv->notify (obsv->instance, PCKAD_INIT);
    }
}

void load_pckad_models(PCkAD *pckad, const char *models_path)
{
    pckad_observer *obsv = NULL;
    glist *mdls = new_glist(0);
    //TODO: devo leggere i file contenuti in models_path
    unsigned char *n = ( unsigned char*) malloc(sizeof( unsigned char));
    char *strategy = (char*) malloc(sizeof(char));
    unsigned short *lck = ( unsigned short*) malloc(sizeof( unsigned short));
    glist *tmp = NULL;
    pckad_model *mdl = NULL;
    DIR *d;
    struct dirent * entry = NULL;//Rappresenta un elemento facente parte della cartella corrente
    unsigned short out_flnm_len= 150;//TODO: si può fare altro?
    
    // apro la cartella
    d = opendir (models_path);
    if (! d)
    {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 models_path, strerror (errno));
        exit (EXIT_FAILURE);
    }
    
    //piccola la fase di pre processamento: calcolo la lunghezza del percorso dalla cartella
    size_t dir_len = strlen(models_path);//TODO: stare attenti ai tipi utilizzati
    size_t tot_len = dir_len + 1 + out_flnm_len;
    char *fname_handle = NULL;//nome del file (con il percorso) per handle
    
    unsigned short i = 0;
    while (1)
    {
        //Mi procuro il nome del file
        entry = readdir (d);
        if (! entry)
        {
            break;
        }
        if (filter_dirfiles(entry->d_name))// non si tratta del file ricercato
            continue;
        fname_handle = (char*) calloc(tot_len, sizeof(char));//TODO: memoria da rilasciare!
        strcpy(fname_handle, models_path);// mi serve per concatenare il percorso al nome del file
        
        
        
        tmp = load_models(strcat(fname_handle, entry->d_name), n, lck, strategy);
        for ( unsigned int i = 0; i < tmp->len; i++)
        {
            mdl = (pckad_model*) get_data(tmp, i);
            add_last(mdls, mdl);
        }
        destroy_glist(tmp);
        free(fname_handle);
        
        i ++;
    }
    
    destroy_glist(pckad->models);
    pckad->models = NULL;
    pckad->models = mdls;
    
    if (closedir (d))//chiudo la cartella
    {
        fprintf (stderr, "Could not close '%s': %s\n", models_path, strerror (errno));
        //exit (EXIT_FAILURE);
    }
    
    // STEP: prima di concludere, devo verificare che i parametri siano coerenti con la configurazione corrente
    if(*n != pckad->scnfg->gc->n || *lck != pckad->scnfg->gc->lck || *strategy != pckad->scnfg->gc->strategy)
    {
        //TODO: soluzione temporanea
        printf ("ERR: I parametri n, lck e strategy non combaciano con la configurazione corrente!\n");
        exit (EXIT_FAILURE);
    }
    
    free(n);
    free(lck);
    free(strategy);
    
    
    for ( unsigned int i = 0; i < observers->len; i++)
    {
        obsv = (pckad_observer*) get_data(observers, i);
        obsv->notify (obsv->instance, PCKAD_LOAD);
    }
}

void save_pckad_models(PCkAD *pckad, const char *models_path)
{
    pckad_observer *obsv = NULL;
    glist *mdls = NULL;
    mdls = pckad->models;
    store_models(models_path, mdls, pckad->scnfg->gc->n, pckad->scnfg->gc->lck, pckad->scnfg->gc->strategy);
    
    
    for ( unsigned int i = 0; i < observers->len; i++)
    {
        obsv = (pckad_observer*) get_data(observers, i);
        obsv->notify (obsv->instance, PCKAD_STORE);
    }
}

void free_PCkAD(PCkAD *pckad)
{
    if(observers != NULL)
    {
        destroy_glist(observers);//TODO: gli osservatori devono essere rilasciati dal client?
    }
    
    pckad_model *mdl = NULL;
    
    free(pckad->retcode);
    
    // rilascio le risorse dei moduli inizializzati
    preprocessor_destr();
    free_cmptas();
    
    free_result_description(pckad->rd);
    
    
    if(pckad->models != NULL)
    {
        for ( unsigned int i = 0; i < pckad->models->len; i++)
        {
            mdl = (pckad_model*) get_data(pckad->models, i);
            free_model(mdl);
        }
        destroy_glist(pckad->models);
    }
    
    
    
    free_pckad_sysconfig(pckad->scnfg);
    free(pckad);
}

void start_training_phase(PCkAD *pckad)
{
    //TODO: implementare
}

void training_step(const unsigned char *data, unsigned int len, PCkAD *pckad)
{
    *pckad->retcode = train_on_pkt(data, len, pckad->models, pckad->scnfg);
    
    switch (*pckad->retcode)
    {
        case INSPECTION_INVALID:
            *pckad->retcode = TRAIN_UNSUPPORTED;
            break;
            
        case INSPECTION_OMEM:
            *pckad->retcode = TRAIN_OMEM;
            break;
            
        case PRPROC_MALFORMED:
            *pckad->retcode = TRAIN_MALFORMED;
            break;
            
        case PRPROC_UNINTERESTING:
            *pckad->retcode = TRAIN_UNINTERESTING;
            break;
            
        case PRPROC_UNSUPPORTED:
            *pckad->retcode = TRAIN_UNSUPPORTED;
            break;
            
        case PRPROC_OMEM:
            *pckad->retcode = TRAIN_OMEM;
            break;
            
        default:
            *pckad->retcode = TRAIN_OK;
            break;
    }
}

void end_training_phase(PCkAD *pckad)
{
    pckad_observer *obsv = NULL;
    end_training(pckad->models, pckad->scnfg);
    
    
    for ( unsigned int i = 0; i < observers->len; i++)
    {
        obsv = (pckad_observer*) get_data(observers, i);
        obsv->notify (obsv->instance, PCKAD_MDL_STATS);
    }
    
    
}

float classify_data(const unsigned char *data, unsigned int len, PCkAD *pckad)
{
    float as = 0;
    
    // STEP: importante! Bisogna rilasciare la memoria assegnata pckad->rd->content
    if(pckad->rd->content != NULL)
    {
        free(pckad->rd->content);
        pckad->rd->content = NULL;
    }
    
    //TODO: il sistema deve controllare la consistenza della configurazione
    as = analyse_pkt(data, len, pckad->models, pckad->scnfg, pckad->retcode, pckad->rd);
    
    
    switch (*pckad->retcode)
    {
        case CLMODEL_MISSING:
            *pckad->retcode = CLASS_NOTPROCESSED;
            break;
            
        case INSPECTION_INVALID:
            *pckad->retcode = CLASS_UNSUPPORTED;
            break;
            
        case INSPECTION_OMEM:
            *pckad->retcode = CLASS_OMEM;
            break;
            
        case PRPROC_MALFORMED:
            *pckad->retcode = CLASS_MALFORMED;
            break;
            
        case PRPROC_UNINTERESTING:
            *pckad->retcode = CLASS_UNINTERESTING;
            break;
            
        case PRPROC_UNSUPPORTED:
            *pckad->retcode = CLASS_UNSUPPORTED;
            break;
            
        case PRPROC_OMEM:
            *pckad->retcode = CLASS_OMEM;
            break;
            
        default:
            *pckad->retcode = CLASS_OK;
            break;
    }

    
    return as;
}























