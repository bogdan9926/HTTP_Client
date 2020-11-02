#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"


char *compute_get_request(char *host, char *url,
                            char *cookies, char *token,int ok_cookie, int ok_token)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol formathost_ip
    if (ok_cookie==1) {
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);
    // add token
    }else if(ok_token==1) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    memset(line, 0, LINELEN);
    free(line);
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data, char *cookies, char *token)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */

    sprintf(line, "Content-Type: %s",content_type);
    compute_message(message, line);

    int length = strlen(body_data);

    sprintf(line, "Content-Length: %d", length);
    compute_message(message, line);

    // Step 4 (optional): add cookies

    if (cookies != NULL) {
           sprintf(line, "Cookie: %s", cookies);
           compute_message(message, line);
       }
    //add token
    if (token != NULL) {
           sprintf(line, "Authorization: Bearer %s", token);
           compute_message(message, line);
       }
    
    // Step 5: add new line at end of header

    compute_message(message, "");
    
    // Step 6: add the actual payload data

    compute_message(message, body_data);

    memset(line, 0, LINELEN);
    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *token)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add token
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    // Step 3: add final new line
    compute_message(message, "");
    memset(line, 0, LINELEN);
    free(line);
    return message;
}



