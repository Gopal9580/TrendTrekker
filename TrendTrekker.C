#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LEN 100
#define MAX_HEAP_SIZE 1000

// Trie Node
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int frequency;
    char word[MAX_WORD_LEN];
} TrieNode;

// Heap Node
typedef struct HeapNode {
    char word[MAX_WORD_LEN];
    int frequency;
} HeapNode;

// Trie Functions
TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->frequency = 0;
    node->word[0] = '\0';
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

void insertTrie(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    while (*word) {
        if (isalpha(*word)) {
            int index = tolower(*word) - 'a';
            if (!curr->children[index])
                curr->children[index] = createTrieNode();
            curr = curr->children[index];
        }
        word++;
    }
    curr->frequency++;
    strcpy(curr->word, word);
}

// Heap Functions
HeapNode heap[MAX_HEAP_SIZE];
int heapSize = 0;

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(int index) {
    if (index && heap[(index - 1) / 2].frequency < heap[index].frequency) {
        swap(&heap[index], &heap[(index - 1) / 2]);
        heapifyUp((index - 1) / 2);
    }
}

void heapifyDown(int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heapSize && heap[left].frequency > heap[largest].frequency)
        largest = left;
    if (right < heapSize && heap[right].frequency > heap[largest].frequency)
        largest = right;
    if (largest != index) {
        swap(&heap[index], &heap[largest]);
        heapifyDown(largest);
    }
}

void insertHeap(const char* word, int frequency) {
    if (heapSize >= MAX_HEAP_SIZE)
        return;

    strcpy(heap[heapSize].word, word);
    heap[heapSize].frequency = frequency;
    heapifyUp(heapSize);
    heapSize++;
}

HeapNode extractMax() {
    HeapNode root = heap[0];
    heap[0] = heap[--heapSize];
    heapifyDown(0);
    return root;
}

// Collect words from Trie
void collectWords(TrieNode* root) {
    if (!root) return;
    if (root->frequency > 0) {
        insertHeap(root->word, root->frequency);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        collectWords(root->children[i]);
    }
}

// Main Program
int main() {
    TrieNode* root = createTrieNode();
    int n;
    char word[MAX_WORD_LEN];

    printf("Enter number of words (hashtags): ");
    scanf("%d", &n);

    printf("Enter the words (hashtags):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", word);
        insertTrie(root, word);
    }

    collectWords(root);

    int topK;
    printf("Enter how many top trends to display: ");
    scanf("%d", &topK);

    printf("\nTop %d Trending Words:\n", topK);
    for (int i = 0; i < topK && heapSize > 0; i++) {
        HeapNode trend = extractMax();
        printf("%s - %d times\n", trend.word, trend.frequency);
    }

    return 0;
}
