#ifndef WEBQUERIES_H
#define WEBQUERIES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "crawler.h"
#include "indexPage.h"

#define MAX_BEST_PAGES 3

/* Web Queries' Driver */
int webQueries(struct listNode *);

/* Compute the TF score of a word for a web page */
double computeTF(struct listNode *, const char *);

/*Compute the IDF score of a word for a web page*/
double computeIDF(struct listNode *, const char *);

#endif