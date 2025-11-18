#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da constante para o tamanho da Tabela Hash
#define TAMANHO_HASH 10

// Estrutura para um nó da lista encadeada (para resolver colisões)
typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash* proximo;
} NoHash;

// A Tabela Hash é um array de ponteiros para a estrutura NoHash
NoHash* tabelaHash[TAMANHO_HASH];

// Estrutura para armazenar a contagem de menções de cada suspeito
typedef struct ContagemSuspeito {
    char nome[50];
    int contagem;
} ContagemSuspeito;

// Array para armazenar a contagem e um contador para o número de suspeitos únicos
ContagemSuspeito contagens[10]; // Supondo um máximo de 10 suspeitos únicos
int numSuspeitosUnicos = 0;

/**
 * @brief Função de espalhamento (Hash Function)
 * Baseada na soma dos valores ASCII dos caracteres da chave (pista)
 * e aplica o módulo do tamanho da tabela.
 * @param chave A string da pista.
 * @return O índice no array da tabela hash.
 */
int calcularHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += (int)chave[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * @brief Inicializa a Tabela Hash, definindo todos os ponteiros como NULL.
 */
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

/**
 * @brief Atualiza ou insere um novo suspeito no array de contagens.
 * @param suspeito O nome do suspeito.
 */
void atualizarContagem(const char* suspeito) {
    // 1. Verificar se o suspeito já existe no array de contagens
    for (int i = 0; i < numSuspeitosUnicos; i++) {
        if (strcmp(contagens[i].nome, suspeito) == 0) {
            contagens[i].contagem++;
            return;
        }
    }
    
    // 2. Se não existe, adicionar como novo suspeito único
    if (numSuspeitosUnicos < 10) {
        strcpy(contagens[numSuspeitosUnicos].nome, suspeito);
        contagens[numSuspeitosUnicos].contagem = 1;
        numSuspeitosUnicos++;
    } else {
        printf("⚠️ Limite de suspeitos únicos atingido para contagem.\n");
    }
}

/**
 * @brief Insere uma nova associação Pista -> Suspeito na Tabela Hash.
 * Lida com colisões usando encadeamento (Separate Chaining).
 * @param pista A chave (clue) a ser armazenada.
 * @param suspeito O valor (suspect) associado à pista.
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = calcularHash(pista);

    // Cria um novo nó
    NoHash* novoNo = (NoHash*)malloc(sizeof(NoHash));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória!\n");
        return;
    }

    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = NULL;

    // Inserção no início da lista encadeada (encadeamento)
    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;

    // Atualiza a contagem de menções
    atualizarContagem(suspeito);

    printf("✅ Inserido: Pista '%s' -> Suspeito '%s' (Índice %d)\n", pista, suspeito, indice);
}

/**
 * @brief Consulta e exibe todas as pistas associadas a um suspeito específico.
 * Percorre toda a tabela hash.
 * @param nomeSuspeito O nome do suspeito a consultar.
 */
void consultarPistasPorSuspeito(const char* nomeSuspeito) {
    printf("\n--- 🔍 Pistas de **%s** ---\n", nomeSuspeito);
    int encontradas = 0;

    // Percorre todos os 'baldes' (índices) da tabela
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = tabelaHash[i];
        // Percorre a lista encadeada em cada balde
        while (atual != NULL) {
            if (strcmp(atual->suspeito, nomeSuspeito) == 0) {
                printf("* Pista: %s\n", atual->pista);
                encontradas++;
            }
            atual = atual->proximo;
        }
    }

    if (encontradas == 0) {
        printf("Nenhuma pista encontrada associada a %s.\n", nomeSuspeito);
    }
    printf("-------------------------------\n");
}

/**
 * @brief Identifica e exibe o suspeito mais citado nas pistas.
 */
void mostrarSuspeitoMaisCitado() {
    if (numSuspeitosUnicos == 0) {
        printf("\n❌ Nenhuma pista armazenada para análise.\n");
        return;
    }

    int contagemMax = -1;
    char suspeitoMaisCitado[50] = "";

    // Percorre o array de contagens para encontrar o máximo
    for (int i = 0; i < numSuspeitosUnicos; i++) {
        if (contagens[i].contagem > contagemMax) {
            contagemMax = contagens[i].contagem;
            strcpy(suspeitoMaisCitado, contagens[i].nome);
        }
    }

    printf("\n===================================\n");
    printf("   🌟 **ANÁLISE FINAL DE EVIDÊNCIAS** 🌟\n");
    printf("===================================\n");
    
    // Exibe a lista completa de contagens
    printf("Contagem de Pistas por Suspeito:\n");
    for (int i = 0; i < numSuspeitosUnicos; i++) {
        printf("* %s: %d menções\n", contagens[i].nome, contagens[i].contagem);
    }

    printf("\n=> O **SUSPEITO MAIS CITADO** nas pistas é: **%s** com %d menções!\n", 
           suspeitoMaisCitado, contagemMax);
    printf("===================================\n");
}

/**
 * @brief Libera a memória alocada pela Tabela Hash.
 */
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = tabelaHash[i];
        while (atual != NULL) {
            NoHash* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        tabelaHash[i] = NULL;
    }
}

/**
 * @brief Função principal para demonstração do sistema.
 */
int main() {
    printf("--- 🕵️‍♂️ Detective Quest: Nível Mestre (Tabela Hash) ---\n");
    inicializarHash();

    // 1. Entrada de Dados: Inserção Pista -> Suspeito
    printf("\n## 📥 Inserindo Pistas e Suspeitos na Tabela Hash\n");
    
    // Teste com colisões (ex: "arma" e "luvas" podem colidir dependendo do TAMANHO_HASH)
    inserirNaHash("Carteira roubada", "Bruno");
    inserirNaHash("Impressão digital", "Carlos");
    inserirNaHash("Relógio quebrado", "Bruno");
    inserirNaHash("Testemunha ocular", "Ana");
    inserirNaHash("Pegadas na lama", "Carlos");
    inserirNaHash("Bilhete anônimo", "Ana");
    inserirNaHash("Arma do crime", "Bruno"); // Pista adicional para Bruno

    // 2. Funcionalidade: Consultar Pistas por Suspeito
    printf("\n## 🔎 Consultando Pistas por Suspeito\n");
    consultarPistasPorSuspeito("Bruno");
    consultarPistasPorSuspeito("Ana");
    consultarPistasPorSuspeito("Carlos");

    // 3. Funcionalidade: Mostrar o Suspeito Mais Citado (Análise Final)
    printf("\n## 🏁 Análise Final\n");
    mostrarSuspeitoMaisCitado();

    // 4. Limpeza da memória
    liberarHash();
    printf("\n--- 🧹 Memória liberada. Fim da simulação. ---\n");

    return 0;
}
