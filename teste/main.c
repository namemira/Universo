#include "universo.h"
#include <locale.h>
#include <windows.h>

/* =======================================================
   main.c — Ponto de entrada e menus do simulador
   ======================================================= */

static void cabecalho(const Universo *u) {
    printf("\n");
    printf("  ============================================================\n");
    printf("                   SIMULADOR DE UNIVERSO                      \n");
    printf("  ============================================================\n");
    printf("  Ano: %-8lld | Era: %-16s | Mundos: %d\n",
           u->anoAtual, NOME_ERA[u->era], u->numMundos);
    if (u->era < ERA_COSMICA) {
        long long faltam = DURACAO_ERA[u->era] - u->anosNaEra;
        printf("  Proxima era em: %I64d anos\n", faltam > 0 ? faltam : 0);
    } else {
        printf("  Era maxima: Era Cosmica atingida!\n");
    }
    printf("  ------------------------------------------------------------\n");
    printf("   1. Criar Mundo              8. Consultar Especies\n");
    printf("   2. Criar Civilizacao        9. Consultar Nivel\n");
    printf("   3. Buscar Civilizacao      10. Avancar no Tempo\n");
    printf("   4. Listar Universo         11. Historico\n");
    printf("   5. Listar Mundos           12. Aplicar Doenca\n");
    printf("   6. Destruir Civilizacao    13. Disparar Evento\n");
    printf("   7. Destruir Mundo          14. Registrar Evento Manual\n");
    printf("   0. Sair\n");
    printf("  ============================================================\n");
    printf("   Opcao: ");
}

/* =======================================================
   Busca civilização (submenu por ID / Nome / Espécie)
   ======================================================= */
static void menu_buscar(Universo *u) {
    printf("\n  --- BUSCAR CIVILIZACAO ---\n");
    printf("  1. Por ID\n");
    printf("  2. Por Nome\n");
    printf("  3. Por Especie\n");
    int op = io_ler_int("  Opcao: ");

    Civilizacao *c = NULL;
    char buf[MAX_NOME];

    switch (op) {
        case 1: {
            int id = io_ler_int("  ID: ");
            c = civ_buscar_id(u, id);
            break;
        }
        case 2:
            io_ler_string("  Nome (ou parte): ", buf, MAX_NOME);
            c = civ_buscar_nome(u, buf);
            break;
        case 3:
            io_ler_string("  Especie (ou parte): ", buf, MAX_ESPECIE);
            c = civ_buscar_especie(u, buf);
            break;
        default:
            printf("  Opcao invalida.\n");
            return;
    }

    if (!c) {
        printf("  Civilizacao nao encontrada.\n");
        return;
    }

    Mundo *m = mundo_buscar(u, c->mundoOrigem);
    printf("\n  ------ Civilizacao Encontrada ------\n");
    printf("  ID          : %d\n",   c->id);
    printf("  Nome        : %s\n",   c->nome);
    printf("  Especie     : %s\n",   c->especie);
    printf("  Populacao   : %I64d\n", c->populacao);
    printf("  Tec         : %d\n",   c->nivelTec);
    printf("  Nivel Evol. : %s\n",   civ_nome_nivel(c->nivelEvolutivo));
    printf("  Mundo       : %s\n",   m ? m->nome : "?");
    printf("  Status      : %s\n",   c->doente ? "DOENTE" : "Saudavel");
    printf("  ------------------------------------\n");
}

/* =======================================================
   Aplicar doença (submenu)
   ======================================================= */
static void menu_doenca(Universo *u) {
    printf("\n  --- APLICAR DOENCA ---\n");
    for (int i = 0; i < TOTAL_DOENCAS; i++)
        printf("  %d. %s\n", i, NOME_DOENCA[i]);
    int t = io_ler_int("  Escolha: ");
    if (t < 0 || t >= TOTAL_DOENCAS) { printf("  Invalido.\n"); return; }
    doenca_aplicar(u, (TipoDoenca)t);
}

/* =======================================================
   Disparar evento (submenu)
   ======================================================= */
static void menu_evento(Universo *u) {
    printf("\n  --- DISPARAR EVENTO ---\n");
    printf("  0. Guerra Mundial\n");
    printf("  1. Meteorito\n");
    printf("  2. Buraco Negro\n");
    printf("  3. Evolucao Tecnologica\n");
    printf("  4. Descoberta Espacial\n");
    printf("  5. Evento Medico (cura doencas)\n");
    printf("  6. Aleatorio\n");
    int op = io_ler_int("  Opcao: ");

    if (op == 6) {
        evento_aleatorio(u);
        tempo_avancar(u, 10);
        return;
    }
    if (op < 0 || op >= TOTAL_EVENTOS) { printf("  Invalido.\n"); return; }

    mundo_listar(u);
    if (!u->numMundos) { printf("  Nenhum mundo disponivel.\n"); return; }
    int mid = io_ler_int("  ID do mundo alvo: ");
    int idx = -1;
    for (int i = 0; i < MAX_MUNDOS; i++)
        if (u->mundos[i].ativo && u->mundos[i].id == mid) { idx = i; break; }

    evento_aplicar(u, (TipoEvento)op, idx);
    tempo_avancar(u, 10);
}

/* =======================================================
   Registrar evento manual
   ======================================================= */
static void menu_registrar_evento(Universo *u) {
    char desc[MAX_DESC];
    printf("\n  --- REGISTRAR EVENTO MANUAL ---\n");
    io_ler_string("  Descricao do evento: ", desc, MAX_DESC);
    pilha_push(&u->historico, desc, u->anoAtual);
    printf("  Evento registrado no historico.\n");
    tempo_avancar(u, 10);
}

/* =======================================================
   MAIN
   ======================================================= */
int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");

    Universo u;
    universo_init(&u);

    int op;
    do {
        cabecalho(&u);

        while (scanf("%d", &op) != 1) {
            io_limpar_buffer();
            printf("  Opcao invalida: ");
        }
        io_limpar_buffer();

        switch (op) {
            case 1:  mundo_criar(&u);                                         break;
            case 2:
                mundo_listar(&u);
                if (!u.numMundos) {
                    printf("  Nenhum mundo disponivel. Crie um mundo primeiro.\n");
                } else {
                    int id = io_ler_int("  ID do mundo destino: ");
                    civ_criar(&u, id);
                }
                break;
            case 3:  menu_buscar(&u);                                         break;
            case 4:  universo_listar(&u);                                     break;
            case 5:  mundo_listar(&u);                                        break;
            case 6:
                universo_listar(&u);
                if (u.numMundos) {
                    int id = io_ler_int("  ID da civilizacao a destruir: ");
                    civ_destruir(&u, id);
                }
                break;
            case 7:
                mundo_listar(&u);
                if (u.numMundos) {
                    int id = io_ler_int("  ID do mundo a destruir: ");
                    mundo_destruir(&u, id);
                }
                break;
            case 8:  civ_consultar_especies(&u);                              break;
            case 9:  era_exibir_nivel(&u);                                    break;
            case 10: {
                long long anos = io_ler_ll("  Quantos anos avancar? ");
                if (anos > 0) tempo_avancar(&u, anos);
                break;
            }
            case 11:
                printf("\n");
                pilha_exibir(&u.historico);
                printf("\n");
                break;
            case 12: menu_doenca(&u);                                         break;
            case 13: menu_evento(&u);                                         break;
            case 14: menu_registrar_evento(&u);                               break;
            case 0:
                printf("\n  Encerrando o Simulador. O universo dorme...\n\n");
                break;
            default:
                printf("  Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (op != 0);

    universo_destruir(&u);
    return 0;
}