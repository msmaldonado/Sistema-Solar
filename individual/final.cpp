#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cctype> 
#include <string>   
#include <fstream>

using namespace std;

int main (int argc, char *argv[]){

  int numero;

	cout << "\n\t\tPulsa un Número \n\t1 programa para calcular energia, momento angular y excentricidad para un planeta en un tiempo \n\t2 para dibujar los planetas \n\t3 para acabar el programa principal \n Numero =   " ; 
	cin >> numero;
	
  while (true) {
  	while(!(numero >=1 && numero <= 3)){
  		cout << "\nNÚMERO ERRÓNEO\n\t\tPulsa un Número \n\t1 programa para calcular energia, momento angular y excentricidad para un planeta en un tiempo \n\t2 para dibujar los planetas \n\t3 para acabar el programa principal \n Numero =   " ; 
		cin >> numero;
  	}

	if (numero == 1)
		//system("g++ ./parte1/parte1.cpp -o ./parte1/mecanica1");
		system("./parte1/mecanica1");	
	else if (numero == 2)
		system("./parte2/mecanica2");
	else if (numero == 3)
		exit(0);
	
	cout << "\n\t\tPulsa un Número \n\t1 programa para calcular energia, momento angular y excentricidad para un planeta en un tiempo \n\t2 para dibujar los planetas \n\t3 para acabar el programa principal \n Numero =   " ; 
	cin >> numero;
  }

}//main