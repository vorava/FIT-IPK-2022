// IPK projekt 2 - filter.c
// Autor: Vojtěch Orava (xorava02)
#include "filter.h"

char *getFilter(ProgramSettings *ps){
    char *filter = malloc(sizeof(char) * MAX_LEN);
    if(filter == NULL) return NULL;
    memset(filter, 0, MAX_LEN);

    if(ps->protocol != -1){
        char *temp = malloc(sizeof(char)*MAX_LEN);
        if(temp == NULL) return NULL;
        memset(temp, 0, MAX_LEN);

        switch (ps->protocol) {
            case TCP_SET:
                snprintf(temp, MAX_LEN, "tcp");
                break;

            case UDP_SET:
                snprintf(temp, MAX_LEN, "udp");
                break;

            case ARP_SET:
                snprintf(temp, MAX_LEN, "icmp"); //TODO ARP
                break;

            case ICMP_SET:
                snprintf(temp, MAX_LEN, "icmp");
                break;
            default:
                return NULL;
        }

        strncat(filter, temp, MAX_LEN);
        free(temp);
    }

    if(ps->port != -1){
        char *temp = malloc(sizeof(char)*MAX_LEN);
        if(temp == NULL) return NULL;
        memset(temp, 0, MAX_LEN);
        snprintf(temp, MAX_LEN, " port %d", ps->port);
        strncat(filter, temp, MAX_LEN);
        free(temp);
    }

    printf("%s\n", filter);
    return filter;
}

void freeFilter(char *f){
    free(f);
}
