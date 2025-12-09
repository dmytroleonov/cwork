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
    printf("Edge added successfully\n");
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
    printf("Graph transferred to array successfully\n");
}

int compare_elements(const void *a, const void *b) {
    Element *elem_a = (Element *)a;
    Element *elem_b = (Element *)b;

    if (elem_a->a != elem_b->a) {
        return (elem_a->a < elem_b->a) ? -1 : 1;
    }

    int b_cmp = strcmp(elem_a->b, elem_b->b);
    if (b_cmp != 0) {
        return b_cmp;
    }

    if (elem_a->c != elem_b->c) {
        return (elem_a->c < elem_b->c) ? -1 : 1;
    }

    return strcmp(elem_a->d, elem_b->d);
}

void sort_array(Element *array, int array_count) {
    if (!array || array_count == 0) {
        fprintf(stderr, "Invalid array or empty array\n");
        return;
    }

    qsort(array, array_count, sizeof(Element), compare_elements);
    printf("Array sorted successfully\n");
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

void process_array(Element *array, int array_count) {
    if (array_count == 0) {
        fprintf(stderr, "Array is empty\n");
        return;
    }

    printf("\n--- Processing Array ---\n");
    printf("Elements where b has 3 characters and d has 4 characters:\n");

    int found = 0;
    for (int i = 0; i < array_count; i++) {
        if (strlen(array[i].b) == 3 && strlen(array[i].d) == 4) {
            double difference = array[i].c - array[i].a;
            printf("Index %d: b=%s, d=%s, difference (c - a) = %.2f\n", i, array[i].b, array[i].d, difference);
            found = 1;
        }
    }

    if (!found) {
        printf("No elements found matching the criteria\n");
    }
    printf("\n");
}

void save_array_to_file(Element *array, int array_count, const char *filename) {
    if (!array || !filename) {
        fprintf(stderr, "Invalid array or filename\n");
        return;
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }

    if (fwrite(&array_count, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Failed to write array count\n");
        fclose(file);
        return;
    }

    if (fwrite(array, sizeof(Element), array_count, file) != (size_t)array_count) {
        fprintf(stderr, "Failed to write array data\n");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Array saved to file: %s\n", filename);
}

void display_graph(Graph *graph) {
    if (!graph) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    printf("\n--- Graph Nodes ---\n");
    printf("Node count: %d\n", graph->node_count);
    for (int i = 0; i < graph->node_count; i++) {
        printf("[%d] a=%d, b=%s, c=%.2f, d=%s\n", i,
               graph->nodes[i]->data.a, graph->nodes[i]->data.b,
               graph->nodes[i]->data.c, graph->nodes[i]->data.d);

        if (graph->nodes[i]->neighbor_count > 0) {
            printf("    Neighbors: ");
            for (int j = 0; j < graph->nodes[i]->neighbor_count; j++) {
                for (int k = 0; k < graph->node_count; k++) {
                    if (graph->nodes[k] == graph->nodes[i]->neighbors[j]) {
                        printf("%d", k);
                        if (j < graph->nodes[i]->neighbor_count - 1) {
                            printf(", ");
                        }
                        break;
                    }
                }
            }
            printf("\n");
        }
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

int input_int(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            return value;
        }
        fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
        while (getchar() != '\n') {
        };
    }
}

double input_double(const char *prompt) {
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1) {
            return value;
        }
        fprintf(stderr, "Invalid input. Please enter a valid double.\n");
        while (getchar() != '\n') {
        };
    }
}

void input_string(const char *prompt, char *buffer, size_t buffer_size) {
    while (1) {
        printf("%s", prompt);
        char format[16];
        snprintf(format, sizeof(format), "%%%zus", buffer_size - 1);
        if (scanf(format, buffer) == 1) {
            return;
        }
        fprintf(stderr, "Invalid input. Please enter a valid string.\n");
        while (getchar() != '\n') {
        };
    }
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
        printf("2. Add Node to Graph\n");
        printf("3. Add Edge to Graph\n");
        printf("4. Transfer Graph to Array\n");
        printf("5. Sort Array\n");
        printf("6. Display Array\n");
        printf("7. Process Array\n");
        printf("8. Save Array to File\n");
        printf("9. Exit\n");
        printf("Choose an option: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            while (getchar() != '\n') {
            };
            continue;
        }

        switch (choice) {
        case 1:
            display_graph(graph);
            break;

        case 2: {
            if (graph->node_count >= ARRAY_CAP) {
                printf("Graph is full\n");
                break;
            }
            int a = input_int("Enter a (int): ");
            char b[MAX_STR_LEN];
            input_string("Enter b (string): ", b, MAX_STR_LEN);
            double c = input_double("Enter c (double): ");
            char d[MAX_STR_LEN];
            input_string("Enter d (string): ", d, MAX_STR_LEN);
            add_node_to_graph(graph, a, b, c, d);
            printf("Node added successfully\n");
            break;
        }

        case 3: {
            if (graph->node_count < 2) {
                printf("Need at least 2 nodes to add an edge\n");
                break;
            }
            int from = input_int("Enter source node index (0-%d): ");
            int to = input_int("Enter destination node index (0-%d): ");
            add_edge(graph, from, to);
            break;
        }

        case 4:
            transfer_graph_to_array(graph, array, &array_count);
            break;

        case 5:
            if (array_count == 0) {
                printf("Array is empty. Transfer graph to array first.\n");
            } else {
                sort_array(array, array_count);
            }
            break;

        case 6:
            display_array(array, array_count);
            break;

        case 7:
            process_array(array, array_count);
            break;

        case 8:
            save_array_to_file(array, array_count, "array.bin");
            break;

        case 9:
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
