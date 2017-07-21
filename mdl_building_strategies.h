//
//  mdl_building_strategies.h
//  PCKAD-v2
//
//  Created by Luciano on 08/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef mdl_building_strategies_h
#define mdl_building_strategies_h

#include "pckad_model.h"

/**
 Implementa la costruzione incrementale di un modello mdl, a partire dal
 contenuto rilevante pd di una porzione di pacchetto, seguendo la strategia 2 Level Strategy.
 L'apprendimento si basa sugli ngram e la suddivisione del contenuto del pacchetto in chunk.
 
 @param mdl il modello da costruire.
 @param pd struttura dati associata al pacchetto da analizzare.
 @param n lunghezza degli ngram (espressa in byte).
 @param lck lunghezza di un singolo chunk (espressa in byte).
 */
void build_2lsmdl (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

/**
 Implementa la costruzione incrementale di un modello mdl, a partire dal
 contenuto rilevante pd di una porzione di pacchetto, seguendo la strategia Local Strategy.
 L'apprendimento si basa sugli ngram e la suddivisione del contenuto del pacchetto in chunk.
 
 @param mdl il modello da costruire.
 @param pd struttura dati associata al pacchetto da analizzare.
 @param n lunghezza degli ngram (espressa in byte).
 @param lck lunghezza di un singolo chunk (espressa in byte).
 */
void build_lsmdl (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

/**
 Implementa la costruzione incrementale di un modello mdl, a partire dal
 contenuto rilevante pd di una porzione di pacchetto, seguendo la strategia Global Strategy.
 L'apprendimento si basa sugli ngram.
 
 @param mdl il modello da costruire.
 @param pd struttura dati associata al pacchetto da analizzare.
 @param n lunghezza degli ngram (espressa in byte).
 @param lck lunghezza di un singolo chunk (espressa in byte).
 */
void build_gsmdl (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck);

#endif /* mdl_building_strategies_h */
