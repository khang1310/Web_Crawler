#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "indexPage.h"

#define MAX_ADDR_LENGTH 3000
#define MAX_LINE_LENGTH 5000

// Number of indexed pages.
extern int indexedPagesCount;

/*
 * a node in our linked-list of web addresses
 */
struct listNode
{
  char addr[MAX_ADDR_LENGTH];
  struct trieNode *tNode;
  int totalTerms;

  struct listNode *next;
};

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *, const char *);

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode **, const char *);

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *);

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *);

int crawler(const char *, const int, struct listNode **);

/*
 * srcAddr should be a web address (e.g., http://www.yahoo.com).
 * link should point to an array of maxLinkLength characters.
 * getLink returns 1 if a link was found and 0 otherwise.
 * If a link was found, "link" will be filled in with the web address.
 */
int getLink(const char *, char *, const int);

struct listNode *newListNode(const char *);

#endif