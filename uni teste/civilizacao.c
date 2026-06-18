#include "universo.h"

/* =======================================================
   civilizacao.c — Criação, destruição, busca e consulta
                   de civilizações; níveis evolutivos
   ======================================================= */

/* ---------- Níveis evolutivos ---------- */

int civ_nivel_evolutivo(int tec) {
    if (tec <=  20) return 0; /* Primitiva  */
    if (tec <=  40) return 1; /* Medieval   */
    if (tec <=  60) return 2; /* Industrial */
    if (tec <=  80) return 3; /* Futurista  */
    return 4;                 /* Espacial   */
}

const char *civ_nome_nivel(int n) {
    static const char *t[] = {
        "Primitiva", "Medieval", "Industrial", "Futurista", "Espacial"
    };
    return (n >= 0 && n <= 4) ? t[n] : "?";
}

/* ---------- CRUD ---------- */

int civ_criar(Universo *u, int mundoId) {
    Mundo *m = mundo_buscar(u, mundoId);
    if (!m) { printf("  Mundo ID %d nao encontrado.\n", mundoId); return 0; }
    if (m->numCivs >= MAX_CIVS_MUNDO) {
        printf("  Mundo \"%s\" ja tem %d civilizacoes (limite).\n",
               m->nome, MAX_CIVS_MUNDO);
        return 0;
    }

    Civilizacao *nova = (Civilizacao *)malloc(sizeof(Civilizacao));
    if (!nova) { printf("  [ERRO] Sem memoria!\n"); return 0; }

    printf("\n  --- CRIAR CIVILIZAÇÃO em \"%s\" ---\n", m->nome);
    io_ler_string("  Nome da civilizacao: ", nova->nome, MAX_NOME);
    io_ler_string("  Especie: ", nova->especie, MAX_ESPECIE);
    nova->populacao = io_ler_ll("  Populacao inicial: ");

    if (nova->populacao > m->limitePopulacional)
        nova->populacao = m->limitePopulacional;
    if (nova->populacao < 1) nova->populacao = 1;

    do {
        nova->nivelTec = io_ler_int("  Nivel tecnologico (0-100): ");
        if (nova->nivelTec < 0 || nova->nivelTec > 100)
            printf("  Valor invalido! Digite um numero entre 0 e 100.\n");
    } while (nova->nivelTec < 0 || nova->nivelTec > 100);

    nova->id             = u->proxIdCiv++;
    nova->nivelEvolutivo = civ_nivel_evolutivo(nova->nivelTec);
    nova->mundoOrigem    = mundoId;
    nova->doente         = 0;

    /* inserção na cabeça da lista encadeada */
    nova->prox  = m->civs;
    m->civs     = nova;
    m->numCivs++;

    char msg[MAX_DESC];
    snprintf(msg, MAX_DESC,
             "CIV CRIADA: [%d] \"%s\" (esp:%s) | Mundo:\"%s\" | Pop:%lld | Tec:%d | Nivel:%s",
             nova->id, nova->nome, nova->especie,
             m->nome, nova->populacao,
             nova->nivelTec, civ_nome_nivel(nova->nivelEvolutivo));
    pilha_push(&u->historico, msg, u->anoAtual);

    printf("  Civilizacao \"%s\" criada com ID %d!\n", nova->nome, nova->id);
    tempo_avancar(u, 10);
    return 1;
}

int civ_destruir(Universo *u, int id) {
    for (int i = 0; i < MAX_MUNDOS; i++) {
        Mundo *m = &u->mundos[i];
        if (!m->ativo) continue;

        Civilizacao *c   = m->civs;
        Civilizacao *ant = NULL;
        while (c) {
            if (c->id == id) {
                if (ant) ant->prox = c->prox;
                else     m->civs   = c->prox;
                m->numCivs--;

                char msg[MAX_DESC];
                snprintf(msg, MAX_DESC,
                         "CIV DESTRUIDA: [%d] \"%s\" | Mundo:\"%s\"",
                         c->id, c->nome, m->nome);
                pilha_push(&u->historico, msg, u->anoAtual);

                printf("  Civilizacao [%d] \"%s\" destruida.\n", c->id, c->nome);
                free(c);
                tempo_avancar(u, 10);
                return 1;
            }
            ant = c;
            c   = c->prox;
        }
    }
    printf("  Civilizacao ID %d nao encontrada.\n", id);
    return 0;
}

/* ---------- Buscas ---------- */

Civilizacao *civ_buscar_id(const Universo *u, int id) {
    for (int i = 0; i < MAX_MUNDOS; i++) {
        if (!u->mundos[i].ativo) continue;
        for (Civilizacao *c = u->mundos[i].civs; c; c = c->prox)
            if (c->id == id) return c;
    }
    return NULL;
}

Civilizacao *civ_buscar_nome(const Universo *u, const char *nome) {
    for (int i = 0; i < MAX_MUNDOS; i++) {
        if (!u->mundos[i].ativo) continue;
        for (Civilizacao *c = u->mundos[i].civs; c; c = c->prox)
            if (strstr(c->nome, nome)) return c;
    }
    return NULL;
}

Civilizacao *civ_buscar_especie(const Universo *u, const char *esp) {
    for (int i = 0; i < MAX_MUNDOS; i++) {
        if (!u->mundos[i].ativo) continue;
        for (Civilizacao *c = u->mundos[i].civs; c; c = c->prox)
            if (strstr(c->especie, esp)) return c;
    }
    return NULL;
}

/* ---------- Consultas ---------- */

void civ_consultar_especies(const Universo *u) {
    printf("\n  --- ESPECIES NO UNIVERSO ---\n");
    int total = 0;
    for (int i = 0; i < MAX_MUNDOS; i++) {
        const Mundo *m = &u->mundos[i];
        if (!m->ativo) continue;
        for (Civilizacao *c = m->civs; c; c = c->prox) {
            printf("  ID:%-3d  Esp: %-16s  Civ: %-18s  Tec:%3d  Status:%s\n",
                   c->id, c->especie, c->nome, c->nivelTec,
                   c->doente ? "DOENTE" : "Saudavel");
            total++;
        }
    }
    if (!total) printf("  Nenhuma especie cadastrada.\n");
    printf("  Total: %d especie(s)\n\n", total);
}
