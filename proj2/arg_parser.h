// IPK projekt 2 - arg_parser.h
// Autor: Vojtěch Orava (xorava02)
#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

#define TCP_SET 1
#define UDP_SET 2
#define ICMP_SET 3
#define ARP_SET 4

#define ARPARG 5
#define ICMPARG 6

typedef struct{
    char interface[1000];
    int port;
    int protocol;
    unsigned int numberOfPackets;
} ProgramSettings;

ProgramSettings *parseArguments(int argc, char *argv[]);
void freeProgramSetting(ProgramSettings *ps);
#endif
