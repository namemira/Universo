#include "universo.h"

/* =======================================================
   tabelas.c — Definição das tabelas de strings globais
   (declaradas extern no universo.h)
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
