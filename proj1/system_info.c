// IPK projekt 1 - system_info.c
// Autor: Vojtěch Orava (xorava02)
#include <stdio.h>
#include <string.h>
#include "system_info.h"

// vraci jmeno serveru v argumentu hostName + true
// v pripade chyby vraci false
bool getHostName(char *hostName, int length){
    FILE *f;
    f = fopen("/proc/sys/kernel/hostname", "r");
    if(f == NULL) return false;
    if(fgets(hostName, length, f) == NULL)
        return false;

    hostName[strcspn(hostName, "\n")] = 0;
    fclose(f);
    return true;
}

// vraci jmeno CPU v argumentu cpuName + true
// v pripade chyby vraci false
bool getCpuName(char *cpuName, int length){
    FILE *f;
    f = popen("cat /proc/cpuinfo | grep 'model name' | head -n 1 | awk  -F ':' '{print $2}' | xargs", "r");
    if(f == NULL) return false;
    if(fgets(cpuName, length, f) == NULL)
        return false;

    cpuName[strcspn(cpuName, "\n")] = 0;
    pclose(f);
    return true;
}

/*
 * vraci CPU load vypocitany podle  https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
 * v argumentu cpuLoad + true , pri chybe vraci false
 *       0        1       2      3        4     5      6        7      8        9
 *      user    nice   system  idle    iowait  irq   softirq  steal  guest  guest_nice
  cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
 */
bool getCpuLoad(char *cpuLoad, int length){
    char temp[length];
    char *end;
    long int previousValues[10];
    long int currentValues[10];
    FILE *f;
    f = popen("cat /proc/stat | head -n1 ", "r");
    if(f == NULL) return false;
    if(fgets(temp, length, f) == NULL)
        return false;

    const char delim[2] = " ";
    char *token;
    int i = 0;
    token = strtok(temp, delim);

    // iterace jednotlivymi hodnotami z /proc/stat
    while(token != NULL) {
        if(i != 0){ //preskoceni policka cpu
            previousValues[i-1] = strtol(token, &end, 10);
        }
        i++;
        token = strtok(NULL, delim);
    }
    pclose(f);

    // 200 ms zpozdeni pro vypocet
    sleep(0.2);
    memset(temp, 0, length);
    f = popen("cat /proc/stat | head -n1 ", "r");
    if(f == NULL) return false;
    if(fgets(temp, length, f) == NULL)
        return false;

    i = 0;
    token = strtok(temp, delim);

    while(token != NULL) {
        if(i != 0){ //preskoceni policka cpu
            currentValues[i-1] = strtol(token, &end, 10);
        }
        i++;
        token = strtok(NULL, delim);
    }
    pclose(f);

    // vypocet procent ze ziskanych hodnot dle navodu z URL
    long int previousTotal = 0;
    long int currentTotal = 0;
    for (int i = 0; i <= 7; i++){
        previousTotal += previousValues[i];
        currentTotal += currentValues[i];
    }

    // totald = Total - PrevTotal
    long int totalDiff = currentTotal - previousTotal;

    //          3      4         3           4
    //idled = idle + iowait - previdle - previowait
    long int idleDiff = currentValues[3] + currentValues[4] - previousValues[3] -
                        previousValues[4];

    // CPU % - nutno prevest na double pro spravny vypocet
    double cpuPrct = ((double)totalDiff - (double)idleDiff)/(double)totalDiff;
    snprintf(cpuLoad, length, "%d", (int)(cpuPrct*100));
    strcat(cpuLoad, "%");
    return true;
}
