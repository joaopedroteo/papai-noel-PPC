#ifndef RENA_HPP
#define RENA_HPP

#include <string>
using namespace std;

class Rena {
private:
    string nome;
    bool inserida;
public:
    Rena();
    Rena(string nome);
    ~Rena();
    string getNome();
    void setNome(string nome);
    void inverteValorInserida();
    bool estaInserida();
};


#endif
