//
//  inspector_testing.c
//  PCKAD-v2
//
//  Created by Luciano on 05/12/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <pcap.h>
#include "inspector_testing.h"
#include "rawdata_inspector.h"
#include "cnfg_reader_director.h"
#include "pckad_sysconfig.h"

char inspect_pcap_test( const char *cnfg_path, const char *data_path)
{
    int *rcd = ( int*) malloc(sizeof( int));
    pckad_sysconfig *scnfg = build_config(cnfg_path, rcd);
    
    struct pcap_pkthdr header; // The header that pcap gives us
    const u_char *packet; // The actual packet
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    handle = pcap_open_offline(data_path, errbuf);   //call pcap library function
    //se non riesco ad aprire il dataset allora faccio terminare il programma con errore
    if (handle == NULL)
    {
        fprintf(stderr,"Couldn't open pcap file %s: %s\n", data_path, errbuf);
        
        exit (EXIT_FAILURE);
    }
    
    
    while ((packet = pcap_next(handle, &header)))
    {
        prp_data *prpdt = create_prp_data();
        pckad_pkt *ppkt = pckad_pkt_dinit();
        char val = inspect_pcap(ppkt, prpdt, packet, header.len, scnfg);
        
        printf("val == %d\n",val);
        free_pckad_pkt(ppkt);
        free_prp_data(prpdt);
    }
    
    pcap_close(handle);  //close the pcap file
    
    free_pckad_sysconfig(scnfg);
    free(rcd);
    return 0;
}
