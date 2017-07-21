//
//  build_factory.c
//  PCKAD-v2
//
//  Created by Luciano on 10/11/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include "build_factory.h"
#include "mdl_building_strategies.h"
#include "pckad_sysconfig.h"

build_model get_build_model(char strategy)
{
    if(strategy == GS_STR)
        return build_gsmdl;
    
    if(strategy == LS_STR)
        return build_lsmdl;
    
    return build_2lsmdl;
}
