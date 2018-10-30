#include <iostream>
#include "Elfo.hpp"
#include <string>

using namespace std;

Elfo::Elfo() {
    this->nome = "";
    this->inserido = false;
}

Elfo::Elfo(string nome){
    this->nome = nome;
    this->inserido = false;
}

Elfo::~Elfo(){
}

string Elfo::getNome(){
    return this->nome;
}

void Elfo::setNome(string nome){
    if(this->nome.compare("") == 0) {
        this->nome = nome;
    }
}

void Elfo::inverteValorInserido() {
    this->inserido = !this->inserido;
}

bool Elfo::estaInserido() {
    return this->inserido;
}

