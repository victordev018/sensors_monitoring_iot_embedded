#ifndef SERVER_H
#define SERVER_H

// inclusion of libraries
#include "lwip/tcp.h"
#include "lwip/dns.h"

// server config
#define SERVER_IP "35.212.15.0"
#define SERVER_PORT 42539

// definitions functions
void server_create_tcp_connection();
void server_close_tcp_connection();
void server_tcp_client_error(void *arg, err_t err);
err_t server_tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void server_send_data_to_server(int button_state, float distance);

#endif
