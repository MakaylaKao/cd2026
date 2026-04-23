#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TokenNode {
    char *lexeme;
    char *token;
    struct TokenNode *next;
} TokenNode;

typedef struct InputBuffer {
    char *data;
    int length;
    int pos;
} InputBuffer;

int is_letter(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int is_id_char(int c) {
    return is_letter(c) || is_digit(c) || c == '_';
}

int is_space_char(int c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

char *copy_string(const char *src) {
    size_t len = strlen(src);
    char *dst = (char *)malloc(len + 1);
    if (dst == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(dst, src);
    return dst;
}

void append_token(TokenNode **head, TokenNode **tail, const char *lexeme, const char *token) {
    TokenNode *new_node = (TokenNode *)malloc(sizeof(TokenNode));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    new_node->lexeme = copy_string(lexeme);
    new_node->token = copy_string(token);
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

const char *keyword_token(const char *word) {
    if (strcmp(word, "int") == 0) {
        return "TYPE_TOKEN";
    }
    if (strcmp(word, "main") == 0) {
        return "MAIN_TOKEN";
    }
    if (strcmp(word, "if") == 0) {
        return "IF_TOKEN";
    }
    if (strcmp(word, "else") == 0) {
        return "ELSE_TOKEN";
    }
    if (strcmp(word, "while") == 0) {
        return "WHILE_TOKEN";
    }
    return "ID_TOKEN";
}

int next_char(InputBuffer *input) {
    if (input->pos >= input->length) {
        return EOF;
    }
    return (unsigned char)input->data[input->pos++];
}

void push_back_char(InputBuffer *input) {
    if (input->pos > 0) {
        input->pos--;
    }
}

void scan_identifier_or_keyword(InputBuffer *input, int first_char, TokenNode **head, TokenNode **tail) {
    char buffer[1024];
    int len = 0;
    int c;

    buffer[len++] = (char)first_char;

    while ((c = next_char(input)) != EOF) {
        if (!is_id_char(c)) {
            push_back_char(input);
            break;
        }
        if (len < (int)sizeof(buffer) - 1) {
            buffer[len++] = (char)c;
        }
    }

    buffer[len] = '\0';
    append_token(head, tail, buffer, keyword_token(buffer));
}

void scan_number(InputBuffer *input, int first_char, TokenNode **head, TokenNode **tail) {
    char buffer[1024];
    int len = 0;
    int c;

    buffer[len++] = (char)first_char;

    while ((c = next_char(input)) != EOF) {
        if (!is_digit(c)) {
            push_back_char(input);
            break;
        }
        if (len < (int)sizeof(buffer) - 1) {
            buffer[len++] = (char)c;
        }
    }

    buffer[len] = '\0';
    append_token(head, tail, buffer, "LITERAL_TOKEN");
}

void scan_relop_or_assign(InputBuffer *input, int first_char, TokenNode **head, TokenNode **tail) {
    int c;
    char buffer[3];

    buffer[0] = (char)first_char;
    buffer[1] = '\0';
    buffer[2] = '\0';

    if (first_char == '=') {
        c = next_char(input);
        if (c == '=') {
            buffer[1] = '=';
            buffer[2] = '\0';
            append_token(head, tail, buffer, "EQUAL_TOKEN");
        } else {
            if (c != EOF) {
                push_back_char(input);
            }
            append_token(head, tail, buffer, "ASSIGN_TOKEN");
        }
        return;
    }

    if (first_char == '>') {
        c = next_char(input);
        if (c == '=') {
            buffer[1] = '=';
            buffer[2] = '\0';
            append_token(head, tail, buffer, "GREATEREQUAL_TOKEN");
        } else {
            if (c != EOF) {
                push_back_char(input);
            }
            append_token(head, tail, buffer, "GREATER_TOKEN");
        }
        return;
    }

    if (first_char == '<') {
        c = next_char(input);
        if (c == '=') {
            buffer[1] = '=';
            buffer[2] = '\0';
            append_token(head, tail, buffer, "LESSEQUAL_TOKEN");
        } else {
            if (c != EOF) {
                push_back_char(input);
            }
            append_token(head, tail, buffer, "LESS_TOKEN");
        }
    }
}

void scan_single_char_token(int c, TokenNode **head, TokenNode **tail) {
    char buffer[2];
    buffer[0] = (char)c;
    buffer[1] = '\0';

    if (c == '(') {
        append_token(head, tail, buffer, "LEFTPAREN_TOKEN");
    } else if (c == ')') {
        append_token(head, tail, buffer, "REFTPAREN_TOKEN");
    } else if (c == '{') {
        append_token(head, tail, buffer, "LEFTBRACE_TOKEN");
    } else if (c == '}') {
        append_token(head, tail, buffer, "REFTBRACE_TOKEN");
    } else if (c == ';') {
        append_token(head, tail, buffer, "SEMICOLON_TOKEN");
    } else if (c == '+') {
        append_token(head, tail, buffer, "PLUS_TOKEN");
    } else if (c == '-') {
        append_token(head, tail, buffer, "MINUS_TOKEN");
    }
}

void print_tokens(TokenNode *head) {
    TokenNode *current = head;
    while (current != NULL) {
        printf("%s: %s\n", current->lexeme, current->token);
        current = current->next;
    }
}

void free_tokens(TokenNode *head) {
    TokenNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->lexeme);
        free(temp->token);
        free(temp);
    }
}

void append_text(char **buffer, int *length, int *capacity, const char *text) {
    int text_len = (int)strlen(text);
    char *new_buffer;

    if (*length + text_len + 1 > *capacity) {
        while (*length + text_len + 1 > *capacity) {
            *capacity *= 2;
        }
        new_buffer = (char *)realloc(*buffer, (size_t)(*capacity));
        if (new_buffer == NULL) {
            printf("Memory allocation failed.\n");
            free(*buffer);
            exit(1);
        }
        *buffer = new_buffer;
    }

    memcpy(*buffer + *length, text, (size_t)text_len);
    *length += text_len;
    (*buffer)[*length] = '\0';
}

char *read_input_until_end_line(void) {
    char line[1024];
    int capacity = 4096;
    int length = 0;
    char *buffer = (char *)malloc((size_t)capacity);

    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    buffer[0] = '\0';

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strcmp(line, "END\n") == 0 || strcmp(line, "END\r\n") == 0) {
            break;
        }
        append_text(&buffer, &length, &capacity, line);
    }

    return buffer;
}

int main(void) {
    TokenNode *head = NULL;
    TokenNode *tail = NULL;
    InputBuffer input;
    int c;

    input.data = read_input_until_end_line();
    input.length = (int)strlen(input.data);
    input.pos = 0;

    while ((c = next_char(&input)) != EOF) {
        if (is_space_char(c)) {
            continue;
        }

        if (is_letter(c)) {
            scan_identifier_or_keyword(&input, c, &head, &tail);
        } else if (is_digit(c)) {
            scan_number(&input, c, &head, &tail);
        } else if (c == '=' || c == '>' || c == '<') {
            scan_relop_or_assign(&input, c, &head, &tail);
        } else if (c == '(' || c == ')' || c == '{' || c == '}' ||
                   c == ';' || c == '+' || c == '-') {
            scan_single_char_token(c, &head, &tail);
        }
    }

    print_tokens(head);
    free_tokens(head);
    free(input.data);
    return 0;
}
