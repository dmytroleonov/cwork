// Dmytro Leonov
// C standard used: c11
// Tested and compiled on linux with: gcc -Wall -Wextra -std=c11 src/main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of elements in array and graph nodes
#define MAX_ARRAY_LEN 100
// Maximum length of string fields in Element
#define MAX_STR_LEN 256

// Structure representing the data stored in each graph node
typedef struct {
    int a;               // integer field
    char b[MAX_STR_LEN]; // string field
    double c;            // double field
    char d[MAX_STR_LEN]; // string field
} Element;

// Graph node structure for dynamic graph representation
typedef struct GraphNode {
    Element data;                 // The actual element stored in the node
    struct GraphNode **neighbors; // Dynamic array of pointers to neighbor nodes
    size_t neighbor_count;        // Current number of neighbors
    size_t neighbor_capacity;     // Allocated capacity for neighbors array
} GraphNode;

// Graph structure containing all nodes
typedef struct {
    GraphNode **nodes;    // Dynamic array of pointers to graph nodes
    size_t node_count;    // Number of nodes currently in graph
    size_t node_capacity; // Allocated capacity for nodes array
} Graph;

// Create and initialize an empty graph
Graph *create_graph(void) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Memory allocation failed for graph\n");
        return NULL;
    }

    // initial capacity
    graph->node_capacity = 10;
    graph->node_count = 0;
    // Allocate memory for nodes array
    graph->nodes = (GraphNode **)malloc(sizeof(GraphNode *) * graph->node_capacity);
    if (!graph->nodes) {
        fprintf(stderr, "Memory allocation failed for graph nodes\n");
        free(graph);
        return NULL;
    }

    return graph;
}

// Add a node with given data to the graph
void add_node_to_graph(Graph *graph, int a, const char *b, double c, const char *d) {
    // Check if graph reached maximum allowed nodes
    if (graph->node_count >= MAX_ARRAY_LEN) {
        fprintf(stderr, "Graph is full\n");
        return;
    }

    // Resize nodes array if capacity exceeded
    if (graph->node_count >= graph->node_capacity) {
        graph->node_capacity *= 2;
        GraphNode **temp = (GraphNode **)realloc(graph->nodes, sizeof(GraphNode *) * graph->node_capacity);
        if (!temp) {
            fprintf(stderr, "Memory reallocation failed for graph nodes\n");
            return;
        }
        graph->nodes = temp;
    }

    // Allocate memory for new node
    GraphNode *new_node = (GraphNode *)malloc(sizeof(GraphNode));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for graph node\n");
        return;
    }

    // Initialize node data
    new_node->data.a = a;
    strncpy(new_node->data.b, b, MAX_STR_LEN - 1);
    new_node->data.b[MAX_STR_LEN - 1] = '\0'; // ensure null termination
    new_node->data.c = c;
    strncpy(new_node->data.d, d, MAX_STR_LEN - 1);
    new_node->data.d[MAX_STR_LEN - 1] = '\0';

    // Initialize neighbors array
    new_node->neighbor_capacity = 5; // initial neighbors capacity
    new_node->neighbor_count = 0;
    new_node->neighbors = (GraphNode **)malloc(sizeof(GraphNode *) * new_node->neighbor_capacity);
    if (!new_node->neighbors) {
        fprintf(stderr, "Memory allocation failed for neighbors\n");
        free(new_node);
        return;
    }

    // Add new node to graph
    graph->nodes[graph->node_count] = new_node;
    graph->node_count++;
    printf("Node added successfully\n");
}

// Add an edge from node at index from_idx to node at index to_idx
void add_edge(Graph *graph, size_t from_idx, size_t to_idx) {
    // Validate indices
    if (!graph || from_idx >= graph->node_count || to_idx >= graph->node_count) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }

    GraphNode *from_node = graph->nodes[from_idx];

    // Resize neighbors array if capacity exceeded
    if (from_node->neighbor_count >= from_node->neighbor_capacity) {
        from_node->neighbor_capacity *= 2;
        GraphNode **temp = (GraphNode **)realloc(from_node->neighbors, sizeof(GraphNode *) * from_node->neighbor_capacity);
        if (!temp) {
            fprintf(stderr, "Memory reallocation failed for neighbors\n");
            return;
        }
        from_node->neighbors = temp;
    }

    // Add neighbor pointer
    from_node->neighbors[from_node->neighbor_count] = graph->nodes[to_idx];
    from_node->neighbor_count++;
    printf("Edge added successfully\n");
}

// Copy all graph nodes into a 1D array of Elements
void transfer_graph_to_array(Graph *graph, Element *array, size_t *array_count) {
    if (!graph || !array_count) {
        fprintf(stderr, "Invalid graph or array\n");
        return;
    }

    if (graph->node_count > MAX_ARRAY_LEN) {
        fprintf(stderr, "Graph has more nodes than array capacity\n");
        return;
    }

    *array_count = 0;
    for (size_t i = 0; i < graph->node_count; i++) {
        // copy node data
        array[*array_count] = graph->nodes[i]->data;
        (*array_count)++;
    }
    printf("Graph transferred to array successfully\n");
}

// Comparison function for qsort: sorts by a, then b, then c, then d
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

// Sort an array of Elements
void sort_array(Element *array, size_t array_count) {
    if (array_count == 0) {
        fprintf(stderr, "Array is empty\n");
        return;
    }

    qsort(array, array_count, sizeof(Element), compare_elements);
    printf("Array sorted successfully\n");
}

// Display contents of an Element array
void display_array(Element *array, size_t array_count) {
    if (array_count == 0) {
        fprintf(stderr, "Array is empty\n");
        return;
    }

    printf("\n--- Array Contents ---\n");
    printf("Count: %zu\n", array_count);
    for (size_t i = 0; i < array_count; i++) {
        printf("[%zu] a=%d, b=%s, c=%.2f, d=%s\n",
               i, array[i].a, array[i].b, array[i].c, array[i].d);
    }
    printf("\n");
}

// Process array: compute c - a for elements with b length 3 and d length 4
void process_array(Element *array, size_t array_count) {
    if (array_count == 0) {
        fprintf(stderr, "Array is empty\n");
        return;
    }

    printf("\n--- Processing Array ---\n");
    printf("c - a for elements where b has 3 characters and d has 4 characters:\n");

    int found = 0;
    for (size_t i = 0; i < array_count; i++) {
        if (strlen(array[i].b) == 3 && strlen(array[i].d) == 4) {
            double difference = array[i].c - array[i].a;
            printf("Index %zu: b=%s, d=%s, difference (c - a) = %.2f\n",
                   i, array[i].b, array[i].d, difference);
            found = 1;
        }
    }

    if (!found) {
        printf("No elements found matching the criteria\n");
    }
    printf("\n");
}

// Save Element array to a binary file
void save_array_to_file(Element *array, size_t array_count, const char *filename) {
    if (!filename) {
        fprintf(stderr, "Invalid filename\n");
        return;
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }

    // Write the count first
    if (fwrite(&array_count, sizeof(size_t), 1, file) != 1) {
        fprintf(stderr, "Failed to write array count\n");
        fclose(file);
        return;
    }

    // Write array contents
    if (fwrite(array, sizeof(Element), array_count, file) != array_count) {
        fprintf(stderr, "Failed to write array data\n");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Array saved to file: %s\n", filename);
}

// Load Element array from a binary file
void load_array_from_file(Element *array, size_t *array_count, const char *filename) {
    if (!filename) {
        fprintf(stderr, "Invalid filename\n");
        return;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file for reading\n");
        return;
    }

    // Read count first
    if (fread(array_count, sizeof(size_t), 1, file) != 1) {
        fprintf(stderr, "Failed to read array count\n");
        fclose(file);
        return;
    }

    if (*array_count > MAX_ARRAY_LEN) {
        fprintf(stderr, "Invalid array count in file\n");
        fclose(file);
        *array_count = 0;
        return;
    }

    // Read array contents
    if (fread(array, sizeof(Element), *array_count, file) != *array_count) {
        fprintf(stderr, "Failed to read array data\n");
        fclose(file);
        *array_count = 0;
        return;
    }

    fclose(file);
    printf("Array loaded from file: %s (count: %zu)\n", filename, *array_count);
}

// Display all graph nodes and their neighbors
void display_graph(Graph *graph) {
    if (!graph) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    printf("\n--- Graph Nodes ---\n");
    printf("Node count: %zu\n", graph->node_count);

    for (size_t i = 0; i < graph->node_count; i++) {
        printf("[%zu] a=%d, b=%s, c=%.2f, d=%s\n",
               i,
               graph->nodes[i]->data.a,
               graph->nodes[i]->data.b,
               graph->nodes[i]->data.c,
               graph->nodes[i]->data.d);

        // Print neighbors
        if (graph->nodes[i]->neighbor_count > 0) {
            printf("    Neighbors: ");
            for (size_t j = 0; j < graph->nodes[i]->neighbor_count; j++) {
                for (size_t k = 0; k < graph->node_count; k++) {
                    if (graph->nodes[k] == graph->nodes[i]->neighbors[j]) {
                        printf("%zu", k);
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

// Free all allocated memory for graph
void free_graph(Graph *graph) {
    if (!graph) {
        return;
    }

    for (size_t i = 0; i < graph->node_count; i++) {
        free(graph->nodes[i]->neighbors);
        free(graph->nodes[i]);
    }
    free(graph->nodes);
    free(graph);
}

// Functions to read user input safely
size_t input_size_t(const char *prompt) {
    size_t value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%zu", &value) == 1) {
            return value;
        }
        fprintf(stderr, "Invalid input. Please enter a valid size.\n");
        while (getchar() != '\n') {
        }; // discard invalid input
    }
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
        // dynamic format string
        snprintf(format, sizeof(format), "%%%zus", buffer_size - 1);
        if (scanf(format, buffer) == 1) {
            return;
        }
        fprintf(stderr, "Invalid input. Please enter a valid string.\n");
        while (getchar() != '\n') {
        };
    }
}

// Main program: menu-driven graph and array operations
int main(void) {
    // initialize graph
    Graph *graph = create_graph();
    if (!graph) {
        return 1;
    }

    Element array[MAX_ARRAY_LEN];
    size_t array_count = 0;

    // Add initial nodes
    add_node_to_graph(graph, 5, "apple", 15.5, "pear");
    add_node_to_graph(graph, 2, "cat", 12.3, "bird");
    add_node_to_graph(graph, 7, "dog", 20.1, "fish");
    add_node_to_graph(graph, 2, "ant", 9.8, "moth");
    add_node_to_graph(graph, 9, "zebra", 25.6, "lion");

    // Add initial edges
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
        printf("9. Load Array from File\n");
        printf("10. Exit\n");
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
            if (graph->node_count >= MAX_ARRAY_LEN) {
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
            break;
        }
        case 3: {
            if (graph->node_count < 2) {
                printf("Need at least 2 nodes\n");
                break;
            }
            size_t from = input_size_t("Enter source node index: ");
            size_t to = input_size_t("Enter destination node index: ");
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
                load_array_from_file(array, &array_count, "array.bin");
                break;
            case 10:
                running = 0;
                break;
            default:
                printf("Invalid choice\n");
            }
        }
    }

    // free all allocated memory
    free_graph(graph);

    printf("Program terminated.\n");
    return 0;
}
