#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct kv {
    struct kv *next;
    struct kv *prev;
    int key;
    int val;
} kv_t;

typedef struct {
    kv_t head;
    kv_t *map;
    int capacity;
} LRUCache;


LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* lru = malloc(sizeof(LRUCache));
    if (!lru) { return NULL; }
    memset(lru, 0, sizeof(LRUCache));

    // setup dummy head
    lru->head.next = NULL;
    lru->head.prev = NULL;
    lru->head.key = -1;
    lru->head.val = -1;

    lru->capacity = capacity;

    // setup map of hashed key -> kv_t*
    lru->map = malloc(sizeof(kv_t) * capacity);
    if (!lru->map) {
        free(lru);
        return NULL;
    }
    memset(lru->map, 0, sizeof(kv_t) * capacity);

    return lru;    
}

int lRUCacheGet(LRUCache* obj, int key) {
    
    return -1;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    
}

void lRUCacheFree(LRUCache* obj) {
    
}

/**
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);
 
 * lRUCachePut(obj, key, value);
 
 * lRUCacheFree(obj);
*/

int main() {
    printf("Hello world\n");
};
