/* This code indexes a web page, printing out the counts of words on that page */

#include "indexPage.h"

// Create a new node (make it easier to deal with the code later)
struct trieNode *newTrieNode()
{
  struct trieNode *node = (struct trieNode *)malloc(sizeof(struct trieNode));

  if (node)
  {
    node->isLeaf = 0;

    int i;
    for (i = 0; i < MAX_CHILDREN_NODES; i++)
    {
      node->children[i] = NULL;
    }
  }

  return node;
}

struct trieNode *indexPage(const char *url, int *totalTerms)
{
  char buffer[BUFFER_SIZE];

  struct trieNode *node = newTrieNode();

  char word[WORD_LENGTH];

  // Print out the URL
  printf("%s\n", url);

  // If getText return 0, an error occurs
  if (getText(url, buffer, BUFFER_SIZE))
  {
    // Remove all non-alphabetic characters and lowercase all the words
    int charPos = 0;

    int i;
    for (i = 0; buffer[i] != '\0'; i++)
    {
      if ((buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= 'A' && buffer[i] <= 'Z'))
      {
        word[charPos] = tolower(buffer[i]);
        charPos++;

        // If the next character is a blank space, a break line or a null character, the word is complete
        if ((buffer[i + 1] < 'a' || buffer[i + 1] > 'z') && (buffer[i + 1] < 'A' || buffer[i + 1] > 'Z'))
        {
          word[charPos] = '\0';
          printf("\t%s\n", word);

          // Add word occurence to the trie
          addWordOccurrence(word, WORD_LENGTH, node);
          (*totalTerms)++;

          charPos = 0;
        }
      }
    }
  }

  return node;
}

/*Get the occurence of a specific word in the trie*/
int getOccurOfWord(struct trieNode *node, const char *word)
{
  struct trieNode *cursor = node;

  for (int i = 0; i < strlen(word); i++)
  {
    int index = word[i] - 'a';
    if (cursor->children[index] == NULL)
      return 0;

    cursor = cursor->children[index];
  }

  if (cursor != NULL && cursor->isLeaf)
  {
    return cursor->occurence;
  }

  return 0;
}

int addWordOccurrence(const char *word, const int wordLength, struct trieNode *node)
{
  struct trieNode *cursor = node;

  int i;
  for (i = 0; word[i] != '\0'; i++)
  {
    int index = *(word + i) - 'a'; // get the index of the letter (for example: if the letter is 'a', the index is 0)

    if (cursor->children[index] == NULL)
    {
      cursor->children[index] = newTrieNode();

      // If somehow a new node cannot be created, return error code 1
      if (cursor->children[index] == NULL)
      {
        printf("Cannot add word occurence. Something went wrong.");

        return 1;
      }
    }

    cursor = cursor->children[index];
  }

  cursor->isLeaf = 1;
  cursor->occurence++;

  return 0;
}

void printTrieContents(struct trieNode *node, char *content, const int wordLength, int level)
{
  if (node->isLeaf)
  {
    content[level] = '\0';

    if (node->occurence)
    {
      printf("%s: %d\n", content, node->occurence);
    }
  }

  int i;
  for (i = 0; i < MAX_CHILDREN_NODES; i++)
  {
    if (node->children[i])
    {
      content[level] = i + 'a';
      printTrieContents(node->children[i], content, WORD_LENGTH, level + 1);
    }
  }
}

int freeTrieMemory(struct trieNode *node)
{
  if (!node)
  {
    return 0;
  }

  int i;
  for (i = 0; i < MAX_CHILDREN_NODES; i++)
  {
    freeTrieMemory(node->children[i]);
  }

  free(node);

  return 0;
}

/* You should not need to modify this function */
int getText(const char *srcAddr, char *buffer, const int bufSize)
{
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if (pipe == NULL)
  {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
            buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize - 1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
