#include "universo.h"

/* =======================================================
   mundo.c — Criação, destruição, busca e listagem de mundos
   ======================================================= */

static int _slot_livre(const Universo *u) {
    for (int i = 0; i < MAX_MUNDOS; i++)
        if (!u->mundos[i].ativo) return i;
    return -1;
}

Mundo *mundo_buscar(Universo *u, int id) {
    for (int i = 0; i < MAX_MUNDOS; i++)
        if (u->mundos[i].ativo && u->mundos[i].id == id)
            return &u->mundos[i];
    return NULL;
}

int mundo_criar(Universo *u) {
    if (u->numMundos >= MAX_MUNDOS) {
        printf("  Limite de %d mundos atingido!\n", MAX_MUNDOS);
        return 0;
    }
    int slot = _slot_livre(u);
    if (slot < 0) return 0;

    Mundo *m = &u->mundos[slot];
    memset(m, 0, sizeof(Mundo));

    printf("\n  --- CRIAR MUNDO ---\n");
    io_ler_string("  Nome: ", m->nome, MAX_NOME);

    printf("  Tipo:\n");
    for (int i = 0; i < TOTAL_TIPOS_MUNDO; i++)
        printf("    %d. %s\n", i, NOME_TIPO_MUNDO[i]);
    int t = io_ler_int("  Escolha: ");
    if (t < 0 || t >= TOTAL_TIPOS_MUNDO) t = MUNDO_TERRESTRE;
    m->tipo = (TipoMundo)t;

    do {
        m->nivelRecursos = io_ler_int("  Nivel de recursos (1-10): ");
        if (m->nivelRecursos < 1 || m->nivelRecursos > 10)
            printf("  Valor invalido! Digite um numero entre 1 e 10.\n");
    } while (m->nivelRecursos < 1 || m->nivelRecursos > 10);

    do {
        m->nivelPerigo = io_ler_int("  Nivel de perigo (1-10): ");
        if (m->nivelPerigo < 1 || m->nivelPerigo > 10)
            printf("  Valor invalido! Digite um numero entre 1 e 10.\n");
    } while (m->nivelPerigo < 1 || m->nivelPerigo > 10);

    /* limite populacional por tipo */
    switch (m->tipo) {
        case MUNDO_CONGELADO:   m->limitePopulacional = POP_MAX / 5; break;
        case MUNDO_VULCANICO:   m->limitePopulacional = POP_MAX / 3; break;
        case MUNDO_TECNOLOGICO: m->limitePopulacional = POP_MAX;     break;
        default:                m->limitePopulacional = POP_MAX / 2; break;
    }

    m->id    = u->proxIdMundo++;
    m->ativo = 1;
    m->civs  = NULL;
    m->numCivs = 0;
    u->numMundos++;

    char msg[MAX_DESC];
    snprintf(msg, MAX_DESC, "MUNDO CRIADO: [%d] \"%s\" (%s) | Rec:%d | Per:%d",
             m->id, m->nome, NOME_TIPO_MUNDO[m->tipo],
             m->nivelRecursos, m->nivelPerigo);
    pilha_push(&u->historico, msg, u->anoAtual);

    printf("  Mundo \"%s\" criado com ID %d!\n", m->nome, m->id);
    tempo_avancar(u, 10);
    return 1;
}

int mundo_destruir(Universo *u, int id) {
    Mundo *m = mundo_buscar(u, id);
    if (!m) { printf("  Mundo ID %d nao encontrado.\n", id); return 0; }

    /* libera lista de civilizações */
    Civilizacao *c = m->civs;
    while (c) { Civilizacao *p = c->prox; free(c); c = p; }

    char nome_tmp[MAX_NOME];
    strncpy(nome_tmp, m->nome, MAX_NOME);

    m->civs    = NULL;
    m->ativo   = 0;
    m->numCivs = 0;
    u->numMundos--;

    char msg[MAX_DESC];
    snprintf(msg, MAX_DESC, "MUNDO DESTRUIDO: [%d] \"%s\"", id, nome_tmp);
    pilha_push(&u->historico, msg, u->anoAtual);

    printf("  Mundo [%d] \"%s\" destruido.\n", id, nome_tmp);
    tempo_avancar(u, 10);
    return 1;
}

void mundo_listar(const Universo *u) {
    if (!u->numMundos) { printf("  Nenhum mundo criado.\n"); return; }
    printf("\n  %-4s %-20s %-12s %-5s %-5s %-5s\n",
           "ID", "Nome", "Tipo", "Rec", "Per", "Civs");
    printf("  %-4s %-20s %-12s %-5s %-5s %-5s\n",
           "----","--------------------","------------","-----","-----","-----");
    for (int i = 0; i < MAX_MUNDOS; i++) {
        const Mundo *m = &u->mundos[i];
        if (!m->ativo) continue;
        printf("  %-4d %-20s %-12s %-5d %-5d %d/%d\n",
               m->id, m->nome, NOME_TIPO_MUNDO[m->tipo],
               m->nivelRecursos, m->nivelPerigo,
               m->numCivs, MAX_CIVS_MUNDO);
    }
    printf("\n");
}
