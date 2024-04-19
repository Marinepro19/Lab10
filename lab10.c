#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initialize a new Trie node
struct TrieNode *createNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            node->children[i] = NULL;
    }
    return node;
}

// Inserts a word into the Trie
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *crawler = root;
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!crawler->children[index])
            crawler->children[index] = createNode();
        crawler = crawler->children[index];
    }
    crawler->isEndOfWord = 1;
    crawler->count++;
}

// Computes the number of occurrences of a word in the Trie
int numberOfOccurrences(struct TrieNode *root, char *word) {
    struct TrieNode *crawler = root;
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!crawler->children[index])
            return 0;
        crawler = crawler->children[index];
    }
    if (crawler != NULL && crawler->isEndOfWord)
        return crawler->count;
    else
        return 0;
}

// Deallocates the Trie
void deallocateTrie(struct TrieNode *root) {
    if (root) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i])
                deallocateTrie(root->children[i]);
        }
        free(root);
    }
}

// Reads the dictionary file and returns the number of words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    for (int i = 0; i < numWords; i++) {
        pInWords[i] = (char *)malloc(50 * sizeof(char));
        fscanf(file, "%s", pInWords[i]);
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }

    // Words to check for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}