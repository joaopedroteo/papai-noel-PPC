#include <iostream>
#include "PapaiNoel.cpp"
#include "Rena.cpp"
using namespace std;

int main() {
	PapaiNoel papaiNoel;
	cout << papaiNoel.estaDormindo() << endl;
	Rena rena("reninha");
	cout << rena.getNome() << endl;
	return 0;
}
