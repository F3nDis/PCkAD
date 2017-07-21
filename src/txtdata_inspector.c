//
//  txtdata_inspector.c
//  PCKAD-v2
//
//  Created by Luciano on 08/04/17.
//  Copyright © 2017 Luciano Argento. All rights reserved.
//

#include "txtdata_inspector.h"
#include "inspector_if.h"

#define SIZE_TXTHEADER 7

/* Txt header */
typedef struct txt_header
{
    unsigned int len; /* payload length */
    unsigned short port; /* tcp or udp port */
    unsigned char src; /* 1 source, 0 destination */
}txt_header_t;

char inspect_txt (pckad_pkt *irep, prp_data *prpdt, const unsigned char *pkt, unsigned int plen, pckad_sysconfig *sc)
{
    char ret = INSPECTION_INVALID;
    txt_header_t *hdr;
    protocol_data *pd = NULL;
    
    
    hdr = (txt_header_t*) (pkt);
    
    for ( char i = 0; i < sc->gc->nr_protocols; i++)
    {
        pd = &sc->gc->protocols [i];
        if(pd->dirc)//1 corrisponde a incoming
        {
            if(!hdr->src && hdr->port == pd->port)
            {
                irep->tcp_port = hdr->port;
                irep->dirc = DEST_PORT;
                ret = INSPECTION_VALID;
            }
        }
        else
        {
            if(hdr->src && hdr->port == pd->port)
            {
                irep->tcp_port = hdr->port;
                irep->dirc = SRC_PORT;
                ret = INSPECTION_VALID;
            }
        }
    }
    
    
    
    // posso inizializzare la struttura dati per il pre elaboratore
    if(ret == INSPECTION_VALID)
    {
        prpdt->port = irep->tcp_port;
        prpdt->dirc = irep->dirc;
        
        prpdt->len = hdr->len - SIZE_TXTHEADER;
        
        
        prpdt->pkt_payload = (u_char*) malloc(sizeof(u_char) * (prpdt->len+1));
        if(prpdt->pkt_payload == NULL)
        {
            return INSPECTION_OMEM;
        }
        for (u_int k = 0; k < prpdt->len; k++)
        {
            prpdt->pkt_payload[k] = *(pkt+ SIZE_TXTHEADER + k);
        }
    }
    
    
    return ret;
}
