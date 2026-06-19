#include "universo.h"

/* =======================================================
   universo.c — Inicialização, destruição e listagem geral
   ======================================================= */

void universo_init(Universo *u) {
    srand((unsigned int)time(NULL));
    memset(u, 0, sizeof(Universo));
    u->era          = ERA_PRIMITIVA;
    u->anoAtual     = 0;
    u->anosNaEra    = 0;
    u->numMundos    = 0;
    u->proxIdCiv    = 1;
    u->proxIdMundo  = 1;
    u->historico.topo    = NULL;
    u->historico.tamanho = 0;

    pilha_push(&u->historico,
               "UNIVERSO CRIADO: Ano 0 | Era Primitiva | 0 mundos | 0 civilizacoes.", 0);

    printf("\n");
    printf("  *** UNIVERSO CRIADO ***\n");
    printf("  Ano: 0 | Era: Era Primitiva\n");
    printf("  Mundos: 0 | Civilizacoes: 0\n");
}

void universo_destruir(Universo *u) {
    for (int i = 0; i < MAX_MUNDOS; i++) {
        if (!u->mundos[i].ativo) continue;
        Civilizacao *c = u->mundos[i].civs;
        while (c) { Civilizacao *p = c->prox; free(c); c = p; }
        u->mundos[i].civs = NULL;
    }
    pilha_liberar(&u->historico);
}

void universo_listar(const Universo *u) {
    printf("\n");
    printf("  ============================================================\n");
    printf("                    ESTADO DO UNIVERSO                        \n");
    printf("  ============================================================\n");
    printf("  Ano: %I64d | Era: %s | Mundos ativos: %d\n",
           u->anoAtual, NOME_ERA[u->era], u->numMundos);
    printf("  ------------------------------------------------------------\n");

    int totalCivs = 0;
    for (int i = 0; i < MAX_MUNDOS; i++) {
        const Mundo *m = &u->mundos[i];
        if (!m->ativo) continue;
        printf("\n  [Mundo %d] %s  Tipo: %s  Recursos: %d  Perigo: %d\n",
               m->id, m->nome, NOME_TIPO_MUNDO[m->tipo],
               m->nivelRecursos, m->nivelPerigo);

        if (!m->civs) {
            printf("    (nenhuma civilizacao)\n");
        } else {
            Civilizacao *c = m->civs;
            while (c) {
                printf("    ID:%-3d  Nome: %-18s  Esp: %-14s  "
                       "Pop: %10lld  Tec: %3d  Nivel: %-10s  %s\n",
                       c->id, c->nome, c->especie, c->populacao,
                       c->nivelTec, civ_nome_nivel(c->nivelEvolutivo),
                       c->doente ? "[DOENTE]" : "[Saudavel]");
                c = c->prox;
                totalCivs++;
            }
        }
    }
    printf("\n  ============================================================\n");
    printf("  Total de civilizacoes: %d\n\n", totalCivs);
}