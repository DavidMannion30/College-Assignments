#include "t1.h"
#include <stdio.h>
#include <stdlib.h>



Graph* create_graph(int num_nodes) 
{
    Graph* graph = malloc(sizeof(Graph));
    graph->numNodes = num_nodes;

    for (int i = 0; i < num_nodes; i++)
     {
        graph->nodes[i] = malloc(sizeof(Node));
        graph->nodes[i]->vertex = i;
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->head = NULL;
    }

    return graph;
}


void add_edge(Graph* g, int from, int to) 
{
    AdjNode* newNode = malloc(sizeof(AdjNode));

    newNode->vertex = to;
    newNode->next = NULL;

    AdjNode** current = &(g->nodes[from]->head);

    while (*current != NULL && (*current)->vertex < to)
    {
        current = &((*current)->next);
    }

    newNode->next = *current;
    *current = newNode;
}



typedef struct {
    Node* array[MAX_SIZE];
    int front;
    int back;
} Queue;



void initializeQueue (Queue* queue)
{
    queue->front = 0;
    queue->back = 0;
}



int isQueueEmpty(Queue* queue)
{
    return queue->front == queue->back;
}



void enqueue(Queue* queue, Node* node)
{
   queue->array[queue->back++] = node;
}



Node* dequeue(Queue* queue)
{
    return queue->array[queue->front++];  
}


void bfs(Graph* g, int origin) {
    Queue queue;
    initializeQueue(&queue);

    Node* start = g->nodes[origin];
    enqueue(&queue, start);
    start->visited = 1;

    printf("BFS ");
    while (!isQueueEmpty(&queue))
     {
        Node* current = dequeue(&queue);
        printf(" %c ", (char)(current->vertex + 'A'));

        AdjNode* neighbor = current->head;
        while (neighbor) 
        {
            Node* nextNode = g->nodes[neighbor->vertex];
            if (!nextNode->visited) 
            {
                nextNode->visited = 1;
                enqueue(&queue, nextNode);
            }
            neighbor = neighbor->next;
        }
    }
    printf("\n");

    for (int i = 0; i < g->numNodes; i++)
        g->nodes[i]->visited = 0;
}



typedef struct 
{
    Node* array[MAX_SIZE];
    int top;
} Stack;



void initializeStack (Stack* stack)
    {
        stack->top = -1;
    }



int isStackEmpty(Stack* stack)
{
    return stack->top == -1;
}



void push(Stack* stack, Node* node)
{
    stack->array[++(stack->top)] = node;
}



Node* pop(Stack* stack)
{
    return stack->array[(stack->top)--];
}

void dfs(Graph* g, int origin) {
    Stack s;
    initializeStack(&s);

    Node* start = g->nodes[origin];
    push(&s, start);
    start->visited = 1;

    printf("DFS:");
    while (!isStackEmpty(&s)) {
        Node* current = pop(&s);
        printf(" %c ", (char)(current->vertex + 'A'));

        // Collect neighbors as Node*
        Node* neighbors[MAX_SIZE];
        int count = 0;

        for (AdjNode* n = current->head; n != NULL; n = n->next) {
            Node* nextNode = g->nodes[n->vertex];
            if (!nextNode->visited) {
                neighbors[count++] = nextNode;  // <-- Node*, not AdjNode*
            }
        }

        // Push neighbors in reverse order for alphabetical DFS
        for (int i = count - 1; i >= 0; i--) {
            neighbors[i]->visited = 1;       // <-- set Node->visited
            push(&s, neighbors[i]);          // <-- push Node*
        }
    }

    printf("\n");

    // Reset visited flags
    for (int i = 0; i < g->numNodes; i++)
        g->nodes[i]->visited = 0;
}


//---------------- Delete Graph ----------------

void delete_graph(Graph* g) {
    for (int i = 0; i < g->numNodes; i++) {
        AdjNode* current = g->nodes[i]->head;
        while (current) {
            AdjNode* tmp = current;
            current = current->next;
            free(tmp);
        }
        free(g->nodes[i]);
    }
    free(g);
}



