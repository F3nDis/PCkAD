//
//  ngclust_data.h
//  ngclust_data
//
//  Created by Luciano on 12/06/17.
//  Copyright © 2017 Luciano Argento. All rights reserved.
//

#ifndef ngclust_data_h
#define ngclust_data_h


typedef struct ngclust_data
{
    unsigned char *string;
    float freq;
    unsigned int len;
}ngclust_data;

/**
 * Crea una struttura ngclust_data vuota.
 *
 * @return Il puntatore a ngclust_data, NULL se dovesse fallire l'operazione.
 */
ngclust_data* create_ngclust_data();

/**
 * Libera la memoria riservata per kd.
 *
 * @param kd Struttura di cui si vuole rilasciare la memoria.
 *
 */
void free_ngclust_data(ngclust_data *kd);

/**
 * Restituisce il puntatore alla stringa contenuta in kd.
 *
 * @param kd Struttura di cui si vuole restituire il puntatore alla stringa che contiene.
 *
 * @return Puntatore della stringa contenuta in kd.
 */
unsigned char* get_kdstring(ngclust_data *kd);

/**
 * Restituisce un decimale, contenente informazioni statistiche
 * sulla stringa contenuta in kd.
 *
 * @param kd Struttura di cui si vuole restituire il decimale che contiene.
 *
 * @return Decimale contenuto in kd.
 */
float get_kdstat(ngclust_data *kd);

/**
 * Imposta il puntatore alla stringa, contenuta in kd, con str.
 *
 * @param kd Struttura di cui si vuole restituire il puntatore alla stringa che contiene.
 * @param str Puntatore con cui inizializzare la stringa contenuta in kd.
 *
 */
void set_kdstring(ngclust_data *kd, unsigned char* str);

/**
 * Imposta il valore della statistica contenuta in kd.
 *
 * @param kd Struttura di cui si vuole restituire il decimale che contiene.
 * @param st Valore della statistica con cui inizializzare il decimale in kd.
 *
 */
void set_kdstat(ngclust_data *kd, float st);








#endif /* ngclust_data_h */
