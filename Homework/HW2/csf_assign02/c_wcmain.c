#include <stdio.h>
#include <stdint.h>
#include "wcfuncs.h"

//test

// Suggested number of buckets for the hash table
#define HASHTABLE_SIZE 13249

void process_input_word(struct WordEntry *buckets[], unsigned char *word, uint32_t *unique_words, const unsigned char **best_word, uint32_t *best_word_count);

int main(int argc, char **argv) {
    // stats (to be printed at end)
    uint32_t total_words = 0;
    uint32_t unique_words = 0;
    const unsigned char *best_word = (const unsigned char *) "";
    uint32_t best_word_count = 0;

    FILE *file;
    file = stdin;

    // Initializing the hash table
    // Will have arrays, each entry is a bucket
    // Bucket points to list of words (hashed same value)
    // All buckets will start empty

    struct WordEntry *buckets[HASHTABLE_SIZE];
    for (unsigned i = 0; i < HASHTABLE_SIZE; i++) {
        buckets[i] = NULL;
    }

    // Opens file for reading, can't use "read"

    if (argc == 2) {
        file = fopen(argv[1], "r");

        if (file == NULL) {
            return 1;
        }
    }

    // Hold word that is being processed
    unsigned char word[MAX_WORDLEN + 1] = {'\0'}; 

    // If word read from file, keep iterating
    while (wc_readnext(file, word)) {
        // Increase total word count
        total_words++;

        // Converts words to lower case and non alpha
        // We want words to be case insensitive??
        // non alpha means Cat & Cat! are same word
        wc_tolower(word);
        wc_trim_non_alpha(word);

        // Adds the word to our hash table
        // Updates unique word count & most frequent word stats
        process_input_word(buckets, word, &unique_words, &best_word, &best_word_count);
    }

    printf("Total words read: %u\n", total_words);
    printf("Unique words read: %u\n", unique_words);
    printf("Most frequent word: %s (%u)\n", (const char *) best_word, best_word_count);

    // Closes file, freeing memory
    if (argc == 2) {
        fclose(file);
    }
    // Basically going through & freeing all the nodes
    for (unsigned i = 0; i < HASHTABLE_SIZE; i++) {
        wc_free_chain(buckets[i]);
    }

    return 0;
}

// TODO: definitions of helper functions

void process_input_word(
        struct WordEntry *buckets[],
        unsigned char *word,
        uint32_t *unique_words,
        const unsigned char **best_word,
        uint32_t *best_word_count
) {
    struct WordEntry *entry = wc_dict_find_or_insert(buckets, HASHTABLE_SIZE, word);

    // Increment the count for this word since we've found/added it once more.
    entry->count++;

    if (entry->count == 1) {
        (*unique_words)++;
    }

    if (entry->count > *best_word_count) {
        *best_word = entry->word;
        *best_word_count = entry->count;
    }
}