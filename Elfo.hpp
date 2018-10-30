#ifndef ELFO_HPP
#define ELFO_HPP

#include <string>
using namespace std;

class Elfo {
private:
    string nome;
    bool inserido;
public:
    Elfo();
    Elfo(string nome);
    ~Elfo();
    string getNome();
    void setNome(string nome);
    void inverteValorInserido();
    bool estaInserido();
};


#endif
