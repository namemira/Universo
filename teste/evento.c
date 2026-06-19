#include "universo.h"

/* =======================================================
   evento.c — Aplicação de eventos (manuais e aleatórios)
   ======================================================= */

void evento_aplicar(Universo *u, TipoEvento tipo, int mundoIdx) {
    Mundo *m = (mundoIdx >= 0 && mundoIdx < MAX_MUNDOS && u->mundos[mundoIdx].ativo)
               ? &u->mundos[mundoIdx] : NULL;
    char msg[MAX_DESC];

    switch (tipo) {

        case EVENTO_GUERRA:
            if (!m) break;
            snprintf(msg, MAX_DESC,
                     "GUERRA MUNDIAL em \"%s\": populacoes reduzidas em 20%%.", m->nome);
            for (Civilizacao *c = m->civs; c; c = c->prox)
                c->populacao = (long long)(c->populacao * 0.8);
            pilha_push(&u->historico, msg, u->anoAtual);
            printf("  *** %s ***\n", msg);
            break;

        case EVENTO_METEORITO:
            if (!m) break;
            snprintf(msg, MAX_DESC,
                     "METEORITO em \"%s\": nivel de recursos reduzido.", m->nome);
            m->nivelRecursos = (m->nivelRecursos > 2) ? m->nivelRecursos - 2 : 1;
            pilha_push(&u->historico, msg, u->anoAtual);
            printf("  *** %s ***\n", msg);
            break;

        case EVENTO_BURACO_NEGRO:
            if (!m) break;
            if (u->numMundos > 1) {
                snprintf(msg, MAX_DESC,
                         "BURACO NEGRO: mundo \"%s\" foi destruido totalmente!", m->nome);
                pilha_push(&u->historico, msg, u->anoAtual);
                printf("  *** %s ***\n", msg);
                mundo_destruir(u, m->id);
            } else {
                snprintf(msg, MAX_DESC,
                         "BURACO NEGRO detectado! Desviou — universo salvo.");
                pilha_push(&u->historico, msg, u->anoAtual);
                printf("  *** %s ***\n", msg);
            }
            break;

        case EVENTO_EVOLUCAO_TEC:
            if (!m) break;
            snprintf(msg, MAX_DESC,
                     "EVOLUCAO TECNOLOGICA em \"%s\": +5 de tecnologia para todas as civs.",
                     m->nome);
            for (Civilizacao *c = m->civs; c; c = c->prox) {
                c->nivelTec += 5;
                if (c->nivelTec > 100) c->nivelTec = 100;
                c->nivelEvolutivo = civ_nivel_evolutivo(c->nivelTec);
            }
            pilha_push(&u->historico, msg, u->anoAtual);
            printf("  *** %s ***\n", msg);
            break;

        case EVENTO_DESCOBERTA:
            if (u->era < ERA_COSMICA) {
                u->anosNaEra += DURACAO_ERA[u->era] / 2;
                snprintf(msg, MAX_DESC,
                         "DESCOBERTA ESPACIAL: evolucao de era acelerada!");
                era_verificar(u);
            } else {
                snprintf(msg, MAX_DESC, "DESCOBERTA COSMICA: novo conhecimento adquirido!");
            }
            pilha_push(&u->historico, msg, u->anoAtual);
            printf("  *** %s ***\n", msg);
            break;

        case EVENTO_MEDICO:
            doenca_curar_todos(u);
            snprintf(msg, MAX_DESC, "EVENTO MEDICO: todas as civilizacoes foram curadas!");
            pilha_push(&u->historico, msg, u->anoAtual);
            printf("  *** %s ***\n", msg);
            break;

        default: break;
    }
}

void evento_aleatorio(Universo *u) {
    /* escolhe índice de mundo ativo aleatoriamente */
    int idx = -1, tentativas = 0;
    int r = rand() % MAX_MUNDOS;
    while (tentativas < MAX_MUNDOS) {
        if (u->mundos[r].ativo) { idx = r; break; }
        r = (r + 1) % MAX_MUNDOS;
        tentativas++;
    }
    if (idx < 0) return;

    TipoEvento tipo = (TipoEvento)(rand() % TOTAL_EVENTOS);
    printf("\n  [EVENTO ALEATORIO]\n");
    evento_aplicar(u, tipo, idx);
}
