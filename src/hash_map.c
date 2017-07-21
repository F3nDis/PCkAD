//
//  hash_map.c
//  PCkAD
//
//  Created by Luciano on 21/04/15.
//  Copyright (c) 2015 Luciano Argento. All rights reserved.
//


/*
 * Generic map implementation. This class is thread-safe.
 * free() must be invoked when only one thread has access to the hashmap.
 */
#include <stdlib.h>
#include <stdio.h>
#include "hash_map.h"

#define INITIAL_SIZE 20
#define CL_INITIAL_SIZE 1
#define MAX_LOAD_FACTOR 0.75f

// We need to keep keys and values
typedef struct _cell{
    ngram_obj *key;
    any_t data;
} cell;

// inizialmente il puntatore della lista sarà NULL
typedef struct _hashmap_element{
    u_int size;// dimensione della lista dei conflitti
    u_int ffreec;// prima cella libera
    cell *clist;// lista dei conflitti
} hashmap_element;

// A hashmap has some maximum size and current size,
// as well as the data to hold.
typedef struct _hashmap_map{
    int table_size;
    u_int size;
    int entries;// indica il numero di liste di conflitto create
    u_int confl;
    hashmap_element *data;
} hashmap_map;

/*
 * Return an empty hashmap, or NULL on failure.
 */
map_t hashmap_new() {
    hashmap_map* m = (hashmap_map*) malloc(sizeof(hashmap_map));
    if(!m) goto err;
    
    m->data = (hashmap_element*) calloc(INITIAL_SIZE, sizeof(hashmap_element));
    if(!m->data) goto err;
    
    m->table_size = INITIAL_SIZE;
    m->size = 0;
    m->entries = 0;
    m->confl = 0;
    
    return m;
err:
    if (m)
        hashmap_free(m);
    return NULL;
}


map_t hashmap_new2(u_int tsize) {
    hashmap_map* m = (hashmap_map*) malloc(sizeof(hashmap_map));
    if(!m) goto err;
    
    m->data = (hashmap_element*) calloc(tsize, sizeof(hashmap_element));
    if(!m->data) goto err;
    
    m->table_size = tsize;
    m->size = 0;
    m->entries = 0;
    m->confl = 0;
    
    return m;
err:
    if (m)
        hashmap_free(m);
    return NULL;
}

/*
 * Hashing function for an integer
 */
unsigned int hashmap_hash_int(hashmap_map * m,unsigned  int key){
    /* Robert Jenkins' 32 bit Mix Function */
    key += (key << 12);
    key ^= (key >> 22);
    key += (key << 4);
    key ^= (key >> 9);
    key += (key << 10);
    key ^= (key >> 2);
    key += (key << 7);
    key ^= (key >> 12);
    
    /* Knuth's Multiplicative Method */
    key = (key >> 3) * 2654435761;
    
    return key % m->table_size;
}

/*
 * Hashing function for an n-gram
 */
unsigned int hashmap_hash_ngram(hashmap_map * m,unsigned  char *key, unsigned short n){
    unsigned int offset_basis = 2166136261;
    unsigned int fnv_prime = 16777619;
    unsigned int hash = offset_basis;
    // unsigned int hash = 1;
    
    for (unsigned short  i = 0; i < n; i++)
    {
        // printf(" %c - %d\n", key [i], key [i]);
        // printf("%c", key [i]);
        //  hash += 281 *hash+ key[i];
        
        
        hash ^= key[i];
        hash *= fnv_prime;
        // hash += fPassonv_prime;
    }
    // printf("\t-\t%u\n", hash);
    
    // return hashmap_hash_int(m, hash);
    return hash % m->table_size;
}

/*
 * Return the integer of the location in data
 * to store the point to the item, or MAP_FULL.
 */
unsigned int hashmap_hash(map_t in, ngram_obj *key){
    return MAP_FULL;
}

/*
 * Doubles the size of a clist
 */
int hashmap_double_clist(hashmap_element *elem){
    elem->size = elem->size * 2;
    cell *new_cl = (cell*) calloc(elem->size,  sizeof(cell));
    if(!new_cl)
    {
        return MAP_OMEM;
    }
    
    for (u_int i = 0; i < elem->ffreec; i++)
    {
        new_cl[i].data = elem->clist [i].data;
        new_cl[i].key = elem->clist [i].key;
    }
    free(elem->clist);
    elem->clist = new_cl;
    
    
    return MAP_OK;
}

unsigned int hashmap_rehash(map_t in)
{
    hashmap_map* m;
    
    /* Cast the hashmap */
    m = (hashmap_map *) in;
    
    hashmap_map *newmap = (hashmap_map *) hashmap_new2(m->table_size * 2);
    if(newmap == NULL)
        return MAP_OMEM;
    
    newmap->table_size = m->table_size * 2;
    
    ngram_obj **keys = hashmap_keys(m);
    
    any_t *item = (any_t*)malloc(sizeof(any_t));
    if(keys == NULL || item == NULL)
    {
        free(newmap);
        return MAP_OMEM;
    }
    
    /*inserisco le chiavi della vecchia mappa nella nuova*/
    u_int i = 0;
    ngram_obj *tmk = NULL;
    for (; i < m->size; i++)
    {
        tmk = keys[i];
        hashmap_get(m, tmk, item);
        hashmap_put(newmap, tmk, *item);
    }
    
    /*rilascia la memoria della sola mappa*/
    i = 0;
    while(i < m->table_size)
    {
        if(m->data[i].clist != NULL)
        {
            free(m->data[i].clist);
        }
        i ++;
    }
    free(m->data);
    
    m->data = newmap->data;
    m->table_size = newmap->table_size;
    m->entries = newmap->entries;// non è detto che le stesse liste di conflitto siano state create
    
    free(newmap);
    
    free(item);
    free(keys);
    
    return MAP_OK;
}

/*
 * Add a pointer to the hashmap with some key
 */
int hashmap_put(map_t in, ngram_obj *key, any_t value){
    unsigned int index;
    hashmap_map* m;
    
    /* Cast the hashmap */
    m = (hashmap_map *) in;
    
    /* Controlla LF */
    if(m->entries/(float)m->table_size >= MAX_LOAD_FACTOR )
    {
        // printf(" prima: totale = %u\noccupato = %u\n", m->table_size, m->entries);
        hashmap_rehash(m);
        // printf(" dopo: totale = %u\noccupato = %u\n", m->table_size, m->entries);
    }
    
    /* Find a place to put our value */
    index = hashmap_hash_ngram(m, key->ngram, key->n);
    // printf("%u\n", index);
    
    /* Set the data */
    if(m->data[index].clist == NULL)
    {
        // nessuna chiave era mai stata inserita in questa posizione
        m->data[index].clist = (cell*) calloc(CL_INITIAL_SIZE, sizeof(cell));
        m->data[index].size = CL_INITIAL_SIZE;
        m->data[index].ffreec = 0;
        
        m->entries ++;
    }
    else if(m->data[index].ffreec == m->data[index].size)
    {
        //TODO: bisogna raddoppiare la dimensione della lista dei conflitti
        if(hashmap_double_clist(&m->data [index]) == MAP_OMEM)
        {
            return MAP_OMEM;
        }
        // printf("f = %u - s = %u\n", m->data[index].ffreec, m->data[index].size);
    }
    
    // il puntatore alla prossima cena libera è già impostato
    cell *c = m->data[index].clist;
    c[m->data[index].ffreec].data = value;
    c[m->data[index].ffreec].key = key;
    m->data[index].ffreec ++;
    m->size++;
    
    
    
    return MAP_OK;
}

/*
 * Get your pointer out of the hashmap with a key
 */
int hashmap_get(map_t in, ngram_obj *key, any_t *arg){
    unsigned int index;
    hashmap_map* m;
    char found = 1;
    
    /* Cast the hashmap */
    m = (hashmap_map *) in;
    
    
    
    /* Find data location */
    index = hashmap_hash_ngram(m, key->ngram, key->n);
    
    /* Linear probing, if necessary */
    cell *cl = m->data[index].clist;
    
    hashmap_element *elem = &m->data[index];
    if(m->data[index].clist == NULL)
    {
        // nessuna chiave era mai stata inserita in questa posizione
        return MAP_MISSING;
    }
    else
    {
        for (u_int i = 0; i < m->data[index].ffreec; i++)
        {
            found = 1;
            cl = &elem->clist [i];
            for ( unsigned short j = 0; j < key->n; j++)
            {
                if(key->ngram [j] != cl->key->ngram [j])
                {
                    found = 0;
                    break;
                }
            }
            if(found)
            {
                *arg = m->data[index].clist [i].data;
                break;
            }
        }
    }
    
    
    if(found)
    {
        return MAP_OK;
    }
    
    *arg = NULL;
        
    
    /* Not found */
    return MAP_MISSING;
}

/*
 * Get a random element from the hashmap
 */
int hashmap_get_one(map_t in, any_t *arg, int remove){
    
    return MAP_MISSING;
}

/*
 * Iterate the function parameter over each element in the hashmap.  The
 * additional any_t argument is passed to the function as its first
 * argument and the hashmap element is the second.
 */
int hashmap_iterate_by_item(map_t in, PFany f, any_t item) {
    
    return MAP_MISSING;
}

/*
 */
extern int hashmap_iterate(map_t in, any_t *item)
{
    static u_int ex = 0, i = 0;// indice esterno, indice interno
    
    /* Cast the hashmap */
    hashmap_map* m = (hashmap_map*) in;
    
    /* On empty hashmap, return immediately */
    if (hashmap_length(m) <= 0)
        return MAP_MISSING;
    
    /* Lock for concurrency */
    
    /* Linear probing */
    while(ex < m->table_size)
    {
        //TODO: da completare
        if(m->data[ex].clist != NULL)
        {
            if(i < m->data[ex].ffreec)
            {
                *item = m->data[ex].clist [i].data;
                i++;
                return MAP_OK;
            }
            else
            {
                i = 0;
            }
        }
        ex++;
    }
    
    /* Unlock */
    ex = 0;
    i = 0;
    return MAP_MISSING;
}

/*
 */
extern ngram_obj ** hashmap_keys(map_t in)
{
    hashmap_map* m = (hashmap_map*) in;
    
    if (hashmap_length(m) <= 0)
        return NULL;
    ngram_obj **keys = (ngram_obj**) calloc(m->size, sizeof(ngram_obj*));
    
    int i = 0;
    unsigned k = 0;
    ngram_obj *tmk = NULL;
    while(i < m->table_size)
    {
        if(m->data[i].clist != NULL)
        {
            for (u_int j = 0; j < m->data[i].ffreec; j++)
            {
                tmk = m->data[i].clist[j].key;
                
                keys[k] = tmk;
                k ++;
            }
        }
        i ++;
    }
    
    return keys;
}

/*
 * Remove an element with that key from the map
 */
int hashmap_remove(map_t in, ngram_obj *key, any_t *arg)
{
    int ri = 0;// ri: indice di rimozione
    unsigned int index;
    hashmap_map* m;
    char found = 0;
    
    /* Cast the hashmap */
    m = (hashmap_map *) in;
    
    /* Lock for concurrency */
    
    /* Find key */
    index = hashmap_hash_ngram(m, key->ngram, key->n);
    
    
    cell *cl = NULL;
    hashmap_element *elem = &m->data[index];
    if(m->data[index].clist == NULL)
    {
        // nessuna chiave era mai stata inserita in questa posizione
        *arg = NULL;
        return MAP_MISSING;
    }
    else
    {
        for (u_int i = 0; i < m->data[index].ffreec; i++)
        {
            cl = &elem->clist [i];
            for ( unsigned short j = 0; j < key->n; j++)
            {
                if(key->ngram [j] != cl->key->ngram [j])
                {
                    break;
                }
                else if(j == key->n -1)
                {
                    ri = i;
                    found = 1;
                }
            }
            if(found)
            {
                *arg = elem->clist [i].data;// deve essere rilasciata la memoria dall'esterno
                break;
            }
        }
    }
    
    if(found)
    {
        // rimozione dell'elemento
        
        if(ri == 0)//CASE 1: ultimo elemento rimasto
        {
            free(m->data[index].clist [ri].key);
            m->data[index].clist = NULL;
            m->data[index].ffreec = 0;
            m->size--;
            m->entries--;
        }
        else if(ri == m->data[index].ffreec-1)//CASE 2: l'elemento si trova nell'ultima posizione occupata
        {
            free(m->data[index].clist [ri].key);
            m->data[index].ffreec--;
            m->size--;
        }
        else if(ri < m->data[index].ffreec-1)//CASE 3: l'elemento si trova in una posizione interna dell'array
        {
            free(m->data[index].clist [ri].key);
            m->size--;
            
            for (u_int j = ri+1; j < m->data[index].ffreec; j++)
            {
                m->data[index].clist [j-1] = m->data[index].clist [j];
            }
            m->data[index].ffreec--;
        }
        return MAP_OK;
    }
    
    *arg = NULL;
    
    /* Data not found */
    return MAP_MISSING;
}

/* Deallocate the hashmap
 I valori devono essere deallocati dall'esterno.
 */
void hashmap_free(map_t in){
    hashmap_map* m = (hashmap_map*) in;
    int i = 0;
    while(i < m->table_size)
    {
        if(m->data[i].clist != NULL)
        {
            for (u_int j = 0; j < m->data[i].ffreec; j++)
            {
                free(m->data[i].clist [j].key->ngram);
                free(m->data[i].clist [j].key);
            }
            free(m->data[i].clist);
        }
        i ++;
    }
    free(m->data);
    free(m);
}

/* Return the length of the hashmap */
int hashmap_length(map_t in){
    hashmap_map* m = (hashmap_map *) in;
    // printf(" dimensione tabella: %u\n", m->table_size);
    if(m != NULL) return m->size;
    else return 0;
}

/*
 * Get the current conflicts
 */
extern int hashmap_conflicts(map_t in)
{
    hashmap_map* m = (hashmap_map *) in;
    if(m != NULL) return m->confl;
    else return 0;
}


void hashmap_check_conflicts(map_t in)
{
    hashmap_map* m = (hashmap_map*) in;
    int i = 0;
    while(i < m->table_size)
    {
        if(m->data[i].clist != NULL)
        {
            printf("%d = %u\n", i, m->data[i].ffreec);
        }
        i ++;
    }
}









