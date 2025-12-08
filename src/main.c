#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_CAP 100
#define MAX_STR_LEN 256

typedef struct {
    int a;
    char b[MAX_STR_LEN];
    double c;
    char d[MAX_STR_LEN];
} Element;

typedef struct GraphNode {
    Element data;
    struct GraphNode **neighbors;
    int neighbor_count;
    int neighbor_capacity;
} GraphNode;

typedef struct {
    GraphNode **nodes;
    int node_count;
    int node_capacity;
} Graph;

Graph *create_graph(void) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Memory allocation failed for graph\n");
        return NULL;
    }

    graph->node_capacity = 10;
    graph->node_count = 0;
    graph->nodes = (GraphNode **)malloc(sizeof(GraphNode *) * graph->node_capacity);

    if (!graph->nodes) {
        fprintf(stderr, "Memory allocation failed for graph nodes\n");
        free(graph);
        return NULL;
    }

    return graph;
}

void add_node_to_graph(Graph *graph, int a, const char *b, double c, const char *d) {
    if (graph->node_count >= ARRAY_CAP) {
        fprintf(stderr, "Graph is full\n");
        return;
    }

    if (graph->node_count >= graph->node_capacity) {
        graph->node_capacity *= 2;
        GraphNode **temp = (GraphNode **)realloc(graph->nodes, sizeof(GraphNode *) * graph->node_capacity);
        if (!temp) {
            fprintf(stderr, "Memory reallocation failed for graph nodes\n");
            return;
        }
        graph->nodes = temp;
    }

    GraphNode *new_node = (GraphNode *)malloc(sizeof(GraphNode));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for graph node\n");
        return;
    }

    new_node->data.a = a;
    strncpy(new_node->data.b, b, MAX_STR_LEN - 1);
    new_node->data.b[MAX_STR_LEN - 1] = '\0';
    new_node->data.c = c;
    strncpy(new_node->data.d, d, MAX_STR_LEN - 1);
    new_node->data.d[MAX_STR_LEN - 1] = '\0';
    new_node->neighbor_capacity = 5;
    new_node->neighbor_count = 0;
    new_node->neighbors = (GraphNode **)malloc(sizeof(GraphNode *) * new_node->neighbor_capacity);

    if (!new_node->neighbors) {
        fprintf(stderr, "Memory allocation failed for neighbors\n");
        free(new_node);
        return;
    }

    graph->nodes[graph->node_count] = new_node;
    graph->node_count++;
}

void add_edge(Graph *graph, int from_idx, int to_idx) {
    if (!graph || from_idx >= graph->node_count || to_idx >= graph->node_count || from_idx < 0 || to_idx < 0) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }

    GraphNode *from_node = graph->nodes[from_idx];

    if (from_node->neighbor_count >= from_node->neighbor_capacity) {
        from_node->neighbor_capacity *= 2;
        GraphNode **temp = (GraphNode **)realloc(from_node->neighbors, sizeof(GraphNode *) * from_node->neighbor_capacity);
        if (!temp) {
            fprintf(stderr, "Memory reallocation failed for neighbors\n");
            return;
        }
        from_node->neighbors = temp;
    }

    from_node->neighbors[from_node->neighbor_count] = graph->nodes[to_idx];
    from_node->neighbor_count++;
}

void transfer_graph_to_array(Graph *graph, Element *array, int *array_count) {
    if (!graph || !array_count) {
        fprintf(stderr, "Invalid graph or array\n");
        return;
    }

    if (graph->node_count > ARRAY_CAP) {
        fprintf(stderr, "Graph has more nodes than array capacity\n");
        return;
    }

    *array_count = 0;
    for (int i = 0; i < graph->node_count; i++) {
        array[*array_count] = graph->nodes[i]->data;
        (*array_count)++;
    }
}

int compare_elements(const void *a, const void *b) {
    Element *elem_a = (Element *)a;
    Element *elem_b = (Element *)b;

    if (elem_a->a < elem_b->a)
        return -1;
    if (elem_a->a > elem_b->a)
        return 1;
    return 0;
}

void sort_array(Element *array, int array_count) {
    if (!array || array_count == 0) {
        fprintf(stderr, "Invalid array or empty array\n");
        return;
    }

    qsort(array, array_count, sizeof(Element), compare_elements);
}

void display_array(Element *array, int array_count) {
    if (!array) {
        fprintf(stderr, "Invalid array\n");
        return;
    }

    printf("\n--- Array Contents ---\n");
    printf("Count: %d\n", array_count);
    for (int i = 0; i < array_count; i++) {
        printf("[%d] a=%d, b=%s, c=%.2f, d=%s\n", i, array[i].a, array[i].b, array[i].c, array[i].d);
    }
    printf("\n");
}

void display_graph(Graph *graph) {
    if (!graph) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    printf("\n--- Graph Nodes ---\n");
    printf("Node count: %d\n", graph->node_count);
    for (int i = 0; i < graph->node_count; i++) {
        printf("[%d] a=%d, b=%s, c=%.2f, d=%s (neighbors: %d)\n", i,
               graph->nodes[i]->data.a, graph->nodes[i]->data.b,
               graph->nodes[i]->data.c, graph->nodes[i]->data.d,
               graph->nodes[i]->neighbor_count);
    }
    printf("\n");
}

void free_graph(Graph *graph) {
    if (!graph)
        return;

    for (int i = 0; i < graph->node_count; i++) {
        free(graph->nodes[i]->neighbors);
        free(graph->nodes[i]);
    }
    free(graph->nodes);
    free(graph);
}

int main(void) {
    Graph *graph = create_graph();
    if (!graph) {
        fprintf(stderr, "Failed to initialize graph\n");
        return 1;
    }

    Element array[ARRAY_CAP];
    int array_count = 0;

    add_node_to_graph(graph, 5, "apple", 15.5, "pear");
    add_node_to_graph(graph, 3, "cat", 12.3, "bird");
    add_node_to_graph(graph, 7, "dog", 20.1, "fish");
    add_node_to_graph(graph, 2, "ant", 9.8, "moth");
    add_node_to_graph(graph, 9, "zebra", 25.6, "lion");

    if (graph->node_count >= 2) {
        add_edge(graph, 0, 1);
        add_edge(graph, 0, 2);
        add_edge(graph, 1, 3);
    }

    int running = 1;

    while (running) {
        printf("\n=== Menu ===\n");
        printf("1. Display Graph\n");
        printf("2. Transfer Graph to Array\n");
        printf("3. Sort Array\n");
        printf("4. Display Array\n");
        printf("5. Exit\n");
        printf("Choose an option: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice) {
        case 1:
            display_graph(graph);
            break;

        case 2:
            transfer_graph_to_array(graph, array, &array_count);
            printf("Graph transferred to array successfully\n");
            break;

        case 3:
            if (array_count == 0) {
                printf("Array is empty. Transfer graph to array first.\n");
            } else {
                sort_array(array, array_count);
                printf("Array sorted successfully\n");
            }
            break;

        case 4:
            display_array(array, array_count);
            break;

        case 5:
            running = 0;
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    free_graph(graph);

    printf("Program terminated.\n");
    return 0;
}
