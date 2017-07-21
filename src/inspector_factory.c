//
//  inspector_factory.c
//  PCKAD-v2
//
//  Created by Luciano on 09/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include "inspector_factory.h"
#include "rawdata_inspector.h"
#include "txtdata_inspector.h"

inspect_rawd get_inspector(char format)
{
    if(format == TXT_FORMAT)
        return inspect_txt;
    
    return inspect_pcap;
}
