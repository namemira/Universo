#include "universo.h"

/* =======================================================
   tempo.c — Avanço de tempo, verificação e exibição de eras
   ======================================================= */

void era_verificar(Universo *u) {
    if (u->era >= ERA_COSMICA) return;
    long long lim = DURACAO_ERA[u->era];
    if (lim > 0 && u->anosNaEra >= lim) {
        u->era++;
        u->anosNaEra = 0;

        char msg[MAX_DESC];
        snprintf(msg, MAX_DESC,
                 "AVANCO DE ERA: Universo entrou na %s!", NOME_ERA[u->era]);
        pilha_push(&u->historico, msg, u->anoAtual);

        printf("\n");
        printf("  *** AVANCO DE ERA! ***\n");
        printf("  O universo agora esta na %s.\n", NOME_ERA[u->era]);
        printf("  Caracteristica: %s\n\n", DESC_ERA[u->era]);
    }
}

void tempo_avancar(Universo *u, long long anos) {
    u->anoAtual  += anos;
    u->anosNaEra += anos;

    printf("  Se passaram %lld anos. (Ano atual: %lld)\n", anos, u->anoAtual);

    era_verificar(u);

    /* 20% de chance de evento aleatório */
    if (u->numMundos > 0 && rand() % 5 == 0)
        evento_aleatorio(u);
}

void era_exibir_nivel(const Universo *u) {
    printf("\n");
    printf("  ============================================================\n");
    printf("                    NIVEL DE JOGO ATUAL                       \n");
    printf("  ============================================================\n");
    printf("  Nivel (Era): %s\n", NOME_ERA[u->era]);
    printf("  Descricao  : %s\n", DESC_ERA[u->era]);
    printf("  Acoes perm.: %s\n", ACOES_ERA[u->era]);
    printf("  Ano atual  : %lld\n", u->anoAtual);
    printf("  Anos na era: %lld", u->anosNaEra);
    if (u->era < ERA_COSMICA) {
        long long faltam = DURACAO_ERA[u->era] - u->anosNaEra;
        printf(" / %lld  (faltam %lld para proxima era)\n",
               DURACAO_ERA[u->era], faltam > 0 ? faltam : 0);
    } else {
        printf(" (Era maxima atingida)\n");
    }
    printf("  ============================================================\n\n");
}
