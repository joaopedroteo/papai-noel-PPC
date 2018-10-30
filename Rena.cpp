#include <iostream>
#include "Rena.hpp"
#include <string>

using namespace std;

Rena::Rena() {
    this->nome = "";
    this->inserida = false;
}

Rena::Rena(string nome){
    this->nome = nome;
    this->inserida = false;
}

Rena::~Rena(){
}

string Rena::getNome(){
    return this->nome;
}

void Rena::setNome(string nome){
    if(this->nome.compare("") == 0) {
        this->nome = nome;
    }
}

void Rena::inverteValorInserida() {
    this->inserida = !this->inserida;
}

bool Rena::estaInserida() {
    return this->inserida;
}

