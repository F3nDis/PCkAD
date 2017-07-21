//
//  cmptas_factory.c
//  PCKAD-v2
//
//  Created by Luciano on 24/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include "as_computation_strategies.h"
#include "cmptas_factory.h"
#include "pckad_sysconfig.h"

compute_as get_compute_as( char strategy, char mechanism)
{
    if(strategy == GS_STR)
        return gsas_ngcomputation;
    
    if(strategy == LS_STR)
        return lsas_ngcomputation;
    
    return twolsas_ngcomputation;
}
