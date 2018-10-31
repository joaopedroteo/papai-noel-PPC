#ifndef AJUDANTE_HPP
#define AJUDANTE_HPP

#include <string>
using namespace std;

class Ajudante {
private:
    string nome;
    bool inserido;
public:
    Ajudante();
    Ajudante(string nome);
    ~Ajudante();
    string getNome();
    void setNome(string nome);
    void inverteValorInserido();
    bool estaInserido();
};


#endif
