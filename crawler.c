#include "crawler.h"

int indexedPagesCount;

/* Create a new list node */
struct listNode *newListNode(const char *addr)
{
  /* Create a new node */
  struct listNode *newNode = (struct listNode *)malloc(sizeof(struct listNode));
  int totalTerms = 0;

  if (newNode == NULL)
  {
    fprintf(stderr, "ERROR: could not allocate memory.\n");
    return NULL;
  }

  // Index the page.
  strncpy(newNode->addr, addr, MAX_ADDR_LENGTH);
  newNode->tNode = indexPage(newNode->addr, &totalTerms);
  newNode->totalTerms = totalTerms;

  newNode->next = NULL;

  return newNode;
}

// Crawler's Driver.
int crawler(const char *fileName, const int maxNumPages, struct listNode **pListStart)
{
  char line[MAX_LINE_LENGTH];
  char url[MAX_ADDR_LENGTH];
  int maxNumHops;

  FILE *file = fopen(fileName, "r");

  if (file == NULL)
  {
    fprintf(stderr, "ERROR: could not open the file.\n");
    return -1;
  }

  indexedPagesCount = 0;
  while (fgets(line, MAX_LINE_LENGTH, file) && indexedPagesCount < maxNumPages)
  {
    // Get the link and the number
    int i;
    for (i = 0; line[i] != ' '; i++)
    {
      url[i] = line[i];
    }

    url[i] = '\0';

    maxNumHops = atoi(line + i + 1);

    //Starts crawling.
    int hopNum = 0;
    while (1)
    {
      //Crawl/index the pages
      if (!contains(*pListStart, url))
      {
        insertBack(pListStart, url);

        indexedPagesCount++;
      }

      hopNum++;

      if (hopNum <= maxNumHops && indexedPagesCount < maxNumPages)
      {
        int res = getLink(url, url, MAX_ADDR_LENGTH);

        if (!res)
        {
          break;
        }
      }

      else
      {
        break;
      }
    }
  }

  return 0;
}

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr)
{
  if (pNode == NULL)
  {
    return 0;
  }

  if (strcmp(pNode->addr, addr) == 0)
  {
    return 1;
  }

  return contains(pNode->next, addr);
}

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode **pNode, const char *addr)
{
  if (*pNode == NULL)
  {
    *pNode = newListNode(addr);

    return;
  }

  if ((*pNode)->next == NULL)
  {
    (*pNode)->next = newListNode(addr);

    return;
  }

  insertBack(&((*pNode)->next), addr);
}

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode)
{
  if (pNode != NULL)
  {
    printf("%s\n", pNode->addr);

    printAddresses(pNode->next);
  }
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode)
{
  // TODO: complete this
  if (pNode == NULL)
  {
    return;
  }

  destroyList(pNode->next);

  freeTrieMemory(pNode->tNode);
  free(pNode);
}

int getLink(const char *srcAddr, char *link, const int maxLinkLength)
{
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if (pipe == NULL)
  {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if (numLinks > 0)
  {
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for (linkNum = 0; linkNum < numLinks; linkNum++)
    {
      fgets(buffer, bufSize, pipe);

      if (r < (linkNum + 1.0) / numLinks)
      {
        break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength - 1] = '\0';

    /* get rid of the newline */
    {
      char *pNewline = strchr(link, '\n');
      if (pNewline != NULL)
      {
        *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if (numLinks > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
