//
//  pckad_model.h
//  PCKAD-v2
//
//  Created by Luciano on 06/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef pckad_model_h
#define pckad_model_h

#include "glist.h"
#include "pktrepr.h"
#include "hash_map.h"
#include "ngram_data.h"

#define MODEL_MISSING 0
#define MODEL_ADDED 1
#define MODEL_OMEM 2
#define MODEL_FOUND 3

/* First class ADT per nascondere la struttura dati contenente gli ngram */
typedef struct mdlcr model_core;

typedef struct pckad_model
{
    model_core *core;// struttura dati che conterrà gli n-gram e relativi dati
    unsigned long obsv_pkts;
    unsigned int nr_of_seqs;//indica il numero totale di sequenze memorizzate nel profilo
    // per le lunghezze a seguire viene usato int per eventuali utilizzi del sistema su flussi di dati
    unsigned int min_pkt_len;// la più piccola lunghezza osservata dei pacchetti inseriti nel cluster
    unsigned int max_pkt_len;// la più grande lunghezza osservata dei pacchetti inseriti nel cluster
    unsigned short port;
    unsigned short mdl_id;
    char is_dest;
    char spk_id;// serve per identificare la porzione del pacchetto ritenuta utile ai fini dell'analisi
}pckad_model;

/**
 inizializza la struttura dati pckad_model a partire da un pacchetto rappresentativo.
 
 @param ppkt pacchetto di rete da cui vengono estratti dati che caratterizzano uno specifico profilo di traffico.
 @param prtn_idx indice indicante la posizione della porzione del pacchetto di interesse per il modello.
 
 @return la struttura dati inizializzata oppure NULL in caso mancanza di memoria.
 */
pckad_model* create_model(pckad_pkt *ppkt, unsigned int prtn_idx);

/**
 effettua il confronto tra due modelli dati in input.
 
 @return 1 se p1 > p2, -1 se p1 < p2, 0 altrimenti.
 */
char compare_model (void * p1, void* p2);

/**
 rilascia la memoria allocata per il modello dato in input.
 
 @param mdl il modello da rilasciare.
 */
void free_model (pckad_model* mdl);

/**
 rilascia la memoria allocata per una struttura ngram_data.
 
 @param nd l'oggetto di cui rilasciare la memoria.
 */
void free_ngram_data(ngram_data *nd);

/***
 Controlla se esiste ngram nella struttura dati, in caso di esito
 positivo restituisce la struttura dati contenente i dati associati
 alla query. In caso contrario viene restituito NULL. Data la natura del
 risultato, la funzione può essere utilizzata anche solamente per interrogare
 la presenza del ngram.
 
 @param mdlcore componente del modello in cui sono memorizzati dati raccolti durante il training.
 @param ngram   da aggiungere.
 @param len     lunghezza del ngram.
 @param retcode puntatore ad un intero indicante il risultato dell'operazione.
 
 @return la struttura dati contenente i dati associati al ngram.
 */
ngram_data* get_ngrdata(model_core *mdlcore, unsigned char *ngram, char len, int *retcode);

/**
 Aggiunge un nuovo ngram nella struttura dati, con annessi dati.
 La funzione riserva nuova memoria per ngram, in caso di aggiunta, mentre
 l'oggetto ngdt viene condiviso.
 Restituisce un codice indicante l'esito dell'operazione.
 
 @param mdlcore componente del modello in cui sono memorizzati dati raccolti durante il training.
 @param ngram ngram da aggiungere.
 @param len lunghezza del ngram.
 @param ngdt i dati associati al ngram.
 
 @return codice indicante l'esito dell'operazione 
 i) l'operazione ha successo;
 ii) l'operazione fallisce data la presenza del ngram nella struttura dati;
 iii) l'operazione fallisce per errori terzi.
 */
int add_ngrdata(model_core *mdlcore, unsigned char *ngram, char len, ngram_data *ngdt);

/**
 restituisce tutti gli ngram contenuti nella struttura dati, necessari
 per interrogare il modello.
 NB implementazione temporanea (motivi di efficienza).
 
 @param mdlcore componente del modello in cui sono memorizzati dati raccolti durante il training.
 @param len     puntatore alla lunghezza dell'array (deve essere inizializzato).
 
 @return array contenente i puntatori agli oggetti ngram_obj.
 */
ngram_obj** getall_ngrams(model_core *mdlcore, int *len);

/**
 salva il contenuto dei modelli della lista mdls in uno o più file binari.
 Nello specifico, viene creato un file per ogni insieme di modelli che condividono
 <porta, direzione, codice pacchetto>.
 
 @param path percorso di destinazione dei file.
 @param mdls lista contenente i modelli da salvare in memoria secondaria.
 @param n lunghezza degli ngram.
 @param lck lunghezza di un chunk.
 @param strategy strategia di training e classificazione (GS, LS, 2LS). Determina la natura di un modello.
 */
void store_models(const char* path, glist *mdls, unsigned char n, unsigned short lck, char strategy);

/**
 carica il contenuto dei modelli contenuti in un file binario il cui percorso specificato da path.
 Viene controllato che i parametri di input n, lck e strategy (relativi alla configurazione corrente
 del sistema), siano coerenti con i valori indicati dal file.
 
 @param path percorso del file sorgente contenente la rappresentazione binaria dei modelli.
 @param n lunghezza degli ngram specificata nel file di configurazione.
 @param lck lunghezza di un chunk specificata nel file di configurazione.
 @param strategy strategia di training e classificazione (GS, LS, 2LS). Determina la natura di un modello
 specificata nel file di configurazione.
 
 @return lista dei modelli caricati dal file specificato in path.
 */
glist* load_models (const char* path, unsigned char *n, unsigned short *lck, char *strategy);

#endif /* pckad_model_h */
