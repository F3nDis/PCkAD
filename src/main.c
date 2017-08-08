//
//  main.c
//  PCkAD-v2
//
//  Created by Luciano Argento on 26/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>
#include <math.h>
#include <sys/stat.h>
#include "http_pkt_txtconverter.h"
#include "pckad_if.h"
#include "pckad_observer.h"
#include "pckad_subject.h"
#include "console_client.h"
#include "pckad_model.h"

#define OFFLINE 0
#define ONLINE 0

#define TESTING 1
#define MAIN 2


#define SUIT_MDLS 3
#define ATTDS_PATH_IDX 1
#define TRADS_PATH_IDX 2

#define PCAP_FORMAT 0
#define TXT_FORMAT 1

#define CNFG_PATH_IDX 0

#define DOT_DIR "."
#define DDOT_DIR ".."
#define DS_STORE_DIR ".DS_Store"

typedef struct input_main
{
    char * carg;
    char * targ;
    char * Targ;
    char * sarg;
    char * larg;
    char * parg;
    char * cnfg_path;
}input_main;

static void print_models(glist *mdls);

static glist* filter_mdls(PCkAD *pckad, glist *attmdls);

static char filter_file (const char* fn, char* badn);
static char filter_dirfiles (const char* fn);
static char checkopts(const char * carg, const char * sarg, const char * targ, const char * larg, const char * Targ, const char * parg, char mod);
static void run_training(PCkAD *pckad, const char * training_set);
static void run_training_on_txt(PCkAD *pckad, const char * training_set);
static void run_training_on_dir_txt(PCkAD *pckad, const char *path);
static void run_training_on_dir(PCkAD *pckad, const char *path);
static void run_offline_classification(PCkAD *pckad, const char * data_path);
static void offline_classification_dir(PCkAD *pckad, const char * data_path);
static void offline_classification_file(PCkAD *pckad, const char * data_path);
static void run_offline_classification_on_txt(PCkAD *pckad, const char * data_path);

static int main_body(input_main *im);
static int filtering_body(input_main *im);


int main(int argc, char **argv)
{
    char main_print = 1;
    char mod = 0;
    char modality = MAIN;
    char * carg = NULL;
    char * targ = NULL;
    char * Targ = NULL;
    char * sarg = NULL;
    char * larg = NULL;
    char * parg = NULL;
    
    int index;
    int c;
    
    if(modality == TESTING)
    {
        printf( "not included yet.\n");
    }
    else if(modality == MAIN)
    {
        opterr = 0;
        while ((c = getopt (argc, argv, "T:l:c:s:t:p:m:")) != -1)
            switch (c)
        {
            case 'c':
                carg = optarg;
                break;
                break;
            case 'l':
                larg = optarg;
                break;
            case 's':
                sarg = optarg;
                break;
            case 't':
                targ = optarg;
                break;
            case 'p':
                parg = optarg;
                break;
            case 'T':
                Targ = optarg;
                break;
            case 'm':
                mod = 0;
                break;
            case '?':
                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                fprintf (stderr, "Unknown option `\\x%x'.\n", optopt);
                exit(EXIT_FAILURE);
        }
        
        if(!checkopts(carg, sarg, targ, larg, Targ, parg, mod))
        {
            printf ("ERR:\n\tc_arg = %s\n\ts_arg = %s\n\tt_arg = %s\n\tl_arg = %s\n\tT_arg = %s\n\tmod = %d\n",
                    carg, sarg, targ, larg, Targ, mod);
            fprintf (stderr, "Le opzioni specificate sono errate!\n");
            exit(EXIT_FAILURE);
        }
        
        
        if(main_print)
        {
            printf ("INFO:\n\tc_arg = %s\n\ts_arg = %s\n\tt_arg = %s\n\tl_arg = %s\n\tT_arg = %s\n\tmod = %d\n",
                    carg, sarg, targ, larg, Targ, mod);
        }
        
        
        if(argc - optind != 1)
        {
            fprintf (stderr, "ERR: Numero di argomenti errato! Atteso: <percorso della cartella contenente i file di configurazione>\n");
            exit(EXIT_FAILURE);
        }
        
        
        if(main_print)
        {
            for (index = optind; index < argc; index++)
            {
                printf ("\tNon-option argument %s\n", argv[index]);
            }
        }
        
        input_main *im = (input_main*) malloc(sizeof(input_main));
        int res = 0;
        if(im == NULL)
        {
            exit(EXIT_FAILURE);
        }
        
        im->carg = carg;
        im->larg = larg;
        im->targ = targ;
        im->Targ = Targ;
        im->parg = parg;
        im->sarg = sarg;
        im->cnfg_path = argv[optind + CNFG_PATH_IDX];
        
        
        res = main_body(im);
        free(im);
        return res;
    }
    else if(modality == SUIT_MDLS)
    {
        if(argc != 4)
        {
            fprintf (stderr, "ERR: Numero di argomenti errato! Atteso: <attack set path> <training set path> <cnfg path>\n");
            exit(EXIT_FAILURE);
        }
        int offset = 1;
        
        input_main *im = (input_main*) malloc(sizeof(input_main));
        int res = 0;
        if(im == NULL)
        {
            exit(EXIT_FAILURE);
        }
        
        im->carg = carg;
        im->larg = larg;
        im->targ = argv[offset + TRADS_PATH_IDX];
        im->Targ = argv[offset + ATTDS_PATH_IDX];
        im->parg = parg;
        im->sarg = sarg;
        im->cnfg_path = argv[optind + CNFG_PATH_IDX];
        
        res = filtering_body(im);
        free(im);
        return res;
    }
    
    
    
//    printf("\nProgramma terminato.\n");
    return 0;
}

static int main_body(input_main *im)
{
    char print = 1;
    
    console_obsv *cobsv = create_console_obsv();
    pckad_observer *pobsv = create_pckad_observer(cobsv, notified_event);
    
    
    PCkAD *pckad = createPCkAD();
    attach(pobsv);
    init_PCkAD(pckad, im->cnfg_path);
    
    int ccd = 0;
    if((ccd = check_consistency(pckad->scnfg)) == CONSISTENCY_OK)
    {
        char pf = ( char ) strtol(im->parg, NULL, 10);
        start_training_phase(pckad);
        
        if(im->larg != NULL)
        {
            //STEP: caricamento dei modelli da file
            load_pckad_models(pckad, im->larg);
            if(pckad->models == NULL && pckad->aprx_models == NULL)
            {
                fprintf (stderr, "ERR: La configurazione dei modelli caricati non combacia con quella del file di configurazione!\n");
                exit(EXIT_FAILURE);
            }
            else if(print)
            {
                printf("INFO: Numero di modelli normali caricati: %u\n", pckad->models->len);
                print_models(pckad->models);
            }
        }
        else
        {
            
            
            
            if(im->targ != NULL)
            {
                if(pf == TXT_FORMAT)
                {
                    
                    run_training_on_txt(pckad, im->targ);
                }
                else
                {
                    run_training(pckad, im->targ);
                }
                if(print)
                {
                    printf("INFO: Numero di modelli normali costruiti: %u\n", pckad->models->len);
//                    print_models(pckad->models);
                }
            }
            else
            {
                if(pf == TXT_FORMAT)
                {
                    run_training_on_dir_txt(pckad, im->Targ);
                }
                else
                {
                    run_training_on_dir(pckad, im->Targ);
                }
                if(print)
                {
                    printf("INFO: Numero di modelli normali costruiti: %u\n", pckad->models->len);
                    //                    print_models(pckad->models);
                }
            }

            
            
            
            end_training_phase(pckad);
            if(im->sarg != NULL)
            {
                save_pckad_models(pckad, im->sarg);
            }
        }
        
        if(im->carg != NULL)
        {
            // STEP: esecuzione della fase di classificazione
            //TODO: per ora supporto solamente la classificazione off line
            if(pf == TXT_FORMAT)
            {
                run_offline_classification_on_txt(pckad, im->carg);
            }
            else
            {
                run_offline_classification(pckad, im->carg);
            }
        }
    }
    else
    {
        printf("Configurazione inconsistente! Codice: %d\n", ccd);
        exit(EXIT_FAILURE);
    }
    
    detach(pobsv);
    
    free_pckad_observer(pobsv);// rilascia anche le risorse per console_obsv
    
    
    free_PCkAD(pckad);
    
    return 0;
}

static int filtering_body(input_main *im)
{
    char print = 0;
    
    console_obsv *cobsv = create_console_obsv();
    pckad_observer *pobsv = create_pckad_observer(cobsv, notified_event);
    
    
    glist *attmdls = NULL, *fmdls = NULL;
    PCkAD *pckad = createPCkAD();
    attach(pobsv);
    init_PCkAD(pckad, im->cnfg_path);
    
    int ccd = 0;
    if((ccd = check_consistency(pckad->scnfg)) == CONSISTENCY_OK)
    {
        run_training_on_dir(pckad, im->Targ);
        end_training_phase(pckad);
        //TODO: ricavare i dati e rilasciare la memoria
        attmdls = pckad->models;
        
        
        pckad->models = new_glist(0);
        
        run_training_on_dir(pckad, im->targ);
        end_training_phase(pckad);
        //TODO: determinare i profili da utilizzare
        fmdls = filter_mdls(pckad, attmdls);
        
        
        if(print)
        {
            printf("INFO: numero di modelli di attacco = %u\n", attmdls->len);
            printf("INFO: modelli originali...\n");
            printf("\tnumero di modelli = %u\n", pckad->models->len);
            print_models(pckad->models);
            printf("INFO: modelli filtrati...\n");
            printf("\tnumero di modelli = %u\n", fmdls->len);
            print_models(fmdls);
        }
    }
    else
    {
        printf("Configurazione inconsistente! Codice: %d\n", ccd);
    }
    
    detach(pobsv);
    
    free_pckad_observer(pobsv);// rilascia anche le risorse per console_obsv
    
    
    
    free_PCkAD(pckad);
    return 0;
}

static char checkopts(const char * carg, const char * sarg, const char * targ, const char * larg, const char * Targ, const char * parg, char mod)
{
    char pf = ( char ) strtol(parg, NULL, 10);
    if(pf != TXT_FORMAT && pf != PCAP_FORMAT)
    {
        return 0;
    }
    
    if((larg != NULL && targ != NULL && Targ != NULL) || (larg == NULL && targ == NULL && Targ == NULL))
    {
        return 0;
    }
    
    if(larg != NULL)
    {
        if(Targ != NULL)
        {
            return 0;
        }
        if(sarg != NULL)
        {
            return 0;
        }
    }
    else if(targ != NULL)
    {
        if(Targ != NULL)
        {
            return 0;
        }
    }
    else
    {
        if(Targ == NULL)
        {
            return 0;
        }
    }
    
    //TODO: bisognerebbe verificare che se mod == ONLINE allora carg contiene il nome di un'interfaccia di rete
    
    return 1;
}

static void run_training(PCkAD *pckad, const char * data_path)
{
    char print = 1;
    if(print)
    {
        printf("\nINFO: esecuzione training su %s in corso...\n", data_path);
    }
    
    struct pcap_pkthdr header; // The header that pcap gives us
    const u_char *packet; // The actual packet
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    handle = pcap_open_offline(data_path, errbuf);   //call pcap library function
    //se non riesco ad aprire il dataset allora faccio terminare il programma con errore
    if (handle == NULL)
    {
        fprintf(stderr,"Couldn't open pcap file %s: %s\n", data_path, errbuf);
        
        exit (EXIT_FAILURE);
    }
    
    
    while ((packet = pcap_next(handle, &header)))
    {
        training_step(packet, header.len, pckad);
    }
    
    pcap_close(handle);  //close the pcap file
    
    
    if(print)
    {
        printf("\nINFO: training step terminato con successo!\n");
    }
}

static void run_offline_classification(PCkAD *pckad, const char * data_path)
{
    struct stat sb;
    
    if (stat(data_path, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        offline_classification_dir(pckad, data_path);
    }
    else
    {
        offline_classification_file(pckad, data_path);
    }
}

static void offline_classification_dir(PCkAD *pckad, const char * data_path)
{
    DIR *d;
    struct dirent * entry = NULL;//Rappresenta un elemento facente parte della cartella corrente
    unsigned short out_flnm_len= 150;//TODO: si può fare altro?
    
    // apro la cartella
    d = opendir (data_path);
    if (! d)
    {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                data_path, strerror (errno));
        exit (EXIT_FAILURE);
    }
    
    //piccola la fase di pre processamento: calcolo la lunghezza del percorso dalla cartella
    size_t dir_len = strlen(data_path);//TODO: stare attenti ai tipi utilizzati
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
        strcpy(fname_handle, data_path);// mi serve per concatenare il percorso al nome del file
        
        offline_classification_file(pckad, strcat(fname_handle, entry->d_name));
        
        free(fname_handle);
        
        i ++;
    }
    
    if (closedir (d))//chiudo la cartella
    {
        fprintf (stderr, "Could not close '%s': %s\n", data_path, strerror (errno));
        //exit (EXIT_FAILURE);
    }
}

static void offline_classification_file(PCkAD *pckad, const char * data_path)
{
    char print = 1;
    char print_as = 0;
    if(print)
    {
        printf("\nINFO: esecuzione classificazione in corso...\n");
    }
    
    float as = 0;
    unsigned int anom = 0, tot = 0;
    struct pcap_pkthdr header; // The header that pcap gives us
    const u_char *packet; // The actual packet
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    unsigned short last_port = 0;// Usato per ottimizzare
    float last_threshold = 0;
    protocol_data *pds = NULL;
    
    
    handle = pcap_open_offline(data_path, errbuf);   //call pcap library function
    //se non riesco ad aprire il dataset allora faccio terminare il programma con errore
    if (handle == NULL)
    {
        fprintf(stderr,"Couldn't open pcap file %s: %s\n", data_path, errbuf);
        
        exit (EXIT_FAILURE);
    }
    
    
    while ((packet = pcap_next(handle, &header)))
    {
        as = classify_data(packet, header.len, pckad);
        
        //        printf("DEBUG code: %d\n", *pckad->retcode);
        
        if(*pckad->retcode == CLASS_OK)
        {
            tot++;
            if(pckad->rd->port != last_port)
            {
                pds = pckad->scnfg->gc->protocols;
                for ( unsigned char i = 0; i < pckad->scnfg->gc->nr_protocols; i++)
                {
                    if(pckad->rd->port == pds [ i ].port)
                    {
                        last_port = pds [ i ].port;
                        last_threshold = pds [ i ].threshold;
                        break;
                    }
                }
            }
            if(as > last_threshold)
            {
                anom++;
            }
            
            if(print_as)
            {
                printf("%f\n", as);
            }
//                        for ( unsigned int i = 0; i < header.len; i++)
//                        {
//                            printf("%c", packet [ i ]);
//                        }
//                        printf("\n\n");
//                        printf("%s\n\n", pckad->rd->content);
        }
    }
    
    pcap_close(handle);  //close the pcap file
    
    
    if(print)
    {
        printf("\nCLASSFICATION RESULTS: %u pacchetti su %u sono stati classificati come anomali\n", anom, tot);
        printf("\nINFO: classificazione conclusa con successo!\n");
    }
}

static void run_training_on_dir(PCkAD *pckad, const char *path)
{
    DIR *d;
    struct dirent * entry = NULL;//Rappresenta un elemento facente parte della cartella corrente
    unsigned short out_flnm_len= 250;//TODO: si può fare altro?
    
    // apro la cartella
    d = opendir (path);
    if (! d)
    {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 path, strerror (errno));
        exit (EXIT_FAILURE);
    }
    
    //piccola la fase di pre processamento: calcolo la lunghezza del percorso dalla cartella
    size_t dir_len = strlen(path);//TODO: stare attenti ai tipi utilizzati
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
        strcpy(fname_handle, path);// mi serve per concatenare il percorso al nome del file
        
        run_training(pckad, strcat(fname_handle, entry->d_name));
        
        free(fname_handle);
        
        i ++;
    }
    
    if (closedir (d))//chiudo la cartella
    {
        fprintf (stderr, "Could not close '%s': %s\n", path, strerror (errno));
        //exit (EXIT_FAILURE);
    }
}




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

static void print_models(glist *mdls)
{
    pckad_model *mdl = NULL;
    for ( unsigned int i = 0; i < mdls->len; i++)
    {
        mdl = (pckad_model*) get_data(mdls, i);
        printf("\tcode: %d\n\t#op: %lu\n\t#ng: %u\n\tmin: %u\n\tmax: %u\n\n", mdl->spk_id, mdl->obsv_pkts, mdl->nr_of_seqs, mdl->min_pkt_len, mdl->max_pkt_len);
//        printf("%lu\n", mdl->obsv_pkts);
    }
}

static glist* filter_mdls(PCkAD *pckad, glist *attmdls)
{
    glist *fmdls = new_glist(0);
    pckad_model *m = NULL, *a = NULL;
    char match = 0;
    unsigned short mcks = 0, acks = 0;
    double avg_op = 0;// numero medio di pacchetti osservati
    
    for ( unsigned int i = 0; i < pckad->models->len; i++)
    {
        m = (pckad_model*) get_data(pckad->models, i);
        avg_op += m->obsv_pkts;
    }
    avg_op /= pckad->models->len;
    
    
    // STEP: filtro i modelli per numero medio di pacchetti osservati
    for ( unsigned int i = 0; i < pckad->models->len; i++)
    {
        m = (pckad_model*) get_data(pckad->models, i);
        
        if(m->obsv_pkts >= avg_op)
        {
            // STEP: filtro i modelli in funzione dei modelli dei pacchetti malevoli
            for ( unsigned int j = 0; j < attmdls->len; j++)
            {
                a = (pckad_model*) get_data(attmdls, j);
                mcks = ( unsigned short) ceilf(m->max_pkt_len / (float) pckad->scnfg->gc->lck);
                acks = ( unsigned short) ceilf(a->max_pkt_len / (float) pckad->scnfg->gc->lck);
                if(m->spk_id == a->spk_id && mcks == acks)
                {
                    match = 1;
                    break;
                }
            }
            
            if(match)
            {
                match = 0;
                add_last(fmdls, m);
            }
        }
    }
    
    return fmdls;
}

static void run_training_on_txt(PCkAD *pckad, const char * training_set)
{
    char print = 0;
    if(print)
    {
        printf("\nINFO: esecuzione training su %s in corso...\n", training_set);
    }
    
    typedef struct txt_header
    {
        unsigned int len; /* payload length */
        unsigned short port; /* tcp or udp port */
        unsigned char src; /* 1 source, 0 destination */
    }txt_header_t;
    
    unsigned int *txtplen = NULL;// contatore al numero di pacchetti testuali
    txtplen = ( unsigned int*) malloc(sizeof( unsigned int));
    unsigned char *packet = NULL;
    unsigned char **txtpkts = NULL;
    
    txt_header_t *hdr;
    
    //TODO: dovrei effettuare un controllo sulla tipologia di pacchetto
    
    FILE *ft = fopen(training_set, "r");
    txtpkts = convert_http_pkts(ft, 0, txtplen);
    fclose(ft);
    
    for ( unsigned int i = 0; i < *txtplen; i++)
    {
        packet = txtpkts[ i ];
        
        hdr = (txt_header_t*) (packet);
        if(hdr->len == 0)
        {
            
            printf("WTF??\n");
        }
        
        training_step(packet, hdr->len, pckad);
    }
    
    free(txtpkts);
    free(txtplen);
    
    if(print)
    {
        printf("\nINFO: training step terminato con successo!\n");
    }
}

static void run_training_on_dir_txt(PCkAD *pckad, const char *path)
{
    DIR *d;
    struct dirent * entry = NULL;//Rappresenta un elemento facente parte della cartella corrente
    unsigned short out_flnm_len= 150;//TODO: si può fare altro?
    
    // apro la cartella
    d = opendir (path);
    if (! d)
    {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 path, strerror (errno));
        exit (EXIT_FAILURE);
    }
    
    //piccola la fase di pre processamento: calcolo la lunghezza del percorso dalla cartella
    size_t dir_len = strlen(path);//TODO: stare attenti ai tipi utilizzati
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
        strcpy(fname_handle, path);// mi serve per concatenare il percorso al nome del file
        
        run_training_on_txt(pckad, strcat(fname_handle, entry->d_name));
        
        free(fname_handle);
        
        i ++;
    }
    
    if (closedir (d))//chiudo la cartella
    {
        fprintf (stderr, "Could not close '%s': %s\n", path, strerror (errno));
        //exit (EXIT_FAILURE);
    }
}

static void run_offline_classification_on_txt(PCkAD *pckad, const char * data_path)
{
    char print = 0;
    char print_as = 0;
    if(print)
    {
        printf("\nINFO: esecuzione classificazione in corso...\n");
    }
    
    typedef struct txt_header
    {
        unsigned int len; /* payload length */
        unsigned short port; /* tcp or udp port */
        unsigned char src; /* 1 source, 0 destination */
    }txt_header_t;
    
    unsigned int *txtplen = NULL;// contatore al numero di pacchetti testuali
    txtplen = ( unsigned int*) calloc(1, sizeof( unsigned int));
    unsigned char *packet = NULL;
    unsigned char **txtpkts = NULL;
    
    txt_header_t *hdr;
    
    
    
    float as = 0;
    unsigned int anom = 0, tot = 0;
    unsigned short last_port = 0;// Usato per ottimizzare
    float last_threshold = 0;
    protocol_data *pds = NULL;
    
    
    //TODO: dovrei effettuare un controllo sulla tipologia di pacchetto
    FILE *ft = fopen(data_path, "r");
    txtpkts = convert_http_pkts(ft, 0, txtplen);
    fclose(ft);
    
    for ( unsigned int i = 0; i < *txtplen; i++)
    {
        packet = txtpkts[ i ];
        hdr = (txt_header_t*) (packet);
        
        
        as = classify_data(packet, hdr->len, pckad);
        
        if(*pckad->retcode == CLASS_OK)
        {
            tot++;
            if(pckad->rd->port != last_port)
            {
                pds = pckad->scnfg->gc->protocols;
                for ( unsigned char i = 0; i < pckad->scnfg->gc->nr_protocols; i++)
                {
                    if(pckad->rd->port == pds [ i ].port)
                    {
                        last_port = pds [ i ].port;
                        last_threshold = pds [ i ].threshold;
                        break;
                    }
                }
            }
            
            if(print_as)
            {
                printf("%f\n", as);
            }
            if(as > last_threshold)
            {
                anom++;
            }
//            else
//            {
//                if(as >0.3)
//                {
//                    for ( unsigned int i = 7; i < hdr->len; i++)
//                    {
//                        printf("%c", packet [ i ]);
//                    }
//                    printf("\n\n");
//                }
//            }
        }
    }
    
    free(txtpkts);
    free(txtplen);
    

    
    if(print)
    {
        printf("\nCLASSFICATION RESULTS: %u pacchetti su %u sono stati classificati come anomali\n", anom, tot);
        printf("\nINFO: classificazione conclusa con successo!\n");
    }
}
























