//
//  core_functionalities.h
//  PCKAD-v2
//
//  Created by Luciano on 08/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef core_functionalities_h
#define core_functionalities_h

#include "pckad_model.h"
#include "pktrepr.h"
#include "glist.h"
#include "pckad_sysconfig.h"

/*
 build_model:
 ============
 implementa la costruzione incrementale di un modello mdl, a partire dal
 contenuto rilevante pd di una porzione di pacchetto.
 Le feature considerate per la costruzione sono legate agli ngram,
 la cui lunghezza corrisponde a n, e, inoltre, si suppone che il contenuto
 sia stato logicamente suddiviso in chunk, di lunghezza pari a lck.
 NB: dato che possono esistere diversi algoritmi, a seconda della strategia scelta
 per analizzare i dati, viene adottato il pattern Strategy.
 
 @mdl: il modello da sottoporre a costruzioni incrementale.
 @pd: il contenuto di un pacchetto da cui estrarre i dati di interesse.
 @n: la lunghezza di un ngram.
 @lck: la lunghezza di un chunk.
 */
typedef void (*build_model) (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

/*
 check_profile:
 ==============
 verifica se il modello mdl e il pacchetto ppkt appartengono allo stesso
 profilo.
 
 @mdl: modello.
 @ppkt: rappresentazione interna di un pacchetto di rete.
 @prtnidx: indice della porzione di interesse del pacchetto ppkt.
 
 returns: 1 in caso di esito positivo, 0 altrimenti.
 */
char check_profile (pckad_model *mdl, pckad_pkt *ppkt, unsigned int prtnidx, unsigned short lck);

/*
 lookfor_replacement:
 ====================
 cerca un modello per ppkt le cui caratteristiche si avvicinano a quelle
 nel profilo di appartenenza del pacchetto. Restituisce l'indice corrispondente
 al modello trovato.
 
 @mdls: lista contenente i modelli di cui dispone il sistema.
 @ppkt: rappresentazione interna di un pacchetto di rete.
 @prtnidx: indice della porzione di interesse del pacchetto ppkt.
 @lck: lunghezza di un chunk.
 
 returns: indice del modello trovato.
 */
unsigned int lookfor_replacement (glist *mdls, pckad_pkt *ppkt, unsigned int prtnidx, unsigned short lck);

/*
 compute_as:
 ===========
 implementa il calcolo dell'indice di anomalia di una porzione di unità dati pd,
 a partire dal confronto con un modello mdl.
 Tale funzione prevede l'implementazione di una strategia basata su: 1. suddivisione
 di pd in chunk (la cui lunghezza corrisponde a lck); 2. uso degli ngram (di cui n
 rappresenta la lunghezza)per il calcolo dell'indice di anomalia.
 NB: dato che possono esistere diversi algoritmi, a seconda della strategia e del
 meccanismo scelti per analizzare i dati (si vedano i file di configurazione DETECTION e
 GLOBAL), viene adottato il pattern Strategy.
 
 @mdl: il modello con cui effettuare il confronto.
 @pd: il contenuto di un pacchetto di cui calcolare l'indice di anomalia.
 @n: la lunghezza di un ngram.
 @lck: la lunghezza di un chunk.
 @mechanism: indica il meccanismo da utilizzare per la classificazione (es. ZSCORE).
 
 returns: l'indice di anomalia calcolato per pd.
 */
typedef float (*compute_as) (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

#endif /* core_functionalities_h */
