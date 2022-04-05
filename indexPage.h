#ifndef INDEXPAGE_H
#define INDEXPAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHILDREN_NODES 26
#define BUFFER_SIZE 300000
#define WORD_LENGTH 2000

struct trieNode
{
  // If the node is leaf, isLeaf = 1, else = 0
  int isLeaf;
  struct trieNode *children[MAX_CHILDREN_NODES];
  int occurence;
};

struct trieNode *newTrieNode();

struct trieNode *indexPage(const char *n, int *);

int addWordOccurrence(const char *, const int, struct trieNode *);

void printTrieContents(struct trieNode *, char *, const int, int);

int freeTrieMemory(struct trieNode *);

int getText(const char *, char *, const int);

/*Get the occurence of a specific word in the trie*/
int getOccurOfWord(struct trieNode *, const char *);

#endif