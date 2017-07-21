//
//  rawdata_inspector.c
//  PCKAD-v2
//
//  Created by Luciano on 29/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include "rawdata_inspector.h"
#include "netheaders.h"
#include "inspector_if.h"

char inspect_pcap (pckad_pkt *irep, prp_data *prpdt, const unsigned char *packet, unsigned int plen, pckad_sysconfig *sc)
{
    char ret = INSPECTION_INVALID;
    const ethernet_header_t *eth_hdr;
    const ip_header_t *ip_hdr;
    const tcp_header_t *tcp_hdr;
    unsigned int size_ip=0, size_tcp =0;
    protocol_data *pd = NULL;
    global_cnfg *lgc = sc->gc;
    
    eth_hdr= (ethernet_header_t*) (packet);
    ip_hdr=(ip_header_t*) (packet + SIZE_ETHERNET);
    size_ip=IP_HL(ip_hdr)*4;
    tcp_hdr= (tcp_header_t*) (packet + SIZE_ETHERNET+ size_ip);
    
    
    //TODO: manca il controllo sulla consistenza delle lunghezze!
    
    
    for ( char i = 0; i < lgc->nr_protocols; i++)
    {
        pd = &lgc->protocols [i];
        if(pd->dirc)//TODO: stabilire se 1 corrisponde a incoming
        {
            if(ntohs(tcp_hdr->th_dport) == pd->port)
            {
                irep->tcp_port = ntohs(tcp_hdr->th_dport);
                irep->dirc = DEST_PORT;
                ret = INSPECTION_VALID;
            }
        }
        else
        {
            if(ntohs(tcp_hdr->th_sport) == pd->port)
            {
                irep->tcp_port = ntohs(tcp_hdr->th_sport);
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
        
        
        size_tcp = TH_OFF(tcp_hdr)*4;
        
        prpdt->len = plen - (SIZE_ETHERNET + size_ip + size_tcp);
        unsigned int plen_ip = ntohs(ip_hdr->ip_len)-size_ip- size_tcp;
        if (prpdt->len != plen_ip || prpdt->len == 0)
        {
            return INSPECTION_INVALID;
        }
        
        
        prpdt->pkt_payload = (u_char*) malloc(sizeof(u_char) * (prpdt->len+1));
        if(prpdt->pkt_payload == NULL)
        {
            return INSPECTION_OMEM;
        }
        for (u_int k = 0; k < prpdt->len; k++)
        {
            prpdt->pkt_payload[k] = *(packet+ SIZE_ETHERNET + size_ip + size_tcp + k);
        }
    }

    
    return ret;
}



