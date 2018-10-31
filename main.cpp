#include <iostream>
#include <omp.h>
#include <unistd.h>

#include "PapaiNoel.cpp"
#include "Ajudante.cpp"


using namespace std;

#define NUMERO_DE_RENAS 9
#define NUMERO_DE_ELFOS 10
#define TAMANHO_GRUPO_ELFOS 3

void insereAjudante(Ajudante** bufferAjudante, Ajudante* ajudante, int* posicaoInserir){
	bufferAjudante[*posicaoInserir] = ajudante;
	(*posicaoInserir)++;
	ajudante->inverteValorInserido();
	cout << ajudante->getNome() << " entrou no buffer." << endl;
}

Ajudante** insereRenas(Ajudante** renas, bool* bufferRenasCheio) {
	// Cria o buffer de renas 
	Ajudante** bufferRenas = new Ajudante*[NUMERO_DE_RENAS];
	int quantidade = 0;
	#pragma omp parallel for shared(quantidade, bufferRenas)
	for(int i = 0; i < NUMERO_DE_RENAS; i++) {
		//Tempo que a Rena está dormindo
		double tempo = (rand() % 10) * 0.1;
		sleep(tempo);
		//Tentamos inserir a rena enquanto ela não estiver no buffer
		while(!renas[i]->estaInserido()) {
			#pragma omp critical
			{
				insereAjudante(bufferRenas, renas[i], &quantidade);
			}
		}
	}
	*bufferRenasCheio = true;
	return bufferRenas;
}

Ajudante** insereElfos(Ajudante** elfos, bool* bufferElfosCheio) {
	// Cria o buffer de elfos
	Ajudante** bufferElfos = new Ajudante*[TAMANHO_GRUPO_ELFOS];
	int quantidade = 0;
	//Variável que indica se as três posições do buffer já estão ocupadas
	bool bufferCheio = false;
	#pragma omp parallel for shared(quantidade, bufferElfos)
	for(int i = 0; i < NUMERO_DE_ELFOS; i++) {
		// Tempo que os elfos estão fabricando brinquedos
		double tempo = (rand() % 10) * 0.05;
		sleep(tempo);
		// Se o elfo está inserido, isso significa que recentemente ele recebeu ajuda do papai noel....
		// Logo, ele deve voltar a fabricar brinquedos em seguida.
		if (elfos[i]->estaInserido()){
			sleep(rand() % 2);
			elfos[i]->inverteValorInserido();
		}
		//Tentamos inserir o elfo enquanto ele não estiver no buffer e o buffer não estiver cheio
		while(!elfos[i]->estaInserido() and !bufferCheio) {
			#pragma omp critical
			{
				//Verifica se a quantidade atual de elfos no buffer é menor que o do tamanho do grupo de elfos.
				bufferCheio = !(quantidade < TAMANHO_GRUPO_ELFOS);
				if(!bufferCheio) {
					insereAjudante(bufferElfos, elfos[i], &quantidade);
				}
			}
		}
	}
	*bufferElfosCheio = true;
	return bufferElfos;
}

//Imprime na tela os ajudantes que irão pedir ajuda ao papai noel
void ajudantesPedindoAjuda(Ajudante** ajudantes, int tamanho){
	cout << ajudantes[tamanho-1]->getNome() << " está pedindo ajuda para os ajudantes: ";
	for(int i = tamanho-1; i >= 0; i--){
		cout << ajudantes[i]->getNome() << " ";
	}
	cout << endl;
}

int main() {
	srand(time(0));
	cout << "INICIO" << endl;

	//Inicialização do papai noel, renas e elfos.
	PapaiNoel papaiNoel;
	Ajudante** renas = new Ajudante*[NUMERO_DE_RENAS];
	Ajudante** elfos = new Ajudante*[NUMERO_DE_ELFOS];

	//Variáveis que indicam o estado do buffer
	bool bufferRenasCheio = false;
	bool bufferElfosCheio = false;

	//Cria as renas e os elfos do problema.
	omp_set_nested(2);
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			#pragma omp parallel for
			for(int i = 0; i < NUMERO_DE_RENAS; i++) {
				renas[i] = new Ajudante("Rena" + to_string(i));
			}
		}
		#pragma omp section
		{
			#pragma omp parallel for
			for(int i = 0; i < NUMERO_DE_ELFOS; i++) {
				elfos[i] = new Ajudante("Elfo" + to_string(i));
			}
		}
	}
	//Fim da criação de renas e elfos

	//Divide o papai noel, renas e elfos em seções paralelas
	#pragma omp parallel sections
	{
		// Seção Papai Noel
		#pragma omp section
		{
			while(true) {
				if(!papaiNoel.estaDormindo()) {
					if(bufferRenasCheio) {
						#pragma omp critical
						{
							cout << "Papai noel está ajudando as renas..." << endl;
							sleep(0.5 + (rand()%10) * 0.5);
							bufferRenasCheio = false;
							papaiNoel.dormir();
						}
					} else {
						#pragma omp critical
						{
							cout << "Papai noel está ajudando os elfos..." << endl;
							sleep((rand() % 10) * 0.5);
							bufferElfosCheio = false;
							papaiNoel.dormir();
						}
					}
				}
			}
		}

		// Seção Renas
		#pragma omp section
		{
			while(true) {
				Ajudante** bufferRenas = insereRenas(renas, &bufferRenasCheio);

				if (!papaiNoel.estaDormindo()){
					cout << "Papai Noel esta ocupado com os elfos..." << endl;
					while(!papaiNoel.estaDormindo()){
					}
				}

				ajudantesPedindoAjuda(bufferRenas, NUMERO_DE_RENAS);
				#pragma omp critical
				{
					papaiNoel.acordar();
				}
				#pragma omp critical
				{
					papaiNoel.dormir();
					
					for(int i = 0; i < NUMERO_DE_RENAS; i++) {
						bufferRenas[i]->inverteValorInserido();
					}
					delete[] bufferRenas;
					
				}
				sleep(5 + rand() % 10);
			}
		}

		// Seção Elfos
		#pragma omp section
		{
			//Durante a execução do programa....
			while(true) {
				Ajudante** bufferElfos = insereElfos(elfos, &bufferElfosCheio);
				
				//Se o papai noel já estiver acordado (e.g auxiliando as renas) espera até que ele acorde
				if (!papaiNoel.estaDormindo()){
					cout << "Papai Noel esta ocupado com as renas..." << endl;
					while(!papaiNoel.estaDormindo()){
					}
				}
				ajudantesPedindoAjuda(bufferElfos, TAMANHO_GRUPO_ELFOS);
				
				#pragma omp critical
				{
					papaiNoel.acordar();
				}
				// reune com o papai noel
				while(!papaiNoel.estaDormindo()){
				}
				
				//PAPAI NOEL VOLTA A DORMIR
				#pragma omp critical
				{
					papaiNoel.dormir();
					
					delete[] bufferElfos;
				}
			}
		}
	}
	return 0;
}
