#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* mergeStrings(const char* word1, const char* word2) {
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    
    char* merged = (char*)malloc(len1 + len2 + 1); // +1 for the null terminator
    if (!merged) {
        // Memory allocation failed
        return NULL;
    }

    int i = 0, j = 0, k = 0;

    while (i < len1 && j < len2) {
        merged[k++] = word1[i++];
        merged[k++] = word2[j++];
    }

    // Append remaining letters from word1, if any
    while (i < len1) {
        merged[k++] = word1[i++];
    }

    // Append remaining letters from word2, if any
    while (j < len2) {
        merged[k++] = word2[j++];
    }

    merged[k] = '\0'; // Null-terminate the merged string

    return merged;
}
