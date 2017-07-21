//
//  pckad_observer.c
//  PCKAD-v2
//
//  Created by Luciano on 14/12/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "pckad_observer.h"
#include "console_client.h"

struct console_observer
{
    char *last_event;
};

void notified_event (void *instance, const char *event)
{
    printf("\n%s\n", event);
}

console_obsv* create_console_obsv()
{
    console_obsv *p = (console_obsv*) malloc(sizeof(console_obsv));
    if(p != NULL)
    {
        p->last_event = NULL;
    }
    return p;
}

void free_console_obsv(console_obsv* cobsv)
{
    if(cobsv->last_event != NULL)
    {
        free(cobsv->last_event);
    }
    free(cobsv);
}

pckad_observer* create_pckad_observer (void *instance, notify_event notify)
{
    pckad_observer *p = (pckad_observer*) malloc(sizeof(pckad_observer));
    if(p != NULL)
    {
        p->instance = instance;
        p->notify = notify;
    }
    return p;
}

void free_pckad_observer(pckad_observer* obsv)
{
    if(obsv->instance != NULL)
    {
        console_obsv *co = (console_obsv*) obsv->instance;
        free_console_obsv(co);
    }
    free(obsv);
}






























