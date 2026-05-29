#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição dos limites de tamanho das estruturas
#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador numérico único
} Peca;

// --- VARIÁVEIS GLOBAIS DA FILA CIRCULAR ---
Peca fila[TAM_FILA];
int frente = 0;
int tras = 0;
int totalFila = 0;

// --- VARIÁVEIS GLOBAIS DA PILHA LINEAR ---
Peca pilha[TAM_PILHA];
int topo = -1; // Começa em -1 indicando pilha vazia

// --- VARIÁVEIS DE CONTROLE DO JOGO ---
int proximoId = 0; // Garante o ID único e sequencial para cada peça criada

// --- PROTÓTIPOS DAS FUNÇÕES ---
Peca gerarPeca();
void inicializarFila();
void adicionarFila();
void jogarPeca();
void reservarPeca();
void usarPecaReservada();
void exibirEstadoAtual();

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    int opcao = -1;

    // Inicializa o jogo preenchendo a fila de peças futuras (começa com 5 peças)
    inicializarFila();

    printf("=== MOTOR DO JOGO: TETRIS STACK (INTERMEDIARIO) ===\n\n");

    // Loop principal do menu interativo
    while (opcao != 0) {
        exibirEstadoAtual();

        printf("\nOpcoes de Acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            jogarPeca();
        } else if (opcao == 2) {
            reservarPeca();
        } else if (opcao == 3) {
            usarPecaReservada();
        } else if (opcao != 0) {
            printf("\n[AVISO] Opcao invalida! Digite um numero do menu.\n");
        }
        printf("\n---------------------------------------------------\n");
    }

    printf("\nSistema Tetris Stack encerrado com sucesso.\n");
    return 0;
}

// Cria uma peça automaticamente com formato aleatório e ID sequencial único
Peca gerarPeca() {
    char formatos[] = {'I', 'O', 'T', 'L'};
    Peca novaPeca;
    
    novaPeca.nome = formatos[rand() % 4];
    novaPeca.id = proximoId;
    proximoId++; // Incrementa globalmente para a próxima criação
    
    return novaPeca;
}

// Preenche a fila circular pela primeira vez até sua capacidade total
void inicializarFila() {
    for (int i = 0; i < TAM_FILA; i++) {
        fila[tras] = gerarPeca();
        tras = (tras + 1) % TAM_FILA;
        totalFila++;
    }
}

// Função auxiliar para reinserir uma peça no fim da fila sempre que uma sai
void adicionarFila() {
    if (totalFila < TAM_FILA) {
        fila[tras] = gerarPeca();
        tras = (tras + 1) % TAM_FILA; // Movimentação circular do índice de trás
        totalFila++;
    }
}

// Ação 1: Remove a peça da frente da fila e simula seu posicionamento no jogo
void jogarPeca() {
    if (totalFila == 0) {
        printf("\n[ERRO] Nao ha pecas na fila para jogar.\n");
        return;
    }

    // Pega a peça da frente
    Peca pecaSaindo = fila[frente];
    
    // Atualiza o índice da frente de forma circular
    frente = (frente + 1) % TAM_FILA;
    totalFila--;

    printf("\n-> Voce jogou a peca: [%c %d]!\n", pecaSaindo.nome, pecaSaindo.id);

    // Regra do jogo: Uma nova peça entra automaticamente no fim da fila
    adicionarFila();
}

// Ação 2: Tira a peça da frente da fila e a coloca no topo da pilha de reserva
void reservarPeca() {
    // 1. Verifica se a pilha está cheia antes de mover
    if (topo >= TAM_PILHA - 1) {
        printf("\n[AVISO] Pilha de reserva cheia! Use uma peca reservada antes.\n");
        return;
    }

    // 2. Remove a peça da frente da fila
    Peca pecaParaReserva = fila[frente];
    frente = (frente + 1) % TAM_FILA;
    totalFila--;

    // 3. Insere a peça no topo da pilha (Push)
    topo++;
    pilha[topo] = pecaParaReserva;

    printf("\n-> Peca [%c %d] movida para a reserva.\n", pecaParaReserva.nome, pecaParaReserva.id);

    // 4. Repõe imediatamente a fila com uma nova peça gerada automaticamente
    adicionarFila();
}

// Ação 3: Remove e utiliza a peça que está no topo da pilha (LIFO)
void usarPecaReservada() {
    // Verifica se há alguma peça na reserva
    if (topo == -1) {
        printf("\n[AVISO] Nao ha pecas na pilha de reserva!\n");
        return;
    }

    // Pega a peça do topo da pilha (Pop)
    Peca pecaReservadaUsada = pilha[topo];
    topo--; // Reduz o topo indicando a remoção

    printf("\n-> Voce usou a peca reservada: [%c %d]!\n", pecaReservadaUsada.nome, pecaReservadaUsada.id);
    // Atenção: Peças retiradas da reserva não geram novas peças na fila, 
    // pois a fila já é reposta no momento em que ela foi guardada.
}

// Exibe o estado atual formatado da fila e da pilha
void exibirEstadoAtual() {
    printf("\nEstado atual:\n");

    // --- Renderização da Fila ---
    printf("Fila de pecas: ");
    int indiceFila = frente;
    for (int i = 0; i < totalFila; i++) {
        printf("[%c %d] ", fila[indiceFila].nome, fila[indiceFila].id);
        indiceFila = (indiceFila + 1) % TAM_FILA;
    }
    printf("\n");

    // --- Renderização da Pilha (Do Topo para a Base) ---
    printf("Pilha de reserva (Topo -> Base): ");
    if (topo == -1) {
        printf("(Vazia)");
    } else {
        for (int i = topo; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
    }
    printf("\n");
}