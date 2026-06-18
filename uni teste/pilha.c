#include "universo.h"

/* =======================================================
   pilha.c — Pilha de histórico (lista encadeada manual)
   ======================================================= */

void pilha_push(Pilha *p, const char *desc, long long ano) {
    NoPilha *no = (NoPilha *)malloc(sizeof(NoPilha));
    if (!no) { printf("[ERRO] Sem memoria para pilha!\n"); return; }
    strncpy(no->desc, desc, MAX_DESC - 1);
    no->desc[MAX_DESC - 1] = '\0';
    no->ano  = ano;
    no->prox = p->topo;
    p->topo  = no;
    p->tamanho++;
}

int pilha_pop(Pilha *p, char *dest, long long *ano) {
    if (!p->topo) return 0;
    NoPilha *tmp = p->topo;
    if (dest) strncpy(dest, tmp->desc, MAX_DESC - 1);
    if (ano)  *ano = tmp->ano;
    p->topo = tmp->prox;
    free(tmp);
    p->tamanho--;
    return 1;
}

void pilha_exibir(const Pilha *p) {
    if (!p->topo) { printf("  Historico vazio.\n"); return; }
    printf("\n  %-5s %-10s %s\n", "Nro", "Ano", "Evento");
    printf("  %-5s %-10s %s\n",
           "-----", "----------",
           "------------------------------------------------------------");
    NoPilha *n = p->topo;
    int i = 1;
    while (n) {
        printf("  %-5d %-10lld %s\n", i++, n->ano, n->desc);
        n = n->prox;
    }
    printf("  Total: %d evento(s)\n", p->tamanho);
}

void pilha_liberar(Pilha *p) {
    while (p->topo) pilha_pop(p, NULL, NULL);
}
