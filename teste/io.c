#include "universo.h"

/* =======================================================
   io.c — Utilitários de entrada/saída
   ======================================================= */

void io_limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void io_ler_string(const char *prompt, char *dest, int tam) {
    printf("%s", prompt);
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

int io_ler_int(const char *prompt) {
    int v;
    printf("%s", prompt);
    while (scanf("%d", &v) != 1) {
        io_limpar_buffer();
        printf("  Valor invalido. Digite um numero inteiro: ");
    }
    io_limpar_buffer();
    return v;
}

long long io_ler_ll(const char *prompt) {
    long long v;
    printf("%s", prompt);
    while (scanf("%I64d", &v) != 1) {
        io_limpar_buffer();
        printf("  Valor invalido. Tente novamente: ");
    }
    io_limpar_buffer();
    return v;
}