#ifndef RENA_HPP
#define RENA_HPP

#include <string>
using namespace std;

class Rena {
private:
    string nome;
public:
    Rena(string nome);
    ~Rena();
    string getNome();
};


#endif
