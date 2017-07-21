//
//  http_pkt_txtconverter.h
//  PCKAD-v2
//
//  Created by Luciano on 08/04/17.
//  Copyright © 2017 Luciano Argento. All rights reserved.
//

#ifndef http_pkt_txtconverter_h
#define http_pkt_txtconverter_h

#include <stdio.h>

/*
 convert_http_pkts:
 =============
 legge la rappresentazione testuale di pacchetti HTTP contenuti
 in un file testuale f. 
 A ciascun contenuto viene associata una intestazione fittizia.
 
 @f: il file testuale da cui leggere i pacchetti HTTP.
 @src: indica se la porta http corrisponde alla sorgente (0) o destinazione (1).
 @rlen: puntatore alla lunghezza del risultato. Deve essere inizializzato.
 
 returns: vettore di puntatori a vettori di tipo unsigned char, rappresentanti
 il contenuto dei pacchetti. Ciascuno di questi vettore contiene un carattere nullo
 in corrispondenza dell'ultima cella, pertanto non viene restituita la lunghezza 
 rispondente.
 */
unsigned char** convert_http_pkts(FILE *f, char src, unsigned int *rlen);

#endif /* http_pkt_txtconverter_h */
