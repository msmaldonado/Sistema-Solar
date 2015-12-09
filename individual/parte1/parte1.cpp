#include <vector>
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <cctype> 
#include <string>   
#include <fstream>

using namespace std;

double pos_final [3];
double pos_final_derivadas [3];
double momento [3];
double tiempo;
double energia;
double phi;
double u;
double a;
double periodo;
double si;

/*Periodo de los planetas en días*/
double periodoMercurio = 87.97;
double periodoVenus =224.7;
double periodoTierra = 365.26;
double periodoMarte = 686.98;
double periodoJupiter = 4332.6;
double periodoSaturno = 10759;
double periodoUrano = 30687;
double periodoNeptuno = 60784;
/*Masa de los planetas por si queremos calcular la energia usando la fórmula en la que aparece la masa*/
double masaMercurio = 3.3 *pow(10,23);
double masaVenus = 48.7 *pow(10,23);
double masaTierra = 59.7 *pow(10,23);
double masaMarte = 6.4 *pow(10,23);
double masaJupiter = 19000 *pow(10,23);
double masaSaturno = 5688 *pow(10,23);
double masaUrano = 868.6 *pow(10,23);
double masaNeptuno = 1024 *pow(10,23);


/*Método para calcular u que usará el método de Newton Raphson a partir de una phi*/
void  calcularExcentricidad(double epsilon){
	float u_antigua = M_PI;
	phi = (epsilon*(sin(u_antigua)-u_antigua*cos(u_antigua)) + si)/(1.0 -epsilon*cos(u_antigua));
	//Método de Newton-Raphson
	while (abs(u_antigua - phi) >= 0.00001){
		u_antigua = phi;
		phi = (epsilon*(sin(phi)-phi*cos(phi)) + si)/(1.0 -epsilon*cos(phi));
	}
	u= phi;
}

void Posicion(double lon, double epsilon, double p){
	//Actualizamos las variables para usarlas en energia sin tener que pasar parámetros
	periodo = p;
	a = lon;
	si = (2* M_PI *tiempo) / periodo ;
	calcularExcentricidad(epsilon);
	
	pos_final[0] = a * (cos(u) - epsilon);
	pos_final[1] = a * sqrt(1 - (epsilon*epsilon)) * sin(u) ;//sqrt(1-epsilon²) * sin(u) ;
	//declaramos una variable ayuda para hacer la derivada por cambio
	double ayuda = (2* M_PI * sqrt(pow(a,1/3)) / periodo )/(sqrt(pow(a,1/3))*(1-epsilon*cos(u)));
	pos_final_derivadas [0] = (-a* sin(u)*ayuda);
	pos_final_derivadas [1] = sqrt(1.0 - (epsilon*epsilon)) * cos (u) *ayuda *a;
	//Completamos la 3 coordenada de ambos vectores para hacer el producto vectorial
	pos_final[2] = 0;
	pos_final_derivadas [2] = 0;
}

/*Método para calcular la energia de un planeta en un tiempo. Varibles a y periodo actualizadas cuando calcula la posición*/
void calcularEnergia(){

	energia = ((pos_final_derivadas[0]* pos_final_derivadas[0] + pos_final_derivadas[1] * pos_final_derivadas[1])/2.0 ) - (4*M_PI*M_PI * a*a*a /(periodo*periodo))/(sqrt(pos_final[0] * pos_final[0] + pos_final[1] * pos_final[1])) ;
}
/*Método que nos calcula el momento angular es decir el producto vectorial de posición y velocidad, siempre en tres coordenadas*/
void calcularMomentoAngular(){

	momento [0] = pos_final[1]*pos_final_derivadas[2] - pos_final[2]*pos_final_derivadas[1];
	momento [1] = pos_final[2]*pos_final_derivadas[0] - pos_final[0]*pos_final_derivadas[2];
	momento [2] = pos_final[0]*pos_final_derivadas[1] - pos_final[1]*pos_final_derivadas[0];
}

int main (int argc, char *argv[]){

	while(true){//Se está ejecutando hasta que lo detenemos
	 //Variables que almacena el planeta y si queremos finalizar
 	 char letra;
 	 char finalizar;
 	 cout<< "\tIntroduce un planeta de la lista pulsando la letra correspondiente \n" << "\tMercurio --> A\n" << "\tVenus --> B\n" << "\tTierra --> C\n" << "\tMarte --> D\n" << "\tJupiter --> E\n";
 	 cout << "\tSaturno --> F\n"<<  "\tUrano --> G\n" << "\tNeptuno --> H  ";
 	 cin >> letra;
 	 //Comprobamos que se ha introducido una letra de la lista
 		while (!(letra ==  'a' || letra ==  'b' || letra ==  'c' || letra ==  'd' || letra ==  'e' || letra ==  'f' || letra ==  'g' || letra ==  'h' || letra ==  'A' || letra ==  'B' || letra ==  'C' || letra ==  'D' || letra ==  'E' || letra ==  'F' || letra ==  'G' || letra ==  'H'  ) ){
			cout << "Letra erronea. Vuelve a introducir Letra de la lista   ";
			cin >> letra;

		}	 
	 cout << "\tIntroduce el tiempo en días: ";
	 cin >> tiempo;

		if(letra == 'a'|| letra == 'A')
		Posicion(0.387,0.206,periodoMercurio)	;
		else if( letra == 'b' || letra == 'B' )
		Posicion(0.723,0.007,periodoVenus);
		else if (letra == 'c' || letra == 'C')
		Posicion(1.00,0.017,periodoTierra);
		else if(letra == 'd' || letra == 'D')
		Posicion(1.524,0.093,periodoMarte);
		else if ( letra == 'e' || letra == 'E')
		Posicion(5.203,0.048,periodoJupiter);
		else if ( letra == 'f' || letra == 'F')
		Posicion (9.546,0.056,periodoSaturno);
		else if ( letra == 'g' || letra == 'G') 
		Posicion (19.20,0.047,periodoUrano);
		else if (letra == 'h' || letra == 'H')
		Posicion(30.09,0.009,periodoNeptuno);
	
 	 calcularMomentoAngular();
 	 calcularEnergia();

 	 cout << "\n\tPosicion: x("<<tiempo << ") = ["<< pos_final[0] <<" , "<< pos_final[1] << " ]" ;
 	 cout << "\n\tLa energia en tiempo " << tiempo << " es : " << energia ;
 	 cout << "\n\tEl momento angular es c("<<tiempo<<"):[ " << momento[0]<<" , " << momento[1]<<" , "<< momento[2]<<" ]";
 	 cout << "\n\tLa excentricidad es : " << u;
 
 	 cout << "\n\n\tQuieres terminar el programa : S o N   ";
 	 cin >> finalizar ; 
	 //Aseguramos que introducimos una tecla correcta
  		while ( !(finalizar == 's' || finalizar == 'S' || finalizar == 'n' || finalizar == 'N')){
  			cout << "\nTecla ERRONEA\n\n\tQuieres terminar el programa : S o N   ";
 			cin >> finalizar ; 

  		}

 	switch(toupper(finalizar)){
		case 'S':
			 cout << "\n";
			exit(0);
			break;
		case 'N':
			cout << "\n";
			break;

	}//switch 

  }//while

}
