#include "universo.h"

/* =======================================================
   Tabelas de strings (definidas aqui, declaradas extern no .h)
   ======================================================= */
const char *NOME_ERA[TOTAL_ERAS] = {
    "Era Primitiva", "Era Medieval", "Era Industrial",
    "Era Futurista",  "Era Espacial",  "Era Cosmica"
};

const char *DESC_ERA[TOTAL_ERAS] = {
    "civilizacoes basicas, baixa tecnologia",
    "surgimento de reinos e guerras",
    "maquinas, crescimento populacional",
    "inteligencia artificial e tecnologia avancada",
    "colonizacao espacial e viagens galacticas",
    "controle avancado do universo"
};

const char *ACOES_ERA[TOTAL_ERAS] = {
    "Criar/destruir civilizacoes primitivas; guerras locais.",
    "Criar reinos; guerras medievais; primeiras exploracoes.",
    "Expansao industrial; doencas; guerras mundiais.",
    "Evolucao tecn. rapida; IA; colonizacao de mundos.",
    "Viagens interestelares; descobertas espaciais; expansao.",
    "Controle total do universo; eventos cosmicos."
};

const char *NOME_TIPO_MUNDO[TOTAL_TIPOS_MUNDO] = {
    "Terrestre", "Vulcanico", "Congelado", "Tecnologico", "Alienigena"
};

const char *NOME_DOENCA[TOTAL_DOENCAS] = {
    "Virus Vermelho", "Peste Cosmica", "Colapso Neural", "Infeccao Alienigena"
};

/* =======================================================
   UTILITÁRIOS DE I/O
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
    while (scanf("%lld", &v) != 1) {
        io_limpar_buffer();
        printf("  Valor invalido. Tente novamente: ");
    }
    io_limpar_buffer();
    return v;
}

/* =======================================================
   PILHA — implementação manual com alocação dinâmica
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

/* =======================================================
   NÍVEIS EVOLUTIVOS
   ======================================================= */
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

/* =======================================================
   UNIVERSO
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
    printf("  Ano: %lld | Era: %s | Mundos ativos: %d\n",
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

/* =======================================================
   MUNDOS
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

/*Recursos*/

   do {
    m->nivelRecursos = io_ler_int("  Nivel de recursos (1-10): ");

    if (m->nivelRecursos < 1 || m->nivelRecursos > 10)
        printf("  Valor invalido! Digite um numero entre 1 e 10.\n");

} while (m->nivelRecursos < 1 || m->nivelRecursos > 10);

    /*NÍvel de perigo*/
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

/* =======================================================
   CIVILIZAÇÕES
   ======================================================= */
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
    io_ler_string("  Nome da civilização: ", nova->nome, MAX_NOME);
    io_ler_string("  Espécie: ", nova->especie, MAX_ESPECIE);
    nova->populacao = io_ler_ll("  População inicial: ");
    if (nova->populacao > m->limitePopulacional)
        nova->populacao = m->limitePopulacional;
    if (nova->populacao < 1) nova->populacao = 1;

    nova->nivelTec = io_ler_int("  Nível tecnologico (0-100): ");
    if (nova->nivelTec < 0)   nova->nivelTec = 0;
    if (nova->nivelTec > 100) nova->nivelTec = 100;

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

/* Consultar espécies — exibe todas as espécies únicas cadastradas */
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

/* =======================================================
   TEMPO E ERAS
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

    /* Mensagem obrigatória conforme o PDF */
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

/* =======================================================
   EVENTOS
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

/* =======================================================
   DOENÇAS
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
                             "DOENCA [Virus Vermelho]: \"%s\" -30%% pop. => %lld",
                             c->nome, c->populacao);
                    break;
                case DOENCA_PESTE_COSMICA:
                    c->populacao = (long long)(c->populacao * 0.5);
                    snprintf(msg, MAX_DESC,
                             "DOENCA [Peste Cosmica]: \"%s\" -50%% pop. => %lld",
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
