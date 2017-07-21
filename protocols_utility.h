//
//  protocols_utility.h
//  ProtocolsUtility
//
//  Created by Luciano on 19/10/16.
//  Copyright © 2016 Luciano Argento. All rights reserved.
//

#ifndef protocols_utility_h
#define protocols_utility_h


#include <stdlib.h>

//definisco tutto ciò che mi serve per estrarre informazioni sulla macro porzioni
#define CRLF "\r\n"

#define HTTP_PORT 80
#define HTTP_MAX_ELEN 1340// tiene in considerazione la lunghezza massima dell'intestazione TCP, ovverosia 100 byte (60 + 40), e dell'intestazione IP, ovverosia 60 byte
#define HTTP_KW_DEL ' '
#define HTTP_SBPKT_DEL '\n'
#define HTTP_PARAM_DEL '?'
#define LF '\n'
#define CR '\r'
#define HTTP_KW "HTTP"
#define HTTP_KW_LEN 4
#define HTTP_VR0 "/1.0"
#define HTTP_VR0_LEN 4
#define HTTP_VR1 "/1.1"
#define HTTP_VR1_LEN 4
#define GET_KW "GET"
#define GET_KW_LEN 3
#define GET_ID 0
#define QUIT_KW "QUIT"
#define QUIT_KW_LEN 4
#define QUIT_ID 1
#define HEAD_KW "HEAD"
#define HEAD_KW_LEN 4
#define HEAD_ID 2
#define POST_KW "POST"
#define POST_KW_LEN 4
#define POST_ID 3
#define PUT_KW "PUT"
#define PUT_KW_LEN 3
#define PUT_ID 4
#define OPTIONS_KW "OPTIONS"
#define OPTIONS_KW_LEN 7
#define OPTIONS_ID 5
#define DELETE_KW "DELETE"
#define DELETE_KW_LEN 6
#define DELETE_ID 6
#define TRACE_KW "TRACE"
#define TRACE_KW_LEN 5
#define TRACE_ID 7
#define CONNECT_KW "CONNECT"
#define CONNECT_KW_LEN 7
#define CONNECT_ID 8
#define FRAG_ID 9

#define FTP_PORT 21
#define FTP_SBPKT_DEL '\n'
#define FTP_ID 5


#define TCP_ID 6
#define TCP_MaxSS 1460

/*
 Verifica se il metodo specificato in ingresso è presente nel contenuto del pacchetto specificato.
 Restituisce 1 in caso di esito positivo, 0 altrimenti.
 Assunzione: il contenuto corrisponde ad un pacchetto HTTP.
 */
char check_http_method(unsigned char *payload, unsigned short len, char *method, unsigned short meth_len);

/*
 Controlla quale metodo è presente nel contenuto dato in input.
 Restituisce il codice del metodo trovato. Qualora nessun metodo venisse trovato,
 verrebbe restituito il codice che identifica un frammento.
 Assunzione: il contenuto corrisponde ad un pacchetto HTTP.
 */
char get_http_method(unsigned char *payload, unsigned short len);



#endif /* protocols_utility_h */
