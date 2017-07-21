//
//  pckad_observer.h
//  PCKAD-v2
//
//  Created by Luciano on 14/12/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef pckad_observer_h
#define pckad_observer_h

typedef void (*notify_event) (void *instance, const char *event);

typedef struct pckad_observer
{
    void *instance;
    notify_event notify;
}pckad_observer;

pckad_observer* create_pckad_observer (void *instance, notify_event notify);

void free_pckad_observer(pckad_observer* obsv);

#endif /* pckad_observer_h */
