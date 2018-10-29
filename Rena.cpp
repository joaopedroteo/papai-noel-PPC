#include <iostream>
#include "Rena.hpp"
#include <string>

using namespace std;

Rena::Rena(string nome){
    this->nome = nome;
}

Rena::~Rena(){
}

string Rena::getNome(){
    return this->nome;
}


