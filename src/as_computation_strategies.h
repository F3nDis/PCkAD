//
//  as_computation_strategies.h
//  cmptas
//
//  Created by Luciano on 15/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef as_computation_strategies_h
#define as_computation_strategies_h

#include "pckad_model.h"
#include "pktrepr.h"

#define CMPTAS_LCNF_OK 1
#define CMPTAS_LCNF_FAIL 2
#define CMPTAS_LCNF_OMEM 0

/*
 cmptas_init:
 ============
 esegue l'inizializzazione del modulo di calcolo degli indici di anomalia.
 
 @cnfs_path: percorso della cartella dei file di configurazione.
 
 returns: il codice di stato rappresentante l'esito dell'operazione.
 */
int cmptas_init (const char *path);

/**
 Rilascia le risorse allocate per la libreria cmptas.
 */
int free_cmptas ();

/*
 lsas_ngcomputation:
 ===================
 implementa il calcolo dell'indice di anomalia as della porzione di unità dati
 pd, impiegando lo zscore, basandosi sulla strategia LS.
 L'algoritmo analizza ogni singolo ngram ng in ciascun chunk ck di pd
 ed esegue i seguenti passi:
 1 per ogni ck
 1.1 se ng appartiene ad mdl ==> calcola lo zscore zs di pd
 1.2 se ng non appartiene ad mdl ==> classifica ng come anomalo
 2 infine, calcola as come #ng_a/#ng_t, dove #ng_a rappresenta il numero di
 ngram anomali in pd mentre #ng_t il numero totale di ngram in pd.
 
 @mdl: il modello con cui effettuare il confronto.
 @pd: il contenuto di un pacchetto di cui calcolare l'indice di anomalia.
 @n: la lunghezza di un ngram.
 @lck: la lunghezza di un chunk.
 @mechanism: indica il meccanismo da utilizzare per la classificazione (es. ZSCORE).
 
 returns: l'indice di anomalia calcolato per pd.
 */
float lsas_ngcomputation (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

/*
 twolsas_ngcomputation:
 ======================
 implementa il calcolo dell'indice di anomalia as della porzione di unità dati
 pd, impiegando lo zscore, basandosi sulla strategia 2LS.
 L'algoritmo analizza ogni singolo ngram ng in pd e in ciascun suo chunk ck
 ed esegue un algoritmo che combina le strategie GS e LS.
 L'indice di anomalia viene calcolato come #ng_a/#ng_t, dove #ng_a rappresenta il numero di
 ngram anomali in pd mentre #ng_t il numero totale di ngram in pd.
 
 @mdl: il modello con cui effettuare il confronto.
 @pd: il contenuto di un pacchetto di cui calcolare l'indice di anomalia.
 @n: la lunghezza di un ngram.
 @lck: la lunghezza di un chunk.
 @mechanism: indica il meccanismo da utilizzare per la classificazione (es. ZSCORE).
 
 returns: l'indice di anomalia calcolato per pd.
 */
float twolsas_ngcomputation (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

/*
 gsas_ngcomputation:
 ===================
 implementa il calcolo dell'indice di anomalia as della porzione di unità dati
 pd, impiegando lo zscore, basandosi sulla strategia GS.
 L'algoritmo analizza ogni singolo ngram ng in pd ed esegue i seguenti passi:
 1.1 se ng appartiene ad mdl ==> calcola lo zscore zs di pd
 1.2 se ng non appartiene ad mdl ==> classifica ng come anomalo
 2 infine, calcola as come #ng_a/#ng_t, dove #ng_a rappresenta il numero di
 ngram anomali in pd mentre #ng_t il numero totale di ngram in pd.
 
 @mdl: il modello con cui effettuare il confronto.
 @pd: il contenuto di un pacchetto di cui calcolare l'indice di anomalia.
 @n: la lunghezza di un ngram.
 @lck: la lunghezza di un chunk.
 @mechanism: indica il meccanismo da utilizzare per la classificazione (es. ZSCORE).
 
 returns: l'indice di anomalia calcolato per pd.
 */
float gsas_ngcomputation (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

#endif /* as_computation_strategies_h */





















