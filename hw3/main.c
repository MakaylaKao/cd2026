#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 10000
#define LEXEME_SIZE 128
#define OUTPUT_SIZE 20000

typedef enum TokenType {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_END,
    TOKEN_INVALID
} TokenType;

TokenType token;

char input_text[INPUT_SIZE];
int input_pos = 0;
int input_len = 0;

char lexeme[LEXEME_SIZE];

char output[OUTPUT_SIZE];
int output_len = 0;

int parse_error = 0;

int is_digit_char(int c) {
    return c >= '0' && c <= '9';
}

int is_space_char(int c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

void append_output(const char *text) {
    int i = 0;

    while (text[i] != '\0') {
        if (output_len >= OUTPUT_SIZE - 1) {
            parse_error = 1;
            return;
        }

        output[output_len] = text[i];
        output_len++;
        i++;
    }

    output[output_len] = '\0';
}

int next_input_char(void) {
    if (input_pos >= input_len) {
        return EOF;
    }

    return (unsigned char)input_text[input_pos++];
}

void push_back_input_char(void) {
    if (input_pos > 0) {
        input_pos--;
    }
}

TokenType scanner(void) {
    int c;
    int len;

    c = next_input_char();

    while (c != EOF && is_space_char(c)) {
        c = next_input_char();
    }

    if (c == EOF) {
        lexeme[0] = '\0';
        return TOKEN_END;
    }

    if (is_digit_char(c)) {
        len = 0;

        while (c != EOF && is_digit_char(c)) {
            if (len < LEXEME_SIZE - 1) {
                lexeme[len] = (char)c;
                len++;
            }

            c = next_input_char();
        }

        lexeme[len] = '\0';

        if (c != EOF) {
            push_back_input_char();
        }

        return TOKEN_NUMBER;
    }

    lexeme[0] = (char)c;
    lexeme[1] = '\0';

    if (c == '+') {
        return TOKEN_PLUS;
    }

    if (c == '(') {
        return TOKEN_LEFT_PAREN;
    }

    if (c == ')') {
        return TOKEN_RIGHT_PAREN;
    }

    return TOKEN_INVALID;
}

void advance_token(void) {
    token = scanner();
}

void syntax_error(void) {
    parse_error = 1;
}

void parse_S(void);
void parse_S_tail(void);
void parse_E(void);

void parse_S(void) {
    if (parse_error) {
        return;
    }

    if (token == TOKEN_NUMBER || token == TOKEN_LEFT_PAREN) {
        append_output("S -> E S'\n");
        parse_E();
        parse_S_tail();
        return;
    }

    syntax_error();
}

void parse_S_tail(void) {
    if (parse_error) {
        return;
    }

    if (token == TOKEN_PLUS) {
        append_output("S' -> + S\n");
        advance_token();
        parse_S();
        return;
    }

    if (token == TOKEN_RIGHT_PAREN || token == TOKEN_END) {
        return;
    }

    syntax_error();
}

void parse_E(void) {
    if (parse_error) {
        return;
    }

    if (token == TOKEN_NUMBER) {
        append_output(lexeme);
        append_output("\n");
        advance_token();
        return;
    }

    if (token == TOKEN_LEFT_PAREN) {
        append_output("E -> ( S )\n");

        advance_token();
        parse_S();

        if (parse_error) {
            return;
        }

        if (token != TOKEN_RIGHT_PAREN) {
            syntax_error();
            return;
        }

        advance_token();
        return;
    }

    syntax_error();
}

int main(void) {
    output[0] = '\0';

    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Parse error\n");
        return 0;
    }

    input_len = (int)strlen(input_text);
    input_pos = 0;

    advance_token();
    parse_S();

    if (!parse_error && token != TOKEN_END) {
        syntax_error();
    }

    if (parse_error) {
        printf("Parse error\n");
    } else {
        printf("%s", output);
    }

    return 0;
}