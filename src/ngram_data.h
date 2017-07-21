//
//  ngram_data.h
//  pckadmdl
//
//  Created by Luciano on 29/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef ngram_data_h
#define ngram_data_h

/*Racchiude le statistiche calcolate per un ngram in un chunk*/
typedef struct ngram_stats
{
    float avg_occ; // contiene le occorrenze medie di apparizione per ogni posizione
    float std_dev;
    unsigned int obsv_pkts; // riferimento ai pacchetti in cui la sequenza è stata osservata (per ogni posizione)
    
    unsigned int supp;// occ_per_pkts per LS - nr_wosp per GS
}ngram_stats;

/*Utilizzo il pattern FIRST-CLASS ADT per astrarre la struttura dati*/
typedef struct ngram_data ngram_data;

ngram_stats* create_ngram_stats();

ngram_data* create_ngram_data( unsigned short chunks, char model, char strategy);

ngram_stats* get_statsofck(ngram_data *ngd, unsigned short ck);

unsigned short usedcks(ngram_data *ngd);

/**
 * Inserisce nella struttura ngram_data le statistiche Y calcolate
 * per il chunk ck.
 * NB: Non bisogna effettuare questa operazione se si sta modificando
 * il contenuto indirizzato da un puntatore ngram_stats, recuperato
 * per mezzo della funzione get_statsofck! In caso contrario, si ha
 * un comportamento indefinito. L'aggiornamento avviene automaticamente,
 * utilizzando il puntatore.
 *
 * @param ngd Struttura dati da aggiornare.
 * @param ck Indice del chunk da aggiornare.
 * @param stats Statistiche calcolate per il chunk.
 */
void set_statsofck(ngram_data *ngd, unsigned short ck, ngram_stats *stats);

ngram_stats* get_statsofdunit(ngram_data *ngd);

void set_statsofdunit(ngram_data *ngd, ngram_stats *stats);

void free_chunks(ngram_data *ngd);

void free_ngram_data(ngram_data *ngd);


#endif /* ngram_data_h */
