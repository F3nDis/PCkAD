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
 Initialises the library, based on a configuration file.
 
 @cnfs_path: configuration file path.
 
 returns: a numeric code representing the operational result.
 */
int cmptas_init (const char *path);

/**
 Frees the memory reserved for the library.
 
 returns: a numeric code representing the operational result.
 */
int free_cmptas ();

/*
 Computes the anomaly score of the relevant payload pd, by comparing pd with its matching the model mdl. 
 The computation is based on the LS strategy and a mechanism for comparing  ngrams (e.g. Z-score).

 @mdl: The model employed for the comparison.
 @pd: The relevant payload for which the function computes the anomaly score.
 @n: The ngram length.
 @lck: The chunk length.
 @mechanism: It denotes what mechanism should be used for competition (e.g. Z-score).
 
 returns: The anomaly score of pd.
 */
float lsas_ngcomputation (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

/*
 Computes the anomaly score of the relevant payload pd, by comparing pd with its matching the model mdl. 
 The computation is based on the 2LS strategy and a mechanism for comparing  ngrams (e.g. Z-score).

 @mdl: The model employed for the comparison.
 @pd: The relevant payload for which the function computes the anomaly score.
 @n: The ngram length.
 @lck: The chunk length.
 @mechanism: It denotes what mechanism should be used for competition (e.g. Z-score).
 
 returns: The anomaly score of pd.
 */
float twolsas_ngcomputation (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

/*
 Computes the anomaly score of the relevant payload pd, by comparing pd with its matching the model mdl. 
 The computation is based on the GS strategy and a mechanism for comparing  ngrams (e.g. Z-score).

 @mdl: The model employed for the comparison.
 @pd: The relevant payload for which the function computes the anomaly score.
 @n: The ngram length.
 @lck: The chunk length.
 @mechanism: It denotes what mechanism should be used for competition (e.g. Z-score).
 
 returns: The anomaly score of pd.
 */
float gsas_ngcomputation (pckad_model *mdl, pkt_data *pd, unsigned char n, unsigned short lck, char mechanism);

#endif /* as_computation_strategies_h */





















