#include <stdio.h>
#include <stdlib.h>

/* Linked list node structure */
typedef struct Node {
    unsigned char ch;      /* character stored in this node */
    int count;             /* count field kept from HW0 */
    struct Node *next;     /* pointer to next node */
} Node;

/* Search a character in the linked list */
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

/* Append a new node at the end of the linked list */
void append_node(Node **head, unsigned char ch) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *current;

    if (new_node == NULL) {
        printf("Memory allocation failed\n");
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

/* Print one character safely */
void print_char(unsigned char ch) {
    if (ch == '\n') {
        printf("\\n");
    }
    else if (ch == '\r') {
        printf("\\r");
    }
    else if (ch == '\t') {
        printf("\\t");
    }
    else if (ch == ' ') {
        printf(" ");
    }
    else if (ch >= 32 && ch <= 126) {
        printf("%c", ch);
    }
    else {
        printf("0x%02X", ch);
    }
}

/* Print all characters stored in the linked list */
void print_list(Node *head) {
    Node *current = head;

    printf("Output => ");

    while (current != NULL) {
        print_char(current->ch);

        if (current->next != NULL) {
            printf(", ");
        }

        current = current->next;
    }

    printf("\n");
}

/* Free memory used by linked list */
void free_list(Node *head) {
    Node *current = head;
    Node *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

/* Main scanner program */
int main(void) {
    FILE *fp;
    int c;
    Node *head = NULL;
    Node *found;

    /* Open main.c */
    fp = fopen("main.c", "rb");
    if (fp == NULL) {
        printf("Cannot open file main.c\n");
        return 1;
    }

    /* Read file character by character */
    while ((c = fgetc(fp)) != EOF) {
        found = find_char(head, (unsigned char)c);

        if (found != NULL) {
            found->count++;
        }
        else {
            append_node(&head, (unsigned char)c);
        }
    }

    fclose(fp);

    /* Output characters from linked list */
    print_list(head);

    /* Free memory */
    free_list(head);

    return 0;
}