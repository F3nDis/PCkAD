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

/* First class ADT to hide any detail concerning the data structure containing the ngrams */
typedef struct mdlcr model_core;

typedef struct pckad_model
{
    model_core *core;// data structure that contains ngrams and data pertaining their distribution
    unsigned long obsv_pkts;
    unsigned int nr_of_seqs;//shows the total number of ngrams stored in a model
    // for the following lengths, int is used for possible uses of data flow (instead of network packets)
    unsigned int min_pkt_len;//  the smallest packet length observed in the profile
    unsigned int max_pkt_len;// the longest packet length observed in the profile
    unsigned short port;
    unsigned short mdl_id;
    char is_dest;
    char spk_id;// used to determine the type of packet payload
}pckad_model;

/**
 Initialises the pckad_model data structure starting from a representative packet ppkt.
 
 @param ppkt network packet from which data characterising a specific network traffic profile are extracted.
 @param prtn_idx index that represents the position of the payload portion of interest.
 
 @return the initialised data structure or NULL if no memory is available.
 */
pckad_model* create_model(pckad_pkt *ppkt, unsigned int prtn_idx);

/**
 Compares two models p1 and p2.
 
 @return 1 if p1 > p2, -1 if p1 < p2, 0 otherwise.
 */
char compare_model (void * p1, void* p2);

/**
 Frees mdl
 
 @param mdl the model to free.
 */
void free_model (pckad_model* mdl);

/**
 Frees the memory allocated for nd.
 
 @param nd the object to free.
 */
void free_ngram_data(ngram_data *nd);

/***
Checks if there exists a ngram in mdlcore. If true it returns
the data structure containing data associated to the query, else it returns NULL.
The function can also be employed just to check whether a ngram is stored.
 
 @param mdlcore Component of the model that stores the data collected during the training phase.
 @param ngram   ngram to add.
 @param len     ngram length.
 @param retcode pointer to the operation result.
 
 @return structure containing data associated to ngram.
 */
ngram_data* get_ngrdata(model_core *mdlcore, unsigned char *ngram, char len, int *retcode);

/**
 Adds a new ngram to the data structure, as well as its data.
 The function reserves new memory for ngram, in case of insertion, while ngdt is shared.
 Returns a code that denotes the operation result.
 
 @param mdlcore Component of the model that stores the data collected during the training phase.
 @param ngram   ngram to add.
 @param len     ngram length.
 @param ngdt    data related to ngram.
 
 @return code denoting the operation result:
 i) the operation succedes;
 ii) the operation fails due to the presence of ngram in the data structure;
 iii) the operation fails due to other types of errors.
 */
int add_ngrdata(model_core *mdlcore, unsigned char *ngram, char len, ngram_data *ngdt);

/**
 Returns all the ngrams contained in the data structure needed to query the model.
 NB temporary implementation (efficiency reasons).
 
 @param mdlcore Component of the model that stores the data collected during the training phase.
 @param len     pointer to the array length (it must be initialised).
 
 @return array containing the pointers to the ngram_obj objects.
 */
ngram_obj** getall_ngrams(model_core *mdlcore, int *len);

/**
 Stores the models contained in mdls in one or more binary files.
 specifically, for each set of models, identified by the triple <port, direction, packet code>, a new file is created.
 
 @param path destination path for the files.
 @param mdls list containing the models to store.
 @param n ngram length.
 @param lck chunk length.
 @param strategy used to choose the training and classification strategy (GS, LS, 2LS).
 */
void store_models(const char* path, glist *mdls, unsigned char n, unsigned short lck, char strategy);

/**
 Loads the models stored in a binary file whose path is provided by path.
 The input parameters n, lck and strategy are set with the values read from the file.
 
 @param path source file path containing the binary representation of the models.
 @param n pointer to the ngram length read from the file, after the execution.
 @param lck pointer to the chunk length read from the file, after the execution.
 @param strategy pointer to the strategy read from the file, after the execution.
 
 @return list containing the models loaded from the file.
 */
glist* load_models (const char* path, unsigned char *n, unsigned short *lck, char *strategy);

#endif /* pckad_model_h */
