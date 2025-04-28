// inclusions libraries
#include "server.h"
#include <string.h>
#include "pico/stdlib.h"


// tcp_pcb struct to stablish connection
struct tcp_pcb *pcb = NULL;

// variables and definition to control connetion attemps
bool have_connection = false;
#define MAX_RETRIES 5
int retries = 0;

// implementation functions

// Callback for server response
err_t server_tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        printf("Connection closed by the server.\n");
        tcp_close(tpcb);
        have_connection = false;
        pcb = NULL;
        return ERR_OK;
    } else {
        printf("Reply received: %s\n", (char*)p->payload);
        tcp_recved(tpcb, p->tot_len);
        pbuf_free(p);
        return ERR_OK;
    }
}

// Callback para erro na conexão TCP
void server_tcp_client_error(void *arg, err_t err) {
    printf("TCP connection error (%d). Trying to reconnect..\n", err);
    pcb = NULL;
    have_connection = false;

    if (retries < MAX_RETRIES) {
        server_create_tcp_connection();
    } else {
        printf("Maximum number of attempts reached. Aborting.\n");
    }
}

// Close tcp connection
void server_close_tcp_connection() {
    if (pcb != NULL) {
        tcp_close(pcb);
        pcb = NULL;
        have_connection = false;
        printf("TCP connection closed.\n");
    }
}

void server_create_tcp_connection() {
    if (pcb != NULL) return;    // connection already exists
    retries++;

    // server config
    ip_addr_t server_ip;
    server_ip.addr = ipaddr_addr(SERVER_IP);

    // creating pcb
    pcb = tcp_new();
    if (!pcb) {
        printf("Failed to create pcb\n");
        return;
    }

    tcp_err(pcb, server_tcp_client_error);  // register callback of error

    // Try connecting to the server
    err_t connect_err = tcp_connect(pcb, &server_ip, SERVER_PORT, NULL);
    if (connect_err == ERR_OK) {
        printf("Conected to server!\n");
        have_connection = true;
        tcp_recv(pcb, server_tcp_client_recv);
    } else {
        printf("Connection failed (%d). Attempted %d/%d\n", connect_err, retries, MAX_RETRIES);
        tcp_abort(pcb);
        pcb = NULL;
    }
}

// Sending data to the server
void server_send_data_to_server(int button_state, float distance) {
    while (pcb == NULL || !have_connection) {
        if (retries >= MAX_RETRIES) {
            printf("Error: Connection could not be re-established after %d attempts.\n", MAX_RETRIES);
            return;  // Aborts if the maximum number of reconnection attempts is reached
        }

        printf("Connection lost. Trying to reconnect... (%d/%d)\n", retries, MAX_RETRIES);
        server_create_tcp_connection();  // Attempts to reconnect
        sleep_ms(1000);
    }

    // Send a request to the server
    retries = 0;

    // preparing the body of the requisition
    char request_body[256];
    snprintf(request_body, sizeof(request_body),
    "{\n"
    "\"buttonState\" : \"%d\",\n"
    "\"distanceValue\" : \"%.2f\"\n"
    "}", button_state, distance);

    // preparing the request
    char request[256];
    snprintf(request, sizeof(request), 
    "POST /api/sensor/send HTTP/1.1\r\n"
    "Host: %s\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: %d\r\n"
    "\r\n"
    "%s"
    "\r\n",
    SERVER_IP, strlen(request_body), request_body);

    // sending a request to the server
    if (tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY) != ERR_OK) {
        printf("Error sending data. Closing connection...\n");
        tcp_abort(pcb);
        pcb = NULL;
        have_connection = false;
        return;
    }

    if (tcp_output(pcb) != ERR_OK) {
        printf("Eerror when sending data (tcp_output). Closing connection...\n");
        tcp_abort(pcb);
        pcb = NULL;
        have_connection = false;
        return;
    }
}
