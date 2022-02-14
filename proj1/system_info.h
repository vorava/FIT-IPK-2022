// IPK projekt 1 - system_info.h
// Autor: Vojtěch Orava (xorava02)
#ifndef __SYSTEM_INFO_H__
#define __SYSTEM_INFO_H__
#include <stdbool.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

bool getHostName(char *hostName, int length);
bool getCpuName(char *cpuName, int length);
bool getCpuLoad(char *cpuLoad, int length);

#endif
