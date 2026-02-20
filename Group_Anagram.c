#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 20000

typedef struct Node {
    char* key;
    int groupIndex;
    struct Node* next;
} Node;

unsigned long hash(char* str) {
    unsigned long h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h % HASH_SIZE;
}

char* getSortedKey(char* s) {
    int count[26] = {0};
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        count[s[i] - 'a']++;
    char* key = malloc(len + 1);
    int idx = 0;
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < count[i]; j++)
            key[idx++] = 'a' + i;
    key[idx] = '\0';
    return key;
}

char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
    if (strsSize == 0) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    Node* hashTable[HASH_SIZE] = {NULL};

    int cap = 8, cnt = 0;
    char*** groups = malloc(sizeof(char**) * cap);
    int* sizes = malloc(sizeof(int) * cap);
    int* caps = malloc(sizeof(int) * cap);

    for (int i = 0; i < strsSize; i++) {
        char* key = getSortedKey(strs[i]);
        unsigned long h = hash(key);
        Node* node = hashTable[h];
        int found = 0;

        while (node) {
            if (strcmp(node->key, key) == 0) {
                found = 1;
                int g = node->groupIndex;
                if (sizes[g] == caps[g]) {
                    caps[g] *= 2;
                    groups[g] = realloc(groups[g], sizeof(char*) * caps[g]);
                }
                groups[g][sizes[g]++] = strs[i];
                break;
            }
            node = node->next;
        }

        if (!found) {
            if (cnt == cap) {
                cap *= 2;
                groups = realloc(groups, sizeof(char**) * cap);
                sizes = realloc(sizes, sizeof(int) * cap);
                caps = realloc(caps, sizeof(int) * cap);
            }
            int g = cnt++;
            caps[g] = 8;
            sizes[g] = 1;
            groups[g] = malloc(sizeof(char*) * caps[g]);
            groups[g][0] = strs[i];

            Node* newNode = malloc(sizeof(Node));
            newNode->key = key;
            newNode->groupIndex = g;
            newNode->next = hashTable[h];
            hashTable[h] = newNode;
        } else {
            free(key);
        }
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        Node* node = hashTable[i];
        while (node) {
            Node* tmp = node;
            node = node->next;
            free(tmp->key);
            free(tmp);
        }
    }

    *returnSize = cnt;
    *returnColumnSizes = malloc(sizeof(int) * cnt);
    for (int i = 0; i < cnt; i++)
        (*returnColumnSizes)[i] = sizes[i];

    free(sizes);
    free(caps);
    return groups;
}