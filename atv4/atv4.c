#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    NUMERO, PAREN_ESQ, PAREN_DIR, SOMA, SUB, MULT, DIV, EOF_TOKEN, ERRO
} TokenType;

typedef struct {
    TokenType tipo;
    char lexema[32];
    int posicao;
} Token;

typedef struct {
    const char *input;
    size_t pos;
} Lexer;

Lexer criar_lexer(const char *input) {
    Lexer lexer;
    lexer.input = input;
    lexer.pos = 0;
    return lexer;
}

Token proximo_token(Lexer *lexer) {
    while (lexer->input[lexer->pos] && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }

    if (lexer->input[lexer->pos] == '\0') {
        return (Token){EOF_TOKEN, "", (int)lexer->pos};
    }

    char c = lexer->input[lexer->pos];
    if (isdigit(c)) {
        size_t start = lexer->pos;
        while (isdigit(lexer->input[lexer->pos])) {
            lexer->pos++;
        }
        Token token = {NUMERO, "", (int)start};
        strncpy(token.lexema, lexer->input + start, lexer->pos - start);
        token.lexema[lexer->pos - start] = '\0';
        return token;
    }

    lexer->pos++;
    switch (c) {
        case '(': return (Token){PAREN_ESQ, "(", (int)lexer->pos - 1};
        case ')': return (Token){PAREN_DIR, ")", (int)lexer->pos - 1};
        case '+': return (Token){SOMA, "+", (int)lexer->pos - 1};
        case '-': return (Token){SUB, "-", (int)lexer->pos - 1};
        case '*': return (Token){MULT, "*", (int)lexer->pos - 1};
        case '/': return (Token){DIV, "/", (int)lexer->pos - 1};
        default: return (Token){ERRO, {c, '\0'}, (int)lexer->pos - 1};
    }
}

void analisar_lexico(const char *entrada) {
    Lexer lexer = criar_lexer(entrada);
    Token token;
    do {
        token = proximo_token(&lexer);
        printf("<");
        switch (token.tipo) {
            case NUMERO: printf("Numero, \"%s\", ", token.lexema); break;
            case PAREN_ESQ: printf("ParenEsq, \"(\", "); break;
            case PAREN_DIR: printf("ParenDir, \")\", "); break;
            case SOMA: printf("Soma, \"+\", "); break;
            case SUB: printf("Sub, \"-\", "); break;
            case MULT: printf("Mult, \"*\", "); break;
            case DIV: printf("Div, \"/\", "); break;
            case EOF_TOKEN: printf("EOF"); break;
            case ERRO: printf("Erro, \"%s\", ", token.lexema); break;
        }
        printf("%d>\n", token.posicao);
    } while (token.tipo != EOF_TOKEN);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 1;
    }

    char conteudo[1024];
    fread(conteudo, 1, sizeof(conteudo) - 1, arquivo);
    fclose(arquivo);
    conteudo[sizeof(conteudo) - 1] = '\0';

    analisar_lexico(conteudo);
    return 0;
}
