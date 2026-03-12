#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    unsigned char ch;
    int count;
    struct Node *next;
} Node;

/* Create and initialize a new node */
Node *create_node(unsigned char ch) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    new_node->ch = ch;
    new_node->count = 1;
    new_node->next = NULL;
    return new_node;
}

/* Append a node to the end of the linked list */
void append_node(Node **head, Node **tail, Node *new_node) {
    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

/* Print the counting result */
void print_list(Node *head) {
    Node *current = head;

    while (current != NULL) {
        if (current->ch == '\n') {
            printf("\\n : %d\n", current->count);
        } else if (current->ch == '\t') {
            printf("\\t : %d\n", current->count);
        } else if (current->ch == '\r') {
            printf("\\r : %d\n", current->count);
        } else if (current->ch == ' ') {
            printf("space : %d\n", current->count);
        } else if (current->ch >= 32 && current->ch <= 126) {
            printf("%c : %d\n", current->ch, current->count);
        } else {
            printf("0x%02X : %d\n", current->ch, current->count);
        }
        current = current->next;
    }
}

/* Free all nodes in the linked list */
void free_list(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    FILE *fp;
    Node *head = NULL;
    Node *tail = NULL;
    Node *table[256] = {0};   /* Lookup table for O(1) average access */
    int c;

    /* Read the source code file itself */
    fp = fopen(__FILE__, "rb");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    while ((c = fgetc(fp)) != EOF) {
        unsigned char ch = (unsigned char)c;

        if (table[ch] != NULL) {
            table[ch]->count++;
        } else {
            Node *new_node = create_node(ch);
            append_node(&head, &tail, new_node);
            table[ch] = new_node;
        }
    }

    fclose(fp);

    print_list(head);
    free_list(head);

    return 0;
}