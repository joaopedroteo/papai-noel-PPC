#include <iostream>
#include "PapaiNoel.hpp"

using namespace std;

PapaiNoel::PapaiNoel(){
    this->dormindo = true;
}

PapaiNoel::~PapaiNoel(){
}

bool PapaiNoel::estaDormindo(){
    return this->dormindo;
}

