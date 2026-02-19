#include <stdlib.h>
#include <stdio.h>

#define HASH_SIZE 10007  // prime number to reduce collisions

typedef struct Node {
    int key;      // the number from nums
    int value;    // its index
    struct Node* next;
} Node;

Node* hashTable[HASH_SIZE];

// Simple hash function (absolute value to handle negatives)
int hash(int key) {
    return abs(key) % HASH_SIZE;
}

// Insert key-value pair into the hash table
void insert(int key, int value) {
    int index = hash(key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Search for a key; return its value (index) if found, otherwise -1
int search(int key) {
    int index = hash(key);
    Node* current = hashTable[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return -1;
}

// Free the entire hash table (optional, but good practice)
void freeHashTable() {
    for (int i = 0; i < HASH_SIZE; i++) {
        Node* current = hashTable[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}

// The required function
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    // Initialize hash table to NULL
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable[i] = NULL;
    }
    
    int* result = (int*)malloc(2 * sizeof(int));
    *returnSize = 2;
    
    for (int i = 0; i < numsSize; i++) {
        int complement = target - nums[i];
        int foundIndex = search(complement);
        if (foundIndex != -1) {
            result[0] = foundIndex;
            result[1] = i;
            freeHashTable();  // clean up before returning
            return result;
        }
        insert(nums[i], i);
    }
    
    // Should never reach here per problem guarantee
    freeHashTable();
    free(result);
    *returnSize = 0;
    return NULL;
}


int main() {
    int nums[] = {2, 7, 11, 15};
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    int target = 9;
    int returnSize;
    int* indices = twoSum(nums, numsSize, target, &returnSize);
    
    if (indices) {
        printf("[%d, %d]\n", indices[0], indices[1]);
        free(indices);
    }
    return 0;
}