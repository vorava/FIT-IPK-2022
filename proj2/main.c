// IPK projekt 2 - main.c
// Autor: Vojtěch Orava (xorava02)
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <pcap/pcap.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include "arg_parser.h"
#include "filter.h"
#include "packet.h"

#define MAX_LEN 1000
#define ETHERNET_HEADER_LEN 14
#define TCP_PROTOCOL_ID 6
#define UDP_PROTOCOL_ID 17
#define ICMP_PROTOCOL_ID 1

void processPacket(u_char *a, const struct pcap_pkthdr *pkthdr, const u_char *packet);

int main(int argc, char *argv[]){
    char errBuffer[PCAP_ERRBUF_SIZE];   //error buffer pro pcap
    pcap_t *handle;     //pcap handler
    struct bpf_program fp;		// zkompilovany filtr
    char *filterExpression; //vyraz pro filtr packetu
    bpf_u_int32 mask;		//sitova maska
    bpf_u_int32 net;		//ip adresa

    ProgramSettings *ps = parseArguments(argc, argv); //zpracovani argumentu
    if(ps == NULL){
        fprintf(stderr, "Wrong arguments!\n");
        return 1;
    }

    // interface neni zadan, nebo je bez hodnoty
    pcap_if_t *alldevs;
    if(!strcmp(ps->interface, "NOT_SET")){
        if(pcap_findalldevs(&alldevs, errBuffer) != 0){
            fprintf(stderr, "Couldnt find active devices. %s", errBuffer);
            return 1;
        }
        pcap_if_t *dev = alldevs; // iteracni promenna
        printf("Active interfaces: \n");
        int i = 1; //cislovani vypisu
        while (dev != NULL){
            printf("%d) %s\n", i, dev->name);
            dev = dev->next;
            i++;
        }
        pcap_freealldevs(alldevs); //uvolneni rozhrani
        return 0;
    }


    if (pcap_lookupnet(ps->interface, &net, &mask, errBuffer) == -1) {
    	fprintf(stderr, "Cant get netmask for device %s\n", ps->interface);
    	net = 0;
    	mask = 0;
    }

    handle = pcap_open_live(ps->interface, BUFSIZ, 1, MAX_LEN, errBuffer);
    if (handle == NULL) {
    	fprintf(stderr, "Couldnt open device %s: %s\n", ps->interface, errBuffer);
    	return(2);
    }

    filterExpression = getFilter(ps);

    if (pcap_compile(handle, &fp, filterExpression, 0, net) == -1) {
    	fprintf(stderr, "Couldnt parse filter %s: %s\n", filterExpression, pcap_geterr(handle));
    	return(2);
    }
    if (pcap_setfilter(handle, &fp) == -1) {
    	fprintf(stderr, "Couldnt use filter %s: %s\n", filterExpression, pcap_geterr(handle));
    	return(2);
    }

    // zpracovani numberOfPackets packetu funkci processPacket
    pcap_loop(handle, ps->numberOfPackets, processPacket, NULL);

    pcap_close(handle);
    freeProgramSetting(ps);
    freeFilter(filterExpression);
    return 0;
}

void processPacket(u_char *a, const struct pcap_pkthdr *pkthdr, const u_char *packet){
    (void)a;
    printf("timestamp: %ld\n", pkthdr->ts.tv_usec);

    struct ether_header *ethHeader;
    ethHeader = (struct ether_header *) packet;

    printf("src MAC: %s\n", ether_ntoa((struct ether_addr *)ethHeader->ether_shost));
    printf("dst MAC: %s\n", ether_ntoa((struct ether_addr *)ethHeader->ether_dhost));
    printf("frame length: %d bytes\n", pkthdr->len);

    struct ip *ipHeader = (struct ip *)(packet + ETHERNET_HEADER_LEN);
    // druhe 4 bity z prvniho bytu jsou ipHeaderLength
    unsigned int ipHeaderLength = ipHeader->ip_hl * 4;
    printf("IPlen %d\n", ipHeaderLength);
    //(void)ipHeaderLength;
    printf("src IP: %s\n", inet_ntoa(ipHeader->ip_src));
    printf("dst IP: %s\n", inet_ntoa(ipHeader->ip_dst));

    // https://unix.superglobalmegacorp.com/BSD4.4Lite/newsrc/netinet/tcp.h.html
    // https://www.tcpdump.org/other/sniffex.c
    // https://www.devdungeon.com/content/using-libpcap-c
    if(ipHeader->ip_p == TCP_PROTOCOL_ID){
        printf("TCP\n");
        struct tcphdr *tcpHeader = (struct tcphdr *)(packet + ETHERNET_HEADER_LEN + ipHeaderLength);
        printf("src port: %d\n", ntohs(tcpHeader->th_sport));
        printf("dst port: %d\n", ntohs(tcpHeader->th_dport));

        unsigned int tcpHeaderLength = tcpHeader->th_off * 4;

        /*
        const u_char* data = packet + ETHERNET_HEADER_LEN + ipHeaderLength + tcpHeaderLength;
        unsigned int dataLength = pkthdr->caplen - (ETHERNET_HEADER_LEN + ipHeaderLength + tcpHeaderLength);

        const u_char *temp_pointer = data;
        unsigned int i = 0;
        while (i < dataLength) {
            printf("%c", *temp_pointer);
            temp_pointer++;
            i++;
        }
        printf("\n");
        */
    }






}
