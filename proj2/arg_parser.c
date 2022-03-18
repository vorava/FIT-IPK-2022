// IPK projekt 2 - arg_parser.c
// Autor: Vojtěch Orava (xorava02)
#include "arg_parser.h"

ProgramSettings *parseArguments(int argc, char *argv[]){

    ProgramSettings *ps = malloc(sizeof(ProgramSettings));
    if(ps == NULL) return NULL;
    strcpy(ps->interface, "NOT_SET");
    ps->port = -1;
    ps->numberOfPackets = 1;
    ps->protocol = -1;

    struct option longOptions[] = {
        {"interface", required_argument, NULL, 'i'},
        {"help", no_argument, NULL, 'h'},
        {"tcp", no_argument, NULL, 't'},
        {"udp", no_argument, NULL, 'u'},
        {"arp", no_argument, NULL, ARPARG},
        {"icmp", no_argument, NULL, ICMPARG},
        {0, 0, 0, 0}
    };

    char *shortOptions = "hi:tup:n:";
    int optionIndex = 0;
    char c;
    char *temp;

    while ((c = getopt_long(argc, argv, shortOptions, longOptions, &optionIndex)) != -1){
        switch (c){
            case 'h':
                printf("TODO napoveda\n");
                break;

            case 'i':
                printf("interface: %s\n", optarg);
                if(optarg[0] != '\0'){
                    memset(ps->interface, 0, sizeof(ps->interface));
                    strncpy(ps->interface, optarg, strlen(optarg));
                }
                break;

            case 't':
                printf("TCP_SET\n");
                if(ps->protocol != -1) {free(ps); return NULL;}
                ps->protocol = TCP_SET;
                break;

            case 'u':
                printf("UDP_SET\n");
                if(ps->protocol != -1)  {free(ps); return NULL;}
                ps->protocol = UDP_SET;
                break;

            case ARPARG:
                printf("ARP_SET\n");
                if(ps->protocol != -1)  {free(ps); return NULL;}
                ps->protocol = ARP_SET;
                break;

            case ICMPARG:
                printf("ICMP_SET\n");
                if(ps->protocol != -1)  {free(ps); return NULL;}
                ps->protocol = ICMP_SET;
                break;

            case 'n': //pocet paketu ke zpracovani
                printf("number: %d\n", atoi(optarg));
                temp = NULL;
                ps->numberOfPackets = (int)strtol(optarg, &temp, 10);
                if(*temp != '\0')  {free(ps); return NULL;}
                break;

            case 'p': //port
                printf("port: %d\n", atoi(optarg));
                temp = NULL;
                ps->port = (int)strtol(optarg, &temp, 10);
                if(*temp != '\0')  {free(ps); return NULL;}
                break;

            default:
                break;
        }

    }

    return ps;
}

void freeProgramSetting(ProgramSettings *ps){
    free(ps);
}
