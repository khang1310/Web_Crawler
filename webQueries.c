#include "webQueries.h"

/* Web Queries' Driver */
int webQueries(struct listNode *node)
{
    /* Process the web query. */
    while (1)
    {
        char webQuery[MAX_LINE_LENGTH];
        struct listNode *cursor = node;
        int inputCheck; // if input is valid, value is 1, otherwise 0.

        // Best web pages.
        char *bestPages[MAX_BEST_PAGES];
        double bestScores[MAX_BEST_PAGES];
        //Set all best scores to 0
        for (int i = 0; i < MAX_BEST_PAGES; i++)
        {
            bestScores[i] = 0;
        }

        // Store the IDF of each word.
        double idfScores[MAX_LINE_LENGTH];

        printf("\nEnter a web query: ");

        if (!gets(webQuery) || (strcmp(webQuery, "") == 0))
        {
            printf("Exiting the program\n");
            break;
        }

        inputCheck = 1;

        for (int i = 0; webQuery[i] != '\0'; i++)
        {
            if ((webQuery[i] < 'a' || webQuery[i] > 'z') && webQuery[i] != ' ')
            {
                inputCheck = 0;
                break;
            }
        }

        if (inputCheck == 0)
        {
            printf("Please enter a query containing only lower-case letters.\n");
            continue;
        }

        printf("Query is \"%s\".\n", webQuery);

        /* Get the words in the term*/
        char *words[MAX_LINE_LENGTH];
        int i = 0;

        words[i] = strtok(webQuery, " ");

        while (words[i] != NULL)
        {
            words[++i] = strtok(NULL, " ");
        }

        printf("IDF scores are\n");

        /* Process the score. */
        for (int i = 0; i < MAX_LINE_LENGTH; i++)
        {
            if (words[i] == NULL)
            {
                break;
            }

            idfScores[i] = computeIDF(node, words[i]);
            printf("IDF(%s): %.7f\n", words[i], idfScores[i]);
        }

        // Compute the score of web pages and find the best one
        while (cursor)
        {
            double score = 0;

            for (int i = 0; i < MAX_LINE_LENGTH; i++)
            {
                if (words[i] == NULL)
                {
                    break;
                }

                score += computeTF(cursor, words[i]) * idfScores[i];
            }

            for (int i = 0; i < MAX_BEST_PAGES; i++)
            {
                if (score > bestScores[i])
                {
                    if (i > 0)
                    {
                        if (score < bestScores[i - 1] || (score == bestScores[i - 1] && strcmp(bestPages[i - 1], cursor->addr)))
                        {
                            if (i == 1)
                            {
                                bestPages[2] = bestPages[1];
                                bestScores[2] = bestScores[1];

                                bestPages[i] = cursor->addr;
                                bestScores[i] = score;
                            }

                            else
                            {
                                bestPages[i] = cursor->addr;
                                bestScores[i] = score;
                            }
                        }
                    }

                    else
                    {
                        bestPages[2] = bestPages[1];
                        bestScores[2] = bestScores[1];
                        bestPages[1] = bestPages[0];
                        bestScores[1] = bestScores[0];

                        bestPages[i] = cursor->addr;
                        bestScores[i] = score;
                    }
                }
            }

            cursor = cursor->next;
        }

        // Print out the best web pages.
        printf("Web pages:\n");

        for (int i = 0; i < 3; i++)
        {
            if (bestScores[i] > 0.0)
            {
                printf("%d. %s (score: %.4f)\n", i + 1, bestPages[i], bestScores[i]);
            }
        }
    }

    return 0;
}

/* Compute the TF score of a word for a web page */
double computeTF(struct listNode *node, const char *word)
{
    return getOccurOfWord(node->tNode, word) * (1.0) / (node->totalTerms);
}

/*Compute the IDF score of a word for a web page*/
double computeIDF(struct listNode *root, const char *word)
{
    struct listNode *cursor = root;
    int pagesContainWord = 0;

    while (cursor != NULL)
    {
        if (getOccurOfWord(cursor->tNode, word))
        {
            pagesContainWord++;
        }

        cursor = cursor->next;
    }

    return log((1.0 + indexedPagesCount) / (1.0 + pagesContainWord));
}