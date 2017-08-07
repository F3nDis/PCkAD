//
//  ngram_data.h
//  pckadmdl
//
//  Created by Luciano on 29/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef ngram_data_h
#define ngram_data_h

/* Contains the statistics computed the for a ngram */
typedef struct ngram_stats
{
    float avg_occ; // represents the average of occurrences
    float std_dev;//standard deviation associated to avg_occ
    unsigned int obsv_pkts; // number of packets in which the ngram has been observed
    
    float supp;// used for wholesomely auxiliary computations
}ngram_stats;

/* Abstraction for the structure containing ngram-related data*/
typedef struct ngram_data ngram_data;

/*
 creates an instance of the ngram_stats structure.
 
 Returns: an instance of the ngram_stats structure.
*/
ngram_stats* create_ngram_stats();

ngram_data* create_ngram_data( unsigned short chunks, char model, char strategy);

ngram_stats* get_statsofck(ngram_data *ngd, unsigned short ck);

unsigned short usedcks(ngram_data *ngd);

/**
 Add to ngd the statistics pertaining the chunk indexed by ck.
 Note that this operation must not be performed if you are working on
 an instance of ngram_stats that has been previously retrieved by means of the function get_statsofck. 
 In this case, undefined behaviour is expected. The content is updated by means of the
 pointer *stats.
 
 * @param ngd Data structure to update with new statistics.
 * @param ck Index to the chunk of interest.
 * @param stats Statistics computed for the chunk ck.
 */
void set_statsofck(ngram_data *ngd, unsigned short ck, ngram_stats *stats);

ngram_stats* get_statsofdunit(ngram_data *ngd);

void set_statsofdunit(ngram_data *ngd, ngram_stats *stats);

void free_chunks(ngram_data *ngd);

void free_ngram_data(ngram_data *ngd);


#endif /* ngram_data_h */
