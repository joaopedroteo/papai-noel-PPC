#include <iostream>
#include <omp.h>
#include <unistd.h>

#include "PapaiNoel.cpp"
#include "Rena.cpp"

using namespace std;

#define NUMERO_DE_RENAS 9
#define NUMERO_DE_ELFOS 10
#define TAMANHO_GRUPO_ELFOS 3

Rena* insereRenas(Rena* renas) {
	Rena* bufferRenas = new Rena[NUMERO_DE_RENAS];
	int quantidade = 0;
	#pragma omp parallel for shared(quantidade, bufferRenas)
	for(int i = 0; i < NUMERO_DE_RENAS; i++) {
		while(!renas[i].estaInserida()) {
			#pragma omp critical
			{
				cout << renas[i].getNome() << endl;
				bufferRenas[quantidade] = renas[i];
				quantidade++;
				renas[i].inverteValorInserida();
			}
		}
	}
	return bufferRenas;
}


int main() {
	cout << "INICIO" << endl;
	PapaiNoel papaiNoel;

	Rena* renas = new Rena[NUMERO_DE_RENAS];
	#pragma omp parallel for
	for(int i = 0; i < NUMERO_DE_RENAS; i++) {
		renas[i].setNome("Rena" + to_string(i));
	}

	// for (int i = 0; i < NUMERO_DE_RENAS; i++) {
	// 	cout << renas[i].getNome() << endl;
	// }
	
	// while(true) !!!
	while(papaiNoel.estaDormindo()) {
		Rena* bufferRenas = insereRenas(renas);
		while(!papaiNoel.estaDormindo()){
			sleep(1);
			cout << "Papai noel ta ocupado" << endl;
		}
		cout << "pedindo ajuda" << endl;
		papaiNoel.acordar();
		//RENAS VAO ENTREGAR PRESENTES
		//E DORMIR 1 ANO
		//PAPAI NOEL VOLTA A DORMIR
	}

	cout << papaiNoel.estaDormindo() << endl;
	// Rena rena("reninha");
	// cout << rena.getNome() << endl;
	return 0;
}
