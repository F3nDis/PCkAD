//
//  CnfgReader.h
//  PCKAD-v2
//
//  Created by Luciano on 03/10/16.
//  Copyright (c) 2016 Luciano Argento. All rights reserved.
//

#ifndef PCKAD_v2_CnfgReader_h
#define PCKAD_v2_CnfgReader_h

#include <stdio.h>
#include <libxml/xmlreader.h>

#include "pckad_sysconfig.h"

#define CR_OK 0
#define CR_UNABLE_PARSE 1
#define CR_UNABLE_OPEN 2

/*
 Read the content of the global configuration file whose path is specified by filename.
 It takes in input an instance of the global_cnfg structure. gcnfg's protocols
 must not be initialised.
 
 @filename: global configuration file path.
 @gcnfg: An instance of the global_cnfg structure.
 
 returns: code the representing the operation results.
 */
int stream_gcfile(const char *filename, global_cnfg *gcnfg);

/*
 Read the content of the training configuration file whose path is specified by filename.
 It takes in input an instance of the training_cnfg structure.
 
 @filename: training configuration file path.
 @gcnfg: An instance of the training_cnfg structure.
 
 returns: code the representing the operation results.
 */
int stream_tcfile(const char *filename, training_cnfg *tcnfg);

/*
 Read the content of the detection configuration file whose path is specified by filename.
 It takes in input an instance of the detection_cnfg structure.
 
 @filename: detection configuration file path.
 @gcnfg: An instance of the detection_cnfg structure.
 
 returns: code the representing the operation results.
 */
int stream_dcfile(const char *filename, detection_cnfg *dcnfg);

#endif
