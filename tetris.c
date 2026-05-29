#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// --- ESTRUTURAS GLOBAIS DE DADOS ---
Peca fila[TAM_FILA];
int frente = 0;
int tras = 0;
int totalFila = 0;

Peca pilha[TAM_PILHA];
int topo = -1; // -1 significa pilha vazia

// --- CONTROLE DE ID ---
int proximoId = 0;

// --- PROTÓTIPOS DAS FUNÇÕES ---
Peca gerarPeca();
void inicializarFila();
void reporFila();
void jogarPeca();
void enviarParaPilha();
void usarPecaPilha();
void trocarPecaAtual();
void trocaMultipla();
void exibirEstado();

int main() {
    srand(time(NULL));
    int opcao = -1;

    // Inicializa a fila com as 5 primeiras peças futuras
    inicializarFila();

    printf("=== MOTOR DO JOGO: TETRIS STACK (AVANCADO) ===\n");

    while (opcao != 0) {
        exibirEstado();

        printf("\nOpcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(); break;
            case 2: enviarParaPilha(); break;
            case 3: usarPecaPilha(); break;
            case 4: trocarPecaAtual(); break;
            case 5: trocaMultipla(); break;
            case 0: printf("\nEncerrando o Tetris Stack. Sistema finalizado!\n"); break;
            default: printf("\n[ERRO] Opcao invalida!\n");
        }
        printf("\n-------------------------------------------------------------\n");
    }

    return 0;
}

// Gera automaticamente peças com formato aleatório e ID único sequencial
Peca gerarPeca() {
    char formatos[] = {'I', 'O', 'T', 'L'};
    Peca novaPeca;
    novaPeca.nome = formatos[rand() % 4];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// Preenche a fila pela primeira vez
void inicializarFila() {
    for (int i = 0; i < TAM_FILA; i++) {
        fila[tras] = gerarPeca();
        tras = (tras + 1) % TAM_FILA;
        totalFila++;
    }
}

// Função auxiliar para garantir que a fila se mantenha cheia
void reporFila() {
    if (totalFila < TAM_FILA) {
        fila[tras] = gerarPeca();
        tras = (tras + 1) % TAM_FILA;
        totalFila++;
    }
}

// Opção 1: Dequeue (Remove a peça da frente)
void jogarPeca() {
    if (totalFila == 0) {
        printf("\n[AVISO] Fila vazia!\n");
        return;
    }
    printf("\nAcao: Voce jogou a peca [%c %d]!\n", fila[frente].nome, fila[frente].id);
    frente = (frente + 1) % TAM_FILA;
    totalFila--;
    reporFila();
}

// Opção 2: Envia a peça da frente da fila para o topo da pilha (Push)
void enviarParaPilha() {
    if (topo >= TAM_PILHA - 1) {
        printf("\n[AVISO] A pilha de reserva esta cheia!\n");
        return;
    }
    if (totalFila == 0) {
        printf("\n[AVISO] Nao ha pecas na fila para reservar!\n");
        return;
    }

    Peca pecaMudar = fila[frente];
    frente = (frente + 1) % TAM_FILA;
    totalFila--;

    topo++;
    pilha[topo] = pecaMudar;

    printf("\nAcao: Peca [%c %d] enviada para a pilha de reserva.\n", pecaMudar.nome, pecaMudar.id);
    reporFila();
}

// Opção 3: Usa a peça do topo da pilha (Pop)
void usarPecaPilha() {
    if (topo == -1) {
        printf("\n[AVISO] Pilha de reserva vazia!\n");
        return;
    }
    printf("\nAcao: Voce usou a peca reservada [%c %d]!\n", pilha[topo].nome, pilha[topo].id);
    topo--;
}

// Opção 4: Substitui a peça da frente da fila com o topo da pilha
void trocarPecaAtual() {
    if (totalFila == 0 || topo == -1) {
        printf("\n[AVISO] Operacao impossivel: A fila ou a pilha estao vazias!\n");
        return;
    }

    // Troca simples usando uma variável temporária
    Peca temp = fila[frente];
    fila[frente] = pilha[topo];
    pilha[topo] = temp;

    printf("\nAcao: Troca simples realizada entre a frente da fila e o topo da pilha.\n");
}

// Opção 5: Alterna os 3 primeiros elementos da Fila com os 3 elementos da Pilha
void trocaMultipla() {
    // Validação estrita: Ambas as estruturas precisam ter pelo menos 3 elementos
    if (totalFila < 3 || topo < 2) {
        printf("\n[AVISO] Operacao impossivel: Sao necessarias pelo menos 3 pecas na fila E na pilha!\n");
        return;
    }

    // Como a fila é circular, precisamos mapear os índices corretos sequencialmente
    int idxFila0 = frente;
    int idxFila1 = (frente + 1) % TAM_FILA;
    int idxFila2 = (frente + 2) % TAM_FILA;

    // Mapeamento da pilha (Lembrando que o topo é o primeiro elemento visualizado)
    int idxPilhaTopo = topo;       // 1º elemento
    int idxPilhaMeio = topo - 1;   // 2º elemento
    int idxPilhaBase = topo - 2;   // 3º elemento

    Peca temp;

    // 1ª Troca: Frente da fila com o Topo da pilha
    temp = fila[idxFila0];
    fila[idxFila0] = pilha[idxPilhaTopo];
    pilha[idxPilhaTopo] = temp;

    // 2ª Troca: Segundo da fila com o Meio da pilha
    temp = fila[idxFila1];
    fila[idxFila1] = pilha[idxPilhaMeio];
    pilha[idxPilhaMeio] = temp;

    // 3ª Troca: Terceiro da fila com a Base (terceiro) da pilha
    temp = fila[idxFila2];
    fila[idxFila2] = pilha[idxPilhaBase];
    pilha[idxPilhaBase] = temp;

    printf("\nAcao: Troca em bloco realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// Exibe o estado gráfico e textual do jogo no terminal
void exibirEstado() {
    printf("\nEstado atual:\n");

    // Print da Fila Circular
    printf("Fila de pecas: ");
    int idx = frente;
    for (int i = 0; i < totalFila; i++) {
        printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");

    // Print da Pilha Linear (Do Topo para a Base)
    printf("Pilha de reserva (Topo -> base): ");
    if (topo == -1) {
        printf("(Vazia)");
    } else {
        for (int i = topo; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
    }
    printf("\n");
}