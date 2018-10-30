#include <iostream>
#include <omp.h>
#include <unistd.h>

#include "PapaiNoel.cpp"
#include "Rena.cpp"
#include "Elfo.cpp"


using namespace std;

#define NUMERO_DE_RENAS 9
#define NUMERO_DE_ELFOS 10
#define TAMANHO_GRUPO_ELFOS 3

Rena** insereRenas(Rena** renas, bool* semaforoLivre) {
	Rena** bufferRenas = new Rena*[NUMERO_DE_RENAS];
	int quantidade = 0;
	#pragma omp parallel for shared(quantidade, bufferRenas)
	for(int i = 0; i < NUMERO_DE_RENAS; i++) {
		double tempo = (rand() % 10) * 0.65;
		sleep(tempo);
		#pragma omp critical
		{
			cout << "tempo dormindo " << renas[i]->getNome() << ": " << tempo << endl;
		}
		while(!renas[i]->estaInserida()) {
			#pragma omp critical
			{
				bufferRenas[quantidade] = renas[i];
				quantidade++;
				renas[i]->inverteValorInserida();
			}
		}
		sleep(0.5);
	}
	return bufferRenas;
}

Elfo** insereElfos(Elfo** elfos, bool* semaforoLivre) {
	Elfo** bufferElfos = new Elfo*[TAMANHO_GRUPO_ELFOS];
	int quantidade = 0;
	bool bufferCheio = false;
	#pragma omp parallel for shared(quantidade, bufferElfos)
	for(int i = 0; i < NUMERO_DE_ELFOS; i++) {
		double tempo = (rand() % 10) * 0.27; //FABRICANDO BRINQUEDOS
		sleep(tempo);
		#pragma omp critical
		{
			cout << "tempo dormindo " << elfos[i]->getNome() << ": " << tempo << endl;
		}
		while(!elfos[i]->estaInserido() and !bufferCheio) {
			#pragma omp critical
			{
				bufferCheio = !(quantidade < TAMANHO_GRUPO_ELFOS);
				if(!bufferCheio) {
					bufferElfos[quantidade] = elfos[i];
					quantidade++;
					elfos[i]->inverteValorInserido();
				}
			}
			sleep(0.5);
		}
	}
	return bufferElfos;
}

int main() {
	srand(time(0));
	cout << "INICIO" << endl;
	PapaiNoel papaiNoel;
	Rena** renas = new Rena*[NUMERO_DE_RENAS];
	Elfo** elfos = new Elfo*[NUMERO_DE_ELFOS];
	omp_set_nested(2);
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			#pragma omp parallel for
			for(int i = 0; i < NUMERO_DE_RENAS; i++) {
				renas[i] = new Rena("Rena" + to_string(i));
			}
		}
		#pragma omp section
		{
			#pragma omp parallel for
			for(int i = 0; i < NUMERO_DE_ELFOS; i++) {
				elfos[i] = new Elfo("Elfo" + to_string(i));
			}
		}
	}

	bool semaforoLivre = true;
	#pragma omp parallel sections shared(semaforoLivre)
	{
		// section rena
		#pragma omp section
		{
			int iteracao = 0;

			while(true) {
				Rena** bufferRenas = insereRenas(renas, &semaforoLivre);
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
						bufferRenas[i]->inverteValorInserida();
					}
					delete[] bufferRenas;
					
				}
				sleep(10);
			}
		}

		// section elfo
		#pragma omp section
		{
			int iteracao = 0;
			while(true) {
				Elfo** bufferElfos = insereElfos(elfos, &semaforoLivre);
				
				while(!papaiNoel.estaDormindo()){
					sleep(1);
					cout << "Papai noel ta ocupado" << endl;
				}
				cout << "elfo pedindo ajuda" << endl;
				#pragma omp critical
				{
					papaiNoel.acordar();
				}
				//ElfoS VAO ENTREGAR PRESENTES
				//E DORMIR 1 ANO
				//PAPAI NOEL VOLTA A DORMIR
				#pragma omp critical
				{
					papaiNoel.dormir();
					
					for(int  i = 0; i < TAMANHO_GRUPO_ELFOS; i++) {
						cout << bufferElfos[i]->getNome() << endl;
						bufferElfos[i]->inverteValorInserido();
					}
					
					delete[] bufferElfos;
				}
			}
		}

	}

	// while(true) !!!
	

	// Rena rena("reninha");
	// cout << rena.getNome() << endl;
	return 0;
}
