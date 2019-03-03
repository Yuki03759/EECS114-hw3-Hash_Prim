#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define INFINITY 1000000.0

typedef struct MinHeapNode{
    int v;
    int parent;
    float key;
    struct MinHeapNode* next;
}MinHeapNode;

typedef struct MinHeap{
    struct MinHeapNode** heap;
    int size;
}MinHeap;

typedef struct Graph{
    MinHeap* minHeap;
    float** adjMatrix;
}Graph;

typedef struct List{
    struct MinHeapNode* front;
    struct MinHeapNode* back;
    int size;
}List;

List* CreateList(void){
    List* l = malloc(sizeof(List));
    assert(l);
    l->front = NULL;
    l->back = NULL;
    l->size = 0;
}

MinHeapNode* CreateMinHeapNode(int v, float k, int parent){
    MinHeapNode* n = malloc(sizeof(MinHeapNode));
    n->v = v;
    n->key = k;
    n->parent = parent;
    n->next = NULL;
    return n;
}

List* removeFirstNode(List* l){
    MinHeapNode* currentNode = l->front;
    l->size--;
    l->front = currentNode->next;
    free(currentNode);
    return l;
}

void addNodeBack(List* l, int v, float key, int parent){
    assert(l);
    
    MinHeapNode* newNode = CreateMinHeapNode(v, key, parent);
    
    if(l->size == 0){
        l->front = newNode;
        l->back = newNode;
    }
    else{
        l->back ->next = newNode;
        l->back = newNode;
    }
    l->size++;
}

void printList(const List* list){
    MinHeapNode* currentNode = list->front;
    while(currentNode != NULL){
        printf(" %d ", currentNode->v);
        printf(" %d ", currentNode->parent);
        printf(" %f \n", currentNode->key);
        currentNode = currentNode->next;
    }
}



void MinHeapify(struct MinHeap* mh, int parent){
    int left = 2*parent;
    int right = 2*parent+1;
    
    assert(parent > 0);
    assert(parent < mh->size);
    
    if( left >= mh -> size && right >= mh -> size ){
     //node has not right child and no left child. Leaf node. Already a heap.   
        return;
    }
    else if ( right >= mh -> size ){
     //node has only a left child   
        if(mh->heap[left]->key  <  mh->heap[parent]->key){  
            MinHeapNode* temp = mh->heap[parent];
            mh->heap[parent] = mh->heap[left];
            mh->heap[left] = temp;
        }    
    }
    
    else{     
        //node has both left and right child

        int min = ( mh -> heap[right] -> key < mh -> heap[left] -> key ) ? right : left;
   
        if(mh->heap[min]->key  <  mh->heap[parent]->key){  
            MinHeapNode* temp = mh->heap[parent];
            mh->heap[parent] = mh->heap[min];
            mh->heap[min] = temp;
        }
        
        MinHeapify( mh, min );
    }
}

void printMinHeapNode(struct MinHeapNode* mhn){
    if(mhn == NULL)
        printf("NULL\n");
    else
        printf("(v -> %d, key -> %lf, parent -> %d)\n", mhn-> v , mhn -> key, mhn -> parent);
}

void printMinHeap(struct MinHeap* mh){
    int i;
    printf("\nMinHeap\n");
    printf("--------------------\n");
    for(i=0; i< mh -> size; i++){
        if(mh->heap[i] == NULL)
            printf("index %d : null\n",i);
        else
            printf("index %d : ", i);
            printMinHeapNode( mh->heap[i]);
    } 
}


void printGraph(struct Graph* g){
    int i, j;
    MinHeap* mh = g-> minHeap;
    
    printMinHeap(mh);
    printf("\n AdjMatrix \n");
    printf("--------------------\n");
    for(i=0; i < mh->size-1; i++){
        for(j=0; j < mh->size-1; j++){
            printf("adj[%d][%d] = %lf\n", i, j, g->adjMatrix[i][j]);
        }
    }
}

MinHeap* CreateMinHeap(MinHeapNode** array, int size){
    struct MinHeap* mh = malloc ( sizeof(struct MinHeap));
    assert ( mh ) ;
    
    mh -> heap = malloc ( sizeof ( struct MinHeapNode* ) * (size+1) );
    mh -> size = size+1;
    
    int i;
    for ( i=1 ; i< mh -> size ; i++ )
    {
        mh -> heap[i] = array[i-1];
    }
    mh -> heap[0] = NULL;
        
    for(i = size/2 ; i > 0 ; i--)
    {
       MinHeapify( mh, i );
    }
    return mh;
}

Graph* CreateGraph(MinHeap* mh, float** adj)
{
    struct Graph* g = malloc (sizeof(struct Graph));  
    assert(g);

    g->minHeap = mh;
    g->adjMatrix = adj;
    
    return g;
}

void writeFile(List* l, char *fileName){
    FILE* fp = fopen(fileName, "w");
    MinHeapNode* currentNode = l->front;
    while(currentNode != NULL){
        fprintf(fp, "v: %d, parent: %d, key: %f\n", currentNode->v, currentNode->parent, currentNode->key); 
        currentNode = currentNode->next;
    }
    fclose(fp);
}

struct Graph * ReadFile ( char * fileName ) 
{
    //read first line of the file.
    FILE *fp = fopen(fileName, "r+");
    int numberOfVertices=-1; //get from first line  
   
    assert(fp);
    fscanf(fp, "%d", &numberOfVertices);
 

    struct MinHeapNode** nodes = malloc ( sizeof ( struct MinHeapNode* ) * (numberOfVertices) );
    assert(nodes);
    int i;
   
    for ( i=0 ; i< numberOfVertices ; i++ )
    {
        nodes[i] = CreateMinHeapNode( i , INFINITY ,INFINITY );
    }
    
    struct MinHeap* mh = CreateMinHeap(nodes, numberOfVertices);
    
   
    float ** adj = malloc ( (sizeof(float*) * numberOfVertices ) );
    assert(adj);
    
    int row = 0;
    for (row = 0; row < numberOfVertices; row ++ )
    {
        adj[row] = malloc ( (sizeof(float) * numberOfVertices ) );
    }
    
    int col;
    //set the whole matrix to infinity
    for (row = 0; row < numberOfVertices; row++){
        for (col = 0; col < numberOfVertices; col++){
            adj[row][col] = INFINITY;
        } 
    }   
   
    int a, b;
    float w;
    i = 0;
    //Keep reading the file
    while(fscanf(fp, "%d" "%d" "%f", &a, &b, &w)!=EOF){
        adj[a][b] = w;
        adj[b][a] = w;
    }
   
    fclose(fp);
    return CreateGraph(mh, adj);
}

struct MinHeapNode* extractMin(struct MinHeap* mh){
    if(mh->size == 0)
        return NULL;
    
    struct MinHeapNode* root = mh -> heap[1];
    mh->heap[1] = mh->heap[mh->size-1];
    mh->size--;
    if( mh->size != 1 )
    {
        MinHeapify(mh, 1);
    }
    
 
    return root;
}



List* prim(struct Graph* g){
    int v;
    MinHeap* mh = g->minHeap;
    List* l = CreateList();
    
    int original_size = mh->size;
    mh->heap[1]->key = 0;
    int j;
    int i=0;
    int test=0;
    int k = 0; 
    
    while(mh -> size != 1){
        //do extract-min
        struct MinHeapNode* mhn = extractMin(mh); 
        addNodeBack(l, mhn->v, mhn->key, mhn->parent);
        int u = mhn -> v;
        
        for(i=1; i<mh ->size; i++){
            
            int v = mh -> heap[i] -> v;
            
            //do if v ε Q abd w(v) < key[v]
            if(g->adjMatrix[u][v] < mh->heap[i] -> key){
              
                //then key[v] <- w(u, v)
                mh->heap[i] -> key = g -> adjMatrix[u][v];
                mh->heap[i] -> parent = u;
                
                for (j= i; j > 0; j= j/2)
                    MinHeapify(mh, j);
            }
        }
    }
    return removeFirstNode(l);
}       

int main() 
{
     Graph* g = ReadFile("dense_100000.txt");
     List* l = prim(g);
     writeFile(l, "solution_dense.txt");

    return 0;
}








