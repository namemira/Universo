#ifndef UNIVERSO_H
#define UNIVERSO_H

/* =======================================================
   Simulador de Universo — Estrutura de Dados I
   Linguagem C | Lista Encadeada + Pilha
   ======================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---------- Limites ---------- */
#define MAX_MUNDOS         10
#define MAX_CIVS_MUNDO      5
#define MAX_NOME           80
#define MAX_ESPECIE        50
#define MAX_DESC          300
#define POP_MAX    10000000000LL   /* 10 bilhões */

/* ---------- Eras ---------- */
typedef enum {
    ERA_PRIMITIVA  = 0,
    ERA_MEDIEVAL   = 1,
    ERA_INDUSTRIAL = 2,
    ERA_FUTURISTA  = 3,
    ERA_ESPACIAL   = 4,
    ERA_COSMICA    = 5,
    TOTAL_ERAS     = 6
} Era;

/* Anos necessários para avançar (índice = era atual; 0 = não avança) */
static const long long DURACAO_ERA[TOTAL_ERAS] = {500, 1000, 1500, 2500, 5000, 0};

extern const char *NOME_ERA[TOTAL_ERAS];
extern const char *DESC_ERA[TOTAL_ERAS];
extern const char *ACOES_ERA[TOTAL_ERAS];

/* ---------- Tipos de Mundo ---------- */
typedef enum {
    MUNDO_TERRESTRE   = 0,
    MUNDO_VULCANICO   = 1,
    MUNDO_CONGELADO   = 2,
    MUNDO_TECNOLOGICO = 3,
    MUNDO_ALIENIGENA  = 4,
    TOTAL_TIPOS_MUNDO = 5
} TipoMundo;

extern const char *NOME_TIPO_MUNDO[TOTAL_TIPOS_MUNDO];

/* ---------- Doenças ---------- */
typedef enum {
    DOENCA_VIRUS_VERMELHO      = 0,
    DOENCA_PESTE_COSMICA       = 1,
    DOENCA_COLAPSO_NEURAL      = 2,
    DOENCA_INFECCAO_ALIENIGENA = 3,
    TOTAL_DOENCAS              = 4
} TipoDoenca;

extern const char *NOME_DOENCA[TOTAL_DOENCAS];

/* ---------- Eventos ---------- */
typedef enum {
    EVENTO_GUERRA          = 0,
    EVENTO_METEORITO       = 1,
    EVENTO_BURACO_NEGRO    = 2,
    EVENTO_EVOLUCAO_TEC    = 3,
    EVENTO_DESCOBERTA      = 4,
    EVENTO_MEDICO          = 5,   /* cura doenças */
    TOTAL_EVENTOS          = 6
} TipoEvento;

/* =======================================================
   ESTRUTURAS DE DADOS
   ======================================================= */

/* Nó da lista encadeada de civilizações */
typedef struct Civilizacao {
    int       id;
    char      nome[MAX_NOME];
    char      especie[MAX_ESPECIE];
    long long populacao;
    int       nivelTec;        /* 0–100 */
    int       nivelEvolutivo;  /* 0–4, calculado a partir de nivelTec */
    int       mundoOrigem;     /* ID do mundo */
    int       doente;          /* 0 = saudável, 1 = doente */
    struct Civilizacao *prox;
} Civilizacao;

/* Mundo — contém lista encadeada de civilizações */
typedef struct {
    int        id;
    char       nome[MAX_NOME];
    TipoMundo  tipo;
    int        nivelRecursos;       /* 1–10 */
    int        nivelPerigo;         /* 1–10 */
    long long  limitePopulacional;
    int        numCivs;
    int        ativo;
    Civilizacao *civs;              /* cabeça da lista encadeada */
} Mundo;

/* Nó da pilha de histórico */
typedef struct NoPilha {
    char      desc[MAX_DESC];
    long long ano;
    struct NoPilha *prox;
} NoPilha;

/* Pilha de histórico (implementação manual) */
typedef struct {
    NoPilha *topo;
    int      tamanho;
} Pilha;

/* Estado global do universo */
typedef struct {
    Mundo     mundos[MAX_MUNDOS];
    int       numMundos;
    long long anoAtual;
    long long anosNaEra;
    Era       era;
    Pilha     historico;
    int       proxIdCiv;
    int       proxIdMundo;
} Universo;

/* =======================================================
   PROTÓTIPOS
   ======================================================= */

/* --- io.c --- */
void      io_limpar_buffer(void);
void      io_ler_string(const char *prompt, char *dest, int tam);
int       io_ler_int(const char *prompt);
long long io_ler_ll(const char *prompt);

/* --- pilha.c --- */
void      pilha_push(Pilha *p, const char *desc, long long ano);
int       pilha_pop(Pilha *p, char *dest, long long *ano);
void      pilha_exibir(const Pilha *p);
void      pilha_liberar(Pilha *p);

/* --- universo.c --- */
void      universo_init(Universo *u);
void      universo_destruir(Universo *u);
void      universo_listar(const Universo *u);

/* --- mundo.c --- */
int       mundo_criar(Universo *u);
int       mundo_destruir(Universo *u, int id);
Mundo    *mundo_buscar(Universo *u, int id);
void      mundo_listar(const Universo *u);

/* --- civilizacao.c --- */
int          civ_criar(Universo *u, int mundoId);
int          civ_destruir(Universo *u, int id);
Civilizacao *civ_buscar_id(const Universo *u, int id);
Civilizacao *civ_buscar_nome(const Universo *u, const char *nome);
Civilizacao *civ_buscar_especie(const Universo *u, const char *esp);
void         civ_consultar_especies(const Universo *u);
int          civ_nivel_evolutivo(int tec);
const char  *civ_nome_nivel(int nivel);

/* --- tempo.c --- */
void      tempo_avancar(Universo *u, long long anos);
void      era_verificar(Universo *u);
void      era_exibir_nivel(const Universo *u);

/* --- evento.c --- */
void      evento_aleatorio(Universo *u);
void      evento_aplicar(Universo *u, TipoEvento tipo, int mundoIdx);

/* --- doenca.c --- */
void      doenca_aplicar(Universo *u, TipoDoenca tipo);
void      doenca_curar_todos(Universo *u);

#endif /* UNIVERSO_H */
