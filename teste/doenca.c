#include "universo.h"

/* =======================================================
   doenca.c — Aplicação e cura de doenças
   ======================================================= */

void doenca_aplicar(Universo *u, TipoDoenca tipo) {
    char msg[MAX_DESC];
    printf("\n  Aplicando \"%s\" ao universo...\n", NOME_DOENCA[tipo]);

    for (int i = 0; i < MAX_MUNDOS; i++) {
        Mundo *m = &u->mundos[i];
        if (!m->ativo) continue;

        /* Mundo Tecnologico: 50% de resistencia */
        if (m->tipo == MUNDO_TECNOLOGICO && rand() % 2 == 0) continue;

        for (Civilizacao *c = m->civs; c; c = c->prox) {
            /* Primitivas mais vulneraveis */
            int chance = (c->nivelTec <= 20) ? 70 : 30;
            if (rand() % 100 >= chance) continue;

            c->doente = 1;

            switch (tipo) {
                case DOENCA_VIRUS_VERMELHO:
                    c->populacao = (long long)(c->populacao * 0.7);
                    snprintf(msg, MAX_DESC,
                             "DOENCA [Virus Vermelho]: \"%s\" -30%% pop. => %I64d",
                             c->nome, c->populacao);
                    break;
                case DOENCA_PESTE_COSMICA:
                    c->populacao = (long long)(c->populacao * 0.5);
                    snprintf(msg, MAX_DESC,
                             "DOENCA [Peste Cosmica]: \"%s\" -50%% pop. => %I64d",
                             c->nome, c->populacao);
                    break;
                case DOENCA_COLAPSO_NEURAL:
                    c->nivelTec = (c->nivelTec >= 10) ? c->nivelTec - 10 : 0;
                    c->nivelEvolutivo = civ_nivel_evolutivo(c->nivelTec);
                    snprintf(msg, MAX_DESC,
                             "DOENCA [Colapso Neural]: \"%s\" tec reduzida => %d",
                             c->nome, c->nivelTec);
                    break;
                case DOENCA_INFECCAO_ALIENIGENA:
                    strncat(c->especie, "-Mut",
                            MAX_ESPECIE - (int)strlen(c->especie) - 1);
                    snprintf(msg, MAX_DESC,
                             "DOENCA [Infeccao Alienigena]: especie de \"%s\" mutada.",
                             c->nome);
                    break;
                default:
                    snprintf(msg, MAX_DESC, "DOENCA desconhecida em \"%s\".", c->nome);
                    break;
            }

            pilha_push(&u->historico, msg, u->anoAtual);
            printf("    ! %s\n", msg);

            /* extinção automática */
            if (c->populacao <= 0) {
                snprintf(msg, MAX_DESC,
                         "EXTINCAO: \"%s\" extinta por doenca.", c->nome);
                pilha_push(&u->historico, msg, u->anoAtual);
                printf("    *** EXTINCAO: \"%s\" foi extinta! ***\n", c->nome);
            }
        }
    }
    tempo_avancar(u, 10);
}

void doenca_curar_todos(Universo *u) {
    int curadas = 0;
    for (int i = 0; i < MAX_MUNDOS; i++) {
        if (!u->mundos[i].ativo) continue;
        for (Civilizacao *c = u->mundos[i].civs; c; c = c->prox) {
            if (c->doente) { c->doente = 0; curadas++; }
        }
    }
    printf("  Civilizacoes curadas: %d\n", curadas);
}