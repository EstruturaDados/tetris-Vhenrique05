#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.


// constantes

// tamanho máximo da fila
#define MAX_QUEUE 5
// tamanho máximo da pilha
#define MAX_STACK 5

// tipos de peças disponíveis
constexpr char TipoDePeca[4] = {'I', 'O', 'T', 'L'};

// estruturas

// estrutura para armazenamento de dados de peças
typedef struct {
	char nome; // nome: letra da peça
	unsigned int id; // id: número sequêncial
	// usei "unsigned int", pois não teremos índices negativos
} Peca;

// estrutura para armazenamento de dados da fila
typedef struct {
	Peca data[MAX_QUEUE]; // dados da fila
	unsigned int head, tail, size; // índice da cabeça, cauda e tamanho da fila
	// usei "unsigned int", pois não teremos índices negativos
} Fila;

// setup
bool iniciarFila();

// funções de utilidades
Peca gerarPeca();
bool filaCheia();
bool filaVazia();

bool enqueue(Peca peca);
bool dequeue(Peca* peca);

// ações
void exibirMenu();
void exibirFilaAtual();
void removerPecaDaFrente();
void atualizarFinalDaFila();

// encerramento
void limparFila();

// variáveis globais
Fila* fila;
static unsigned int nid = 0; // número de id

// implementação das funções

// aloca a fila na memória
bool iniciarFila() {
	fila = (Fila*)calloc(MAX_QUEUE, sizeof(Fila));
	return fila != NULL;
}

// cria uma peça aleatória e retorna ela
Peca gerarPeca() {
	return (Peca){
		TipoDePeca[rand() % 4],
		nid++,
	};
}

bool filaCheia() {
	// verifica se a fila foi iniciada
	if (fila == NULL) return false;

	return fila->size >= MAX_QUEUE;
}

bool filaVazia() {
	// verifica se a fila foi iniciada
	if (fila == NULL) return false;

	return fila->size == 0;
}

// adiciona uma peca na fila
bool enqueue(const Peca peca) {
	// retorna falha se a fila não tiver sido iniciada
	// ou se ela estiver cheia
	if (fila == NULL || filaCheia()) return false;

	fila->data[fila->tail] = peca;
	fila->tail = (fila->tail + 1) % MAX_QUEUE;
	fila->size++;

	return true;
}

// remove uma peça da fila, e o "salva" na variável "peca"
bool dequeue(Peca* peca) {
	// retorna falha se a fila não tiver sido iniciada,
	// ou se ela estiver vazia,
	if (fila == NULL || filaVazia()) return false;

	// se a variável peca não tiver sido iniciada,
	// pulamos o processo de transportar o valor da "cabeça":
	if (peca != NULL)
		*peca = fila->data[fila->head];

	fila->head = (fila->head + 1) % MAX_QUEUE;
	fila->size--;

	return true;
}

void exibirMenu() {
	unsigned int escolha;
	do {
		printf("-------[Menu Principal]-------\n");
		printf(" > Selecione uma ação:\n");
		printf(" - [1]: Jogar peça (remover da frente da fila)\n");
		printf(" - [2]: Inserir nova peça (adicionar no final da fila)\n");
		printf(" - [3]: Exibir o estado da fila\n");
		printf(" - [0]: Encerra o jogo\n");
		printf("------------------------------\n");
		printf(">> ");
		scanf("%d", &escolha);

		printf("\n------------------------------\n");
		switch (escolha) {
		case 0:
			printf("[*] Saindo do jogo...\n");
			return; // esse return é valido para encerrar a função exibirMenu()

		case 1:
			printf("[*] Removendo peça...\n");

			Peca peca_removida;
			if (!dequeue(&peca_removida)) {
				// se: filaVazia() for verdadeiro -> avisa que a fila está vazia
				// senão: avisa falha (sem detalhes)
				filaVazia()
					? printf("[!] A fila está vazia\n")
					: printf("[!] Falha ao tentar remover peça\n");
			} else {
				printf("[*] Peça removida: [%d - %c]\n", peca_removida.id, peca_removida.nome);
			}

		// exibe o estado atual após remoção de peça
			exibirFilaAtual();
			break;

		case 2:
			printf("[*] Inserindo nova peça...\n");

			Peca nova_peca = gerarPeca();
			if (!enqueue(nova_peca)) {
				// se: filaCheia() for verdadeiro -> avisa que a fila está cheia
				// senão: avisa falha (sem detalhes)
				filaCheia()
					? printf("[!] A fila está cheia\n")
					: printf("[!] Falha ao tentar adicionar peça\n");
			} else {
				printf("[*] Peça adicionada: [%d - %c]\n", nova_peca.id, nova_peca.nome);
			}

		// exibe o estado atual após adição de peça
			exibirFilaAtual();
			break;

		case 3:
			exibirFilaAtual();
			break;

		default:
			printf("[!] Opção inválida\n");
			break;
		}
	} while (escolha != 0);
}

// exibe o estado atual da fila
void exibirFilaAtual() {
	printf("[*] Exibindo estado atual da fila...\n");

	// verifica se a fila foi iniciada
	if (fila == NULL) {
		printf("[!] A fila não foi iniciada...\n");
		return;
	}

	if (fila->size == 0) {
		printf("[-] Fila vazia\n");
		return;
	}

	printf("[*] Fila:\n[>] \t");

	// faço um for-loop usando o i e i_head para armazenar o índice e cabeça
	for (unsigned int i = 0, i_head; i < fila->size; i++) {
		// define i_head para o local da cabeça da fila
		i_head = (fila->head + i) % MAX_QUEUE;

		// usa o ponteiro "*p" para ler os dados
		const Peca* p = &fila->data[i_head];
		printf("[%d - %c] ", p->id, p->nome);
	}

	printf("\n");
}

void limparFila() { free(fila); }


int main() {
	// 🧩 Nível Novato: Fila de Peças Futuras
	//
	// - Crie uma struct Peca com os campos: tipo (char) e id (int).
	// - Implemente uma fila circular com capacidade para 5 peças.
	// - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
	// - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
	// - Exiba a fila após cada ação com uma função mostrarFila().
	// - Use um menu com opções como:
	//      1 - Jogar peça (remover da frente)
	//      0 - Sair
	// - A cada remoção, insira uma nova peça ao final da fila.


	// 🧠 Nível Aventureiro: Adição da Pilha de Reserva
	//
	// - Implemente uma pilha linear com capacidade para 3 peças.
	// - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
	// - Permita enviar uma peça da fila para a pilha (reserva).
	// - Crie um menu com opção:
	//      2 - Enviar peça da fila para a reserva (pilha)
	//      3 - Usar peça da reserva (remover do topo da pilha)
	// - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
	// - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


	// 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
	//
	// - Implemente interações avançadas entre as estruturas:
	//      4 - Trocar a peça da frente da fila com o topo da pilha
	//      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
	// - Para a opção 4:
	//      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
	//      Troque os elementos diretamente nos arrays.
	// - Para a opção 5:
	//      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
	//      Use a lógica de índice circular para acessar os primeiros da fila.
	// - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
	// - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
	// - O menu deve ficar assim:
	//      4 - Trocar peça da frente com topo da pilha
	//      5 - Trocar 3 primeiros da fila com os 3 da pilha

	// configura o locale e a seed do gerador de números aleatórios
	setlocale(LC_ALL, "portuguese");
	srand(time(NULL));

	if (!iniciarFila()) {
		printf("[!] Falha ao alocar a fila na memória\n");
		return -1;
	}

	printf("[*] Alocação da fila na memória realizado com sucesso\n");

	// inicia os valores da fila, com valores aleatórios, até sua capacidade máxima
	for (int i = 0; i < MAX_QUEUE; i++)
		enqueue(gerarPeca());

	exibirMenu();

	limparFila();

	return 0;
}
