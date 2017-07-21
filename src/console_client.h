//
//  console_client.h
//  PCKAD-v2
//
//  Created by Luciano on 14/12/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef console_client_h
#define console_client_h

typedef struct console_observer console_obsv;

void notified_event (void *instance, const char *event);

console_obsv* create_console_obsv();

void free_console_obsv(console_obsv* cobsv);

#endif /* console_client_h */
