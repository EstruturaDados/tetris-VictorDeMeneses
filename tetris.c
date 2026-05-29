#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5

// Estrutura simples para a peça
typedef struct {
    char nome;
    int id;
} Peca;

// Variáveis globais para facilitar o acesso direto nas funções
Peca fila[TAMANHO];
int frente = 0;
int tras = 0;
int totalElementos = 0;
int proximoId = 0;

// Função para gerar uma peça automática
Peca gerarPeca() {
    char formatos[] = {'I', 'O', 'T', 'L'};
    Peca novaPeca;
    
    novaPeca.nome = formatos[rand() % 4]; // Escolhe uma letra aleatória
    novaPeca.id = proximoId;              // Define o ID atual
    proximoId++;                          // Incrementa para o próximo ID
    
    return novaPeca;
}

// Função para exibir a fila na tela
void exibirFila() {
    printf("\nFila de pecas: ");
    
    if (totalElementos == 0) {
        printf("(Vazia)\n");
        return;
    }

    int indice = frente;
    for (int i = 0; i < totalElementos; i++) {
        printf("[%c %d] ", fila[indice].nome, fila[indice].id);
        indice = (indice + 1) % TAMANHO; // Anda para a próxima posição do círculo
    }
    printf("\n");
}

// Função para jogar a peça (Remover da frente)
void dequeue() {
    if (totalElementos == 0) {
        printf("\n[Erro] A fila esta vazia! Nao ha pecas para jogar.\n");
        return;
    }

    printf("\nVoce jogou a peca: [%c %d]!\n", fila[frente].nome, fila[frente].id);
    
    frente = (frente + 1) % TAMANHO; // Move a frente para o próximo elemento
    totalElementos--;
}

// Função para adicionar uma nova peça (Inserir no final)
void enqueue() {
    if (totalElementos == TAMANHO) {
        printf("\n[Erro] A fila esta cheia! Jogue uma peca antes.\n");
        return;
    }

    Peca novaPeca = gerarPeca();
    fila[tras] = novaPeca;
    
    printf("\nNova peca gerada e guardada: [%c %d]\n", novaPeca.nome, novaPeca.id);
    
    tras = (tras + 1) % TAMANHO; // Move o final para a próxima posição livre
    totalElementos++;
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de aleatórios
    int opcao = -1;

    // Preenche a fila inicialmente com 5 peças
    for (int i = 0; i < TAMANHO; i++) {
        fila[tras] = gerarPeca();
        tras = (tras + 1) % TAMANHO;
        totalElementos++;
    }

    // Menu do Jogo
    while (opcao != 0) {
        exibirFila();
        
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            dequeue();
        } else if (opcao == 2) {
            enqueue();
        } else if (opcao != 0) {
            printf("\nOpcao invalida!\n");
        }
        printf("-------------------------------------------\n");
    }

    printf("\nJogo encerrado.\n");
    return 0;
}