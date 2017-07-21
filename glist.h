//
//  glist.h
//  PCKAD-v2
//
//  Created by Luciano on 27/09/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_glist_h
#define PCKAD_v2_glist_h

#include <stdlib.h>
#include "arraylcore.h"

#define FREE_OK 2
#define FREE_NA 3

glist* new_glist(size_t data_size);

glist* create_glist(size_t data_size, unsigned int idim);

/**
 * Rilascia la memoria allocata per list. Non viene tuttavia rilasciata la memoria
 * degli elementi contenuti in list!
 *
 * @param list    lista di quelli lasciare la memoria.
 *
 * @return        restituisce all'intero rappresentante un codice di ritorno (FREE_OK in caso di esito positivo).
 */
int destroy_glist(glist *list);

unsigned int add_first(glist *list, void *gdata);

unsigned int add_last(glist *list, void *gdata);

unsigned int add_to_slist(glist *list, void *gdata, char compare (void *, void*));

unsigned int set_data(glist *list, void *gdata, unsigned int i);

void* remove_first(glist *list);

/**
 * Rimuove l'ultimo elemento da list e ne restituisce il puntatore.
 * Restituisce NULL se list non contiene alcun elemento.
 *
 * @param list    lista da cui rimuovere l'ultimo elemento, se presente.
 *
 * @return        restituisce il puntatore all'ultimo elemento della lista list. NULL se list non contiene alcun elemento.
 */
void* remove_last(glist *list);

void* remove_data(glist *list, unsigned int i);

void* get_first(glist *list);

void* get_last(glist *list);

void* get_data(glist *list, unsigned int i);

#endif
