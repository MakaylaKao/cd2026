#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    unsigned char ch;
    int count;
    struct Node *next;
} Node;

/* Find a character in the linked list */
Node *find_char(Node *head, unsigned char target) {
    Node *current = head;
    while (current != NULL) {
        if (current->ch == target) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* Append a new node to the end of the linked list */
void append_node(Node **head, unsigned char ch) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *current;

    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    new_node->ch = ch;
    new_node->count = 1;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
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
    Node *found;
    int c;

    /* Read the source code file itself */
    fp = fopen(__FILE__, "rb");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    while ((c = fgetc(fp)) != EOF) {
        found = find_char(head, (unsigned char)c);
        if (found != NULL) {
            found->count++;
        } else {
            append_node(&head, (unsigned char)c);
        }
    }

    fclose(fp);

    print_list(head);
    free_list(head);

    return 0;
}