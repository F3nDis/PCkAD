//
//  http_pkt_txtconverter.c
//  PCKAD-v2
//
//  Created by Luciano on 08/04/17.
//  Copyright © 2017 Luciano Argento. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "http_pkt_txtconverter.h" 

#define L_INIT_LEN 10000 /*dimensione iniziale del vettore contenente tutti i pacchetti letti*/
#define P_INIT_LEN 3000 /*dimensione iniziale del vettore riservato per il pacchetto corrente*/

#define HTTP_PORT 80
#define FLAG_IDX 6
#define PORT_LEN 2
#define LEN_LEN 4
#define TXT_HEADER_LEN 7

#define GET_KW "GET"
#define GET_LEN 3
#define POST_KW "POST"
#define POST_LEN 4


typedef union u_int_or_bytes
{
    u_int num;
    char i_ch[sizeof(u_int)];
}u_int_or_bytes;

typedef union u_short_or_bytes
{
    u_short num;
    char s_ch[sizeof(u_short)];
}u_short_or_bytes;


static unsigned char** ralloc_l (unsigned char **l, unsigned int llen, unsigned int newlen);
static unsigned char* ralloc_p (unsigned char *p, unsigned int plen, unsigned int newlen);
static char check_line (char * line, size_t len);

unsigned char** convert_http_pkts(FILE *f, char src, unsigned int *rlen)
{
    unsigned char **l = ( unsigned char**) calloc(L_INIT_LEN, sizeof( unsigned char*));
    unsigned char **ltmp = NULL;
    unsigned char *p = ( unsigned char*) calloc(P_INIT_LEN, sizeof( unsigned char));
    unsigned char *ptmp = NULL;
    unsigned int llen = L_INIT_LEN;// lunghezza corrente
    unsigned int plen = P_INIT_LEN;
    unsigned int lcnt = 0;// prossima cella da utilizzare
    unsigned int pcnt = 0;
    
    u_int_or_bytes uib;
    u_short_or_bytes usb;
    usb.num = HTTP_PORT;
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    // reads text until newline
    //TODO: it seems that getline is specific to GNU libc, not portable!
    while ((read = getline(&line, &len, f)) != -1)
    {
        if(pcnt == 0)// primissimo pacchetto
        {
            // S: costruisco l'intestazione
            for ( unsigned short i = LEN_LEN; i < LEN_LEN + PORT_LEN; i++)
            {
                p[ i ] = usb.s_ch[ i - LEN_LEN ];
            }
            p [ FLAG_IDX ] = src;
            pcnt += TXT_HEADER_LEN;
            // E: intestazione
            
            if(read >= plen -1 - pcnt)// una cella deve essere riservata per \0
            {
                ptmp = ralloc_p(p, plen, plen*2);// allocare nuova memoria per p
                free(p);
                p = ptmp;
                ptmp = NULL;
                plen = plen*2;
            }
            memcpy(p+pcnt,line,read);
            pcnt += read;// punta alla prossima cella disponibile
            continue;
        }
        
        if(check_line(line, len))// controllare se si tratta di un nuovo pacchetto
        {
            if(lcnt >= llen)
            {
                ltmp = ralloc_l(l, llen, llen*2);// allocare nuova memoria per l
                free(l);
                l = ltmp;
                ltmp = NULL;
                llen = llen*2;
            }
            
            if(pcnt < plen-1)//ci sono celle inutilizzate
            {
                ptmp = ralloc_p(p, plen, pcnt+1);
                free(p);
                p = ptmp;
                ptmp = NULL;
                plen = pcnt+1;// +1 per il carattere nullo
            }
            p[ pcnt ] = '\0';
            
            uib.num = plen;
            for ( unsigned int i = 0; i < LEN_LEN; i++)
            {
                p[ i ] = uib.i_ch[ i ];
            }
            
            l[ lcnt ++ ] = p;
            
            
            // costruisco un nuovo pacchetto
            p = ( unsigned char*) calloc(P_INIT_LEN, sizeof( unsigned char));
            plen = P_INIT_LEN;
            pcnt = 0;
            
            
            // S: costruisco l'intestazione
            for ( unsigned short i = LEN_LEN; i < LEN_LEN + PORT_LEN; i++)
            {
                p[ i ] = usb.s_ch[ i - LEN_LEN];
            }
            p [ FLAG_IDX ] = src;
            pcnt += TXT_HEADER_LEN;
            // E: intestazione
            
            
            if(read >= plen -1 - pcnt)// una cella deve essere riservata per \0
            {
                ptmp = ralloc_p(p, plen, plen*2);// allocare nuova memoria per p
                free(p);
                p = ptmp;
                ptmp = NULL;
                plen = plen*2;
            }
            memcpy(p+pcnt,line,read);
            pcnt += read;// punta alla prossima cella disponibile
        }
        else // costruzione in corso
        {
            if(read >= plen -1 - pcnt)// una cella deve essere riservata per \0
            {
                ptmp = ralloc_p(p, plen, plen*2);// allocare nuova memoria per p
                free(p);
                p = ptmp;
                ptmp = NULL;
                plen = plen*2;
            }
            memcpy(p+pcnt,line,read);
            pcnt += read;// punta alla prossima cella disponibile
        }
    }
    
    if(lcnt >= llen)// gestisco l'ultimo pacchetto rimasto
    {
        ltmp = ralloc_l(l, llen, llen+1);// allocare una nuova cella per l
        free(l);
        l = ltmp;
        ltmp = NULL;
        llen = llen+1;
    }
    
    if(pcnt < plen-1)//ci sono celle inutilizzate
    {
        ptmp = ralloc_p(p, plen, pcnt+1);
        free(p);
        p = ptmp;
        ptmp = NULL;
        plen = pcnt+1;// +1 per il carattere nullo
    }
    p[ pcnt ] = '\0';
    
    uib.num = plen;
    for ( unsigned int i = 0; i < LEN_LEN; i++)
    {
        p[ i ] = uib.i_ch[ i ];
    }
    
    l[ lcnt ++ ] = p;
    *rlen = lcnt;
    
    
    return l;
}

static char check_line (char * line, size_t len)
{
    if(memcmp(line, GET_KW, GET_LEN) == 0)
        return 1;
    
    return memcmp(line, POST_KW, POST_LEN) == 0;
}

static unsigned char* ralloc_p (unsigned char *p, unsigned int plen, unsigned int newlen)
{
    unsigned char *np = ( unsigned char*) calloc(newlen, sizeof( unsigned char));
    
    unsigned int lim = (newlen > plen)? plen : newlen;
    
    for ( unsigned int i = 0; i < lim; i++)
    {
        np[ i ] = p[ i ];
    }
    
    return np;
}

static unsigned char** ralloc_l (unsigned char **l, unsigned int llen, unsigned int newlen)
{
    unsigned char **nl = ( unsigned char**) calloc(newlen, sizeof( unsigned char*));
    
    unsigned int lim = (newlen > llen)? llen : newlen;
    
    for ( unsigned int i = 0; i < lim; i++)
    {
        nl[ i ] = l[ i ];
    }
    
    return nl;
}



























