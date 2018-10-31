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
}

Ajudante** insereRenas(Ajudante** renas, bool* bufferRenasCheio) {
	Ajudante** bufferRenas = new Ajudante*[NUMERO_DE_RENAS];
	int quantidade = 0;
	#pragma omp parallel for shared(quantidade, bufferRenas)
	for(int i = 0; i < NUMERO_DE_RENAS; i++) {
		double tempo = (rand() % 10) * 0.65;
		sleep(tempo);
		while(!renas[i]->estaInserido()) {
			#pragma omp critical
			{
				insereAjudante(bufferRenas, renas[i], &quantidade);
				// bufferRenas[quantidade] = renas[i];
				// quantidade++;
				// renas[i]->inverteValorInserido();
			}
		}
		sleep(0.5);
	}
	*bufferRenasCheio = true;
	return bufferRenas;
}

Ajudante** insereElfos(Ajudante** elfos, bool* bufferElfosCheio) {
	Ajudante** bufferElfos = new Ajudante*[TAMANHO_GRUPO_ELFOS];
	int quantidade = 0;
	bool bufferCheio = false;
	#pragma omp parallel for shared(quantidade, bufferElfos)
	for(int i = 0; i < NUMERO_DE_ELFOS; i++) {
		double tempo = (rand() % 10) * 0.27; // TEMPO FABRICANDO BRINQUEDOS
		sleep(tempo);
		if (elfos[i]->estaInserido()){
			#pragma omp critical
			{
				cout << elfos[i]->getNome() << " indo trabalhar..." << endl;
			}
			sleep(3);
			elfos[i]->inverteValorInserido();
		}
		while(!elfos[i]->estaInserido() and !bufferCheio) {
			#pragma omp critical
			{
				bufferCheio = !(quantidade < TAMANHO_GRUPO_ELFOS);
				if(!bufferCheio) {
					insereAjudante(bufferElfos, elfos[i], &quantidade);
					// bufferElfos[quantidade] = elfos[i];
					// quantidade++;
					// elfos[i]->inverteValorInserido();
				}
			}
			sleep(0.5);
		}
	}
	*bufferElfosCheio = true;
	return bufferElfos;
}

int main() {
	srand(time(0));
	cout << "INICIO" << endl;
	PapaiNoel papaiNoel;
	Ajudante** renas = new Ajudante*[NUMERO_DE_RENAS];
	Ajudante** elfos = new Ajudante*[NUMERO_DE_ELFOS];
	bool bufferRenasCheio = false;
	bool bufferElfosCheio = false;
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

	#pragma omp parallel sections//E DORMIR 1 ANO
	{
		// section papai noel
		#pragma omp section
		{
			while(true) {
				if(!papaiNoel.estaDormindo()) {
					if(bufferRenasCheio) {
						#pragma omp critical
						{
							cout << "ajudando renas" << endl;
							sleep(0.8);
							
							bufferRenasCheio = false;
							papaiNoel.dormir();
						}
					} else {
						#pragma omp critical
						{
							cout << "ajudando elfos" << endl;
							sleep(0.3);
							bufferElfosCheio = false;
							papaiNoel.dormir();
						}
					}
				}
			}
		}
		// section rena
		#pragma omp section
		{

			while(true) {
				Ajudante** bufferRenas = insereRenas(renas, &bufferRenasCheio);

				while(!papaiNoel.estaDormindo()){
					cout << "Papai noel ta ocupado" << endl;
				}
				cout << "rena pedindo ajuda" << endl;
				#pragma omp critical
				{
					papaiNoel.acordar();
				}
				//RENAS VAO ENTREGAR PRESENTES
				//E DORMIR 1 ANO
				//PAPAI NOEL VOLTA A DORMIR
				#pragma omp critical
				{
					papaiNoel.dormir();
					
					for(int i = 0; i < NUMERO_DE_RENAS; i++) {
						cout << bufferRenas[i]->getNome() << endl;
						bufferRenas[i]->inverteValorInserido();
					}
					delete[] bufferRenas;
					
				}
				sleep(10);
			}
		}

		// section elfo
		#pragma omp section
		{
			while(true) {
				Ajudante** bufferElfos = insereElfos(elfos, &bufferElfosCheio);
				
				while(!papaiNoel.estaDormindo()){
					sleep(1);
					cout << "Papai noel ta ocupado" << endl;
				}
			
				
				cout << "elfo pedindo ajuda" << endl;
				#pragma omp critical
				{
					
					for(int i = 0; i < TAMANHO_GRUPO_ELFOS; i++){
						cout << bufferElfos[i]->getNome() << endl;
					}
					
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
