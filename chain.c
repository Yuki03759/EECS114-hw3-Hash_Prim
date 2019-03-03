#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX 100000
int fail_insert = 0;
int fail_search = 0;

typedef struct Node{
    int key;
    int value;
    struct Node* next;
}Node;

typedef struct HashTable{
    struct Node* *table;
    int size;
}HashTable;

HashTable* CreateTable(int size){
    struct HashTable* ht = malloc(sizeof(struct HashTable));
    ht-> table = malloc(sizeof(struct Node*)*size);
    ht-> size = size;
    return ht;
}

Node* CreateNode(int key, int value){
    Node* n = malloc(sizeof(struct Node));
    n-> key = key;
    n-> value = value;
    n-> next = NULL;
}

int calculateHash(int key, int size){
    int h = key%size;
    return h;
}

void Insert(struct HashTable *ht, int key, int value){
    int pos = calculateHash(key, ht->size);
    Node* newNode = CreateNode(key, value);
    if(ht->table[pos] == NULL){
        ht->table[pos] = newNode;
    }
    else{
        Node *temp = ht->table[pos];
        while(temp->next){
            temp = temp->next;
            fail_insert++;
        }
        temp->next = newNode;
    }
}


int Search(HashTable* ht, int key){
    int h = calculateHash(key, ht->size);
    while(ht->table[h] != NULL){
        if(ht->table[h]->key == key){
            return ht->table[h]->value;
        }
        ht->table[h] = ht->table[h]->next;
        fail_search++;   
    }
}


void printTable(struct HashTable* ht){
    int i;
    Node *n;
    assert(ht);
    assert(ht->table);
    for(i=0; i<ht->size; i++){
        if( ht -> table[i] == NULL) 
            printf("index %d : null\n",i);
        else{
            //printf("index %d : (%d, %d)\n", i, ht -> table[i] -> key ,ht -> table[i] -> value);
            printf("index %d : ", i);
            n = ht ->table[i];
            while(n!=NULL){
                printf("(%d, %d) ", n -> key , n -> value);
                n=n->next;
            }
            printf("\n");
        }
    }    
}

int main(int argc, char* argv[]){
    int size=1;
    if(argc == 2){
        size = atoi(argv[1]);
        assert(size>0);
    }
    else{
        fprintf(stderr, "usage: %s <n> \n", argv[0]);
    }
    HashTable *ht = CreateTable(size);
    HashTable *ht2 = CreateTable(size);
    
    srand(time(NULL));
    int v, k, i;
    
    
//load factor 50%    
    for(i=0; i < ht->size/2; i++){
        v = rand() % MAX;
        k = rand() % MAX;
        Insert(ht, v, k);
      
    }
    printf("\nload factor 50%\n");
    printf("chain(insert) : %d\n", fail_insert);
    printf("average chain : %5lf\n", (float)fail_insert / (float)(ht->size/2));
    
    for(i=0; i< 10000; i++)
    {
        k = rand() % MAX;
        Search(ht, k);
    }
    
    float average = (float)fail_search/10000.0;
    printf("chain(search) : %d\n", fail_search);
    printf("average chain : %5lf\n\n", average);   

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
    printf("chain(search) : %d\n", fail_search);
    printf("average chain : %5lf\n\n", average);
    
    return 0;
}
