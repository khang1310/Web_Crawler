#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "crawler.h"
#include "indexPage.h"
#include "webQueries.h"

int main(int argc, char **argv)
{
    struct listNode *pListStart;

    long seed;

    char fileName[MAX_LINE_LENGTH];

    int maxNumPages;

    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "USAGE: %s startAddr maxNumPages [rand seed]", argv[0]);
        return -1;
    }

    /* initialization */
    if (argc >= 4)
    {
        seed = atol(argv[3]);
    }
    else
    {
        seed = time(NULL);
    }

    srand(seed);

    strncpy(fileName, argv[1], MAX_LINE_LENGTH);
    fileName[MAX_LINE_LENGTH - 1] = '\0';

    maxNumPages = atoi(argv[2]);

    // Crawl the web pages.
    printf("Indexing...\n");
    crawler(fileName, maxNumPages, &pListStart);

    webQueries(pListStart);

    destroyList(pListStart);

    return 0;
}