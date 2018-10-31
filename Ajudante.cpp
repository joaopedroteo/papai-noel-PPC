#include <iostream>
#include "Ajudante.hpp"
#include <string>

using namespace std;

Ajudante::Ajudante() {
    this->nome = "";
    this->inserido = false;
}

Ajudante::Ajudante(string nome){
    this->nome = nome;
    this->inserido = false;
}

Ajudante::~Ajudante(){
}

string Ajudante::getNome(){
    return this->nome;
}

void Ajudante::setNome(string nome){
    if(this->nome.compare("") == 0) {
        this->nome = nome;
    }
}

void Ajudante::inverteValorInserido() {
    this->inserido = !this->inserido;
}

bool Ajudante::estaInserido() {
    return this->inserido;
}

