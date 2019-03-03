#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

int size = 0;
int fail_insert=0;
int fail_search=0;
#define MAX 100000
#define A 0.6180339887

typedef struct Node{
    int key;
    int value;
}Node;

typedef struct HashTable{
    struct Node* *table;
    int size;
}HashTable;

HashTable* CreateTable(int size)
{
    struct HashTable* ht = malloc(sizeof(struct HashTable)); //table[size] = {};
 
    ht -> table = malloc( sizeof(struct Node*)*size );
    ht -> size = size;
    return ht;
}

Node* CreateNode(int key, int value){
    Node* n = malloc(sizeof(struct Node));
    n -> key = key;
    n -> value = value;
    return n;
}

int calculateHash(int key, int size){
    int h = (size*((double)(A*key)-(int)(A*key)));
    return h;
    
}

void Insert(struct HashTable *ht, int key, int value){
    int pos = calculateHash(key, size); 
    int i=1;
    Node* newNode = CreateNode(key, value);
    if(ht->table[pos] == NULL)
        ht->table[pos] = newNode;
    else{
        while(ht->table[pos+i]!=NULL){
            i++;
            fail_insert++;
        }
        ht->table[pos+i] = newNode;
    }
  
}

int Search(struct HashTable *ht, int key){
    int i = 0;
    int pos = calculateHash(key, size); 
    
    while(ht->table[pos] != NULL){
        if(ht->table[pos]->key == key){
            //printf("\nnode (%d, %d) is found at position %d\n", ht-> table[pos]-> key, ht-> table[pos] -> value, pos);
            return ht -> table[pos] -> value;
        }
        i++;
        fail_search++;
        pos = (key + i) % size;
    }
    
    //printf("cannot find the value at %d\n", key);
    return -1;
}

void printTable(struct HashTable* ht){
    int i;
    assert(ht);
    assert(ht->table);
    for(i=0; i<size; i++){
        if( ht -> table[i] == NULL) 
            printf("index %d : null\n",i);
        else
            printf("index %d : (%d, %d)\n", i, ht -> table[i] -> key ,ht -> table[i] -> value);
    }    
}

int main(int argc, char* argv[]){
    
    if(argc == 2){
        size = atoi(argv[1]);
        assert(size>0);
    }
    else{
        fprintf(stderr, "usage: %s <n> \n", argv[0]);
        fprintf(stderr, "where <n> is the size of table");
    }
    
    HashTable *ht = CreateTable(size);
    HashTable *ht2 = CreateTable(size);

    srand(time(NULL));
    int v, k, i; //load_factor = k/size
    
//load factor 50%    
    for(i=0; i < ht->size/2; i++){
        v = rand() % MAX;
        k = rand() % MAX;
        Insert(ht, v, k);
      
    }
    printf("\nload factor 50%\n");
    printf("reprobe(insert) : %d\n", fail_insert);
    printf("average reprobe : %5lf\n", (float)fail_insert / (float)(ht->size/2));
    
    for(i=0; i< 10000; i++)
    {
        k = rand() % MAX;
        Search(ht, k);
    }
    
    float average = (float)fail_search/10000.0;
    //printTable(ht);
    printf("reprobe(search) : %d\n", fail_search);
    printf("average reprobe : %5lf\n\n", average);

//load factor 90%    
    for(i=0; i < ht2->size*0.9; i++){
        v = rand() % MAX;
        k = rand() % MAX;
        Insert(ht2, v, k);
      
    }
    printf("\nload factor 90%\n");
    printf("reprobe(insert) : %d\n", fail_insert);
    printf("average reprobe : %5lf\n", (float)fail_insert / (float)(ht->size*0.9));
    
    for(i=0; i< 10000; i++)
    {
        k = rand() % MAX;
        Search(ht, k);
    }
    
    average = (float)fail_search/10000.0;
    //printTable(ht);
    printf("reprobe(search) : %d\n", fail_search);
    printf("average reprobe : %5lf\n\n", average);
    
    return 0;
}
