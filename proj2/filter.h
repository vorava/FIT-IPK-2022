// IPK projekt 2 - filter.h
// Autor: Vojtěch Orava (xorava02)
#ifndef __FILTER_H__
#define __FILTER_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arg_parser.h"
#define MAX_LEN 1000

char *getFilter(ProgramSettings *ps);
void freeFilter(char *f);

#endif
