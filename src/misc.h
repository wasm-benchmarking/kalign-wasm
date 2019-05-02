#ifndef MISC_H
#define MISC_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>


#include "tldevel.h"



extern int byg_detect(int* text,int n);
extern int byg_start(char* pattern,char*text);
extern int byg_end(char* pattern,char*text);
extern int byg_count(char* pattern,char*text);


/* The following two hash functions are taken from the supplementary of: */

/* Steinegger, Martin, and Johannes Söding. "Clustering huge protein sequence sets in linear time." Nature communications 9.1 (2018): 2542. */

// (c) 2017 Johannes Soeding & Martin Steinegger, Gnu Public License version 3
unsigned circ_hash(const int * x, unsigned length);
unsigned circ_hash_next(const int * x, unsigned length, int x_first, short unsigned h);

#endif
