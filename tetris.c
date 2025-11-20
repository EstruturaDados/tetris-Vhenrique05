#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha

// constantes

// tamanho máximo da fila
#define MAX_QUEUE 5
// tamanho máximo da pilha
#define MAX_STACK 3

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

// estrutura para armazenamento de dados da pilha
typedef struct {
	Peca data[MAX_STACK]; // dados da pilha
	unsigned int size; // índice do tamanho da fila
	// usei "unsigned int", pois não teremos índices negativos
} Pilha;

// setup
bool iniciarFila();
bool iniciarPilha();

// funções de utilidades
Peca gerarPeca();
bool filaCheia();
bool filaVazia();
bool pilhaCheia();
bool pilhaVazia();

bool enqueue(Peca peca);
bool dequeue(Peca* peca);
bool push(Peca peca);
bool pop(Peca* peca);

// ações
void exibirMenu();
void exibirFilaAtual();
void exibirPilhaAtual();
void removerPecaDaLista();
void adicionarPecaNaLista();
void reservarPecaNaPilha();
void usarPecaDaPilha();

// encerramento
void liberarFila();
void liberarPilha();

// variáveis globais
Fila* fila;
Pilha* pilha;
static unsigned int nid = 0; // número de id

// implementação das funções

// aloca a fila na memória
bool iniciarFila() {
	fila = (Fila*)malloc(sizeof(Fila));

	// se falhou, fila será nulo
	if (fila == NULL) {
		return false;
	}

	fila->head = 0;
	fila->size = 0;
	fila->tail = 0;

	return true;
}

// aloca a fila na memória
bool iniciarPilha() {
	pilha = (Pilha*)malloc(sizeof(Pilha));

	if (pilha == NULL) {
		return false;
	}

	pilha->size = 0;

	return true;
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

	return fila->size <= 0;
}

bool pilhaCheia() {
	// verifica se a pilha foi iniciada
	if (pilha == NULL) return false;

	return pilha->size >= MAX_STACK;
}

bool pilhaVazia() {
	// verifica se a pilha foi iniciada
	if (pilha == NULL) return false;

	return pilha->size <= 0;
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

// adiciona uma peca na pilha
bool push(const Peca peca) {
	// retorna falha se a pilha não tiver sido iniciada
	// ou se ela estiver cheia
	if (pilha == NULL || pilhaCheia()) return false;

	// ele aumenta o tamanho da pilha e lê o seu valor
	pilha->data[pilha->size] = peca;
	pilha->size++;

	return true;
}

// remove uma peça da pilha, e o "salva" na variável "peca"
bool pop(Peca* peca) {
	// retorna falha se a pilha não tiver sido iniciada,
	// ou se ela estiver vazia,
	if (pilha == NULL || pilhaVazia()) return false;

	// diminui o tamanho da lista
	pilha->size--;

	// se a variável peca não tiver sido iniciada,
	// pulamos o processo de transportar o valor da peça:
	if (peca != NULL)
		// como o valor já foi subtraído, não há necessidade de escrever "[size - 1]"
		*peca = pilha->data[pilha->size];

	return true;
}

void exibirMenu() {
	unsigned int escolha;
	do {
		printf("-------[Menu Principal]-------\n");
		printf(" > Selecione uma ação:\n");
		printf(" - [1]: Jogar peça (remover da frente da fila)\n");
		printf(" - [2]: Inserir nova peça (adicionar no final da fila)\n");
		printf(" - [3]: Reservar peça da frente da fila (push)\n");
		printf(" - [4]: Usar peça reservada (pop)\n");
		printf(" - [5]: Exibir o estado atual (fila e pilha)\n");
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
			removerPecaDaLista();
			break;

		case 2:
			adicionarPecaNaLista();
			break;

		case 3:
			reservarPecaNaPilha();
			break;

		case 4:
			usarPecaDaPilha();
			break;

		case 5:
			exibirFilaAtual();
			exibirPilhaAtual();
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

	if (filaVazia()) {
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

// exibe o estado atual da pilha
void exibirPilhaAtual() {
	printf("[*] Exibindo estado atual da pilha...\n");

	// verifica se a fila foi iniciada
	if (pilha == NULL) {
		printf("[!] A pilha não foi iniciada...\n");
		return;
	}

	if (pilhaVazia()) {
		printf("[-] Pilha vazia\n");
		return;
	}

	printf("[*] Pilha:\n[>] \t");

	// faço um for-loop para ler todos os itens (do topo para a base)
	for (int i = pilha->size - 1; i >= 0; --i) {
		// usa o ponteiro "*p" para ler os dados
		const Peca* p = &pilha->data[i];
		printf("[%d - %c] ", p->id, p->nome);
	}

	printf("\n");
}

void removerPecaDaLista() {
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
	exibirPilhaAtual();
}

void adicionarPecaNaLista() {
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
	exibirPilhaAtual();
}

// se houver espaço na pilha,
// envia a próxima peça (da fila) para a pilha (reserva)
// e logo em seguida, gera uma nova peça para repor a fila
void reservarPecaNaPilha() {
	printf("[*] Reservando peça da frente da fila (push)...\n");

	// realiza a checagem de espaço antes de realizar qualquer operação
	// e cancela caso não houver
	if (filaVazia()) {
		printf("[!] A fila está vazia\n");
		return;
	}

	if (pilhaCheia()) {
		printf("[!] A pilha está cheia\n");
		return;
	}

	// peça para reservar
	Peca peca_removida;

	// tenta retirar ela da fila
	if (!dequeue(&peca_removida)) {
		printf("[!] Falha ao tentar remover peça para reservar\n");

		// cancela operação devido à ocorrência de um erro:
		return;
	}

	// tenta inserir ela na pilha
	if (!push(peca_removida)) {
		printf("[!] Falha ao tentar inserir peça na reserva\n");

		// cancela operação devido à ocorrência de um erro:
		return;
	}

	printf("[*] Peça reservada: [%d - %c]\n", peca_removida.id, peca_removida.nome);

	// se falhar em repor a peça, apenas mostra mensagem de erro
	if (!enqueue(gerarPeca())) {
		printf("[!] Falha ao tentar repor peça na fila\n");
	}

	// exibe o estado atual após remoção de peça
	exibirFilaAtual();
	exibirPilhaAtual();
}

// se houver espaço na fila,
// envia a próxima peça (reservada) para a fila
void usarPecaDaPilha() {
	printf("[*] Usando peça reservada (pop)...\n");

	// realiza a checagem de espaço antes de realizar qualquer operação
	// e cancela caso não houver
	if (pilhaVazia()) {
		printf("[!] A pilha está vazia\n");
		return;
	}

	if (filaCheia()) {
		printf("[!] A fila está cheia\n");
		return;
	}

	// peça para retirar da reserva
	Peca peca_reserva;

	// tenta retirar ela da pilha
	if (!pop(&peca_reserva)) {
		printf("[!] Falha ao tentar remover peça da reserva\n");

		// cancela operação devido à ocorrência de um erro:
		return;
	}

	// tenta inserir ela na fila
	if (!enqueue(peca_reserva)) {
		printf("[!] Falha ao tentar inserir peça, para reservar\n");

		// cancela operação devido à ocorrência de um erro:
		return;
	}

	printf("[*] Peça retirada: [%d - %c]\n", peca_reserva.id, peca_reserva.nome);

	// exibe o estado atual após adição de peça
	exibirFilaAtual();
	exibirPilhaAtual();
}

// libera o espaço alocado na memória
void liberarFila() { free(fila); }

// libera o espaço alocado na memória
void liberarPilha() { free(pilha); }


int main() {
	// configura o locale e a seed do gerador de números aleatórios
	setlocale(LC_ALL, "portuguese");
	srand(time(NULL));

	if (!iniciarFila()) {
		printf("[!] Falha ao alocar a fila na memória\n");
		return -1;
	}

	printf("[*] Alocação da fila na memória realizado com sucesso\n");

	if (!iniciarPilha()) {
		printf("[!] Falha ao alocar a pilha na memória\n");
		return -1;
	}

	printf("[*] Alocação da pilha na memória realizado com sucesso\n");

	// inicia os valores da fila, com valores aleatórios, até sua capacidade máxima
	for (int i = 0; i < MAX_QUEUE; i++)
		enqueue(gerarPeca());

	exibirMenu();

	liberarFila();
	liberarPilha();

	return 0;
}
