#include <cctype>   // toupper
#include <string>   // std::string
#include <iostream> // std::cout
#include <fstream>  // ifstream
#include <cmath>    // fabs
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

// includes de OpenGL/glut

#include <GL/glut.h>  // freeglut standard (incluye cabecera opengl)
#include <GL/freeglut_ext.h> // extensiones freeglut (glutSolidCylinder)

// includes de archivos en el directorio de trabajo (de las prácticas)

#include "error-ogl.hpp"
#include "modelo.h"
using namespace std;

// *********************************************************************
// **
// ** Variables globales
// ** (se inicializan en las funciones de inicialización)
// **
// *********************************************************************

// variables que definen la posicion de la camara en coordenadas polares

float
   camara_angulo_x ,   // angulo de rotación entorno al eje X
   camara_angulo_y ;   // angulo de rotación entorno al eje Y

// ---------------------------------------------------------------------
// variables que definen el view-frustum (zona visible del mundo)

float
   frustum_factor_escala ,  // factor de escala homogeneo que se aplica al frustum (sirve para alejar o acercar)
   frustum_ancho ,          // ancho, en coordenadas del mundo
   frustum_dis_del ,        // distancia al plano de recorte delantero (near)
   frustum_dis_tra ;        // distancia al plano de recorte trasero (far)

// ---------------------------------------------------------------------
// variables que determinan la posicion y tamaño inicial de la ventana
// (el tamaño se actualiza al cambiar el tamaño durante la ejecución)

int
	ventana_pos_x  ,  // posicion (X) inicial de la ventana, en pixels
  ventana_pos_y  ,  // posicion (Y) inicial de la ventana, en pixels
  ventana_tam_x  ,  // ancho inicial y actual de la ventana, en pixels
  ventana_tam_y  ;  // alto inicial actual de la ventana, en pixels

Modelo SistemaSolar;
char letra,letra2;

double pos_final [3],pos_final_derivadas [3],momento [3], pos_rotacion[2], matriz_rotacion [2][2];
double tiempo,energia,phi,u,a,periodo,si,w, Glepsilon;

/*Masa de los planetas por si queremos calcular la energia usando la fórmula en la 	que aparece la masa*/
/*double masaMercurio = 3.3 *pow(10,23);
double masaVenus = 48.7 *pow(10,23);
double masaTierra = 59.7 *pow(10,23);
double masaMarte = 6.4 *pow(10,23);
double masaJupiter = 19000 *pow(10,23);
double masaSaturno = 5688 *pow(10,23);
double masaUrano = 868.6 *pow(10,23);
double masaNeptuno = 1024 *pow(10,23);*/

/*...FUNCIONES UTILIZADAS PARA REALIZAR LOS CALCULOS...*/

/*Método para calcular u que usará el método de Newton Raphson a partir de una phi*/
void  calcularExcentricidad(double epsilon){
  si = (2*M_PI*tiempo)/periodo ;
	double u_antigua = M_PI;
	phi = (epsilon*(sin(u_antigua)-u_antigua*cos(u_antigua))+si)/(1.0-epsilon*cos(u_antigua));
	//Metodo de Newton-Raphson
	while (abs(u_antigua-phi) >= 0.00000001){
		u_antigua = phi;
		phi = (epsilon*(sin(phi)-phi*cos(phi))+si)/(1.0-epsilon*cos(phi));
	}
	u = phi;
}

void Posicion(double epsilon){
	//Actualizamos las variables para usarlas en energia sin tener que pasar parámetros
  Glepsilon = epsilon;
	pos_final[0] = a*(cos(u)-epsilon);
	pos_final[1] = a*sqrt(1-(epsilon*epsilon))*sin(u) ;//sqrt(1-epsilon²) * sin(u) ;
	//declaramos una variable ayuda para hacer la derivada por cambio
	double ayuda = (2*M_PI*sqrt(pow(a,1/3))/periodo)/(sqrt(pow(a,1/3))*(1-epsilon*cos(u)));
	pos_final_derivadas[0] = (-a*sin(u)*ayuda);
	pos_final_derivadas[1] = sqrt(1.0-(epsilon*epsilon))*cos(u)*ayuda*a;
	//Completamos la 3 coordenada de ambos vectores para hacer el producto vectorial
	pos_final[2] = 0;
	pos_final_derivadas[2] = 0;
}

/*Método para calcular la energia de un planeta en un tiempo. Varibles a y periodo actualizadas cuando calcula la posición*/
void calcularEnergia(){
	energia = ((pos_final_derivadas[0]*pos_final_derivadas[0]+pos_final_derivadas[1]*pos_final_derivadas[1])/2.0 )-(4*M_PI*M_PI*a*a*a/(periodo*periodo))/(sqrt(pos_final[0]*pos_final[0]+pos_final[1]*pos_final[1]));
}

/*Método que nos calcula el momento angular es decir el producto vectorial de posición y velocidad, siempre en tres coordenadas*/
void calcularMomentoAngular(){
	momento[0] = pos_final[1]*pos_final_derivadas[2]-pos_final[2]*pos_final_derivadas[1];
	momento[1] = pos_final[2]*pos_final_derivadas[0]-pos_final[0]*pos_final_derivadas[2];
	momento[2] = pos_final[0]*pos_final_derivadas[1]-pos_final[1]*pos_final_derivadas[0];
}

void calcularTiempo( double epsilon){
    tiempo = (periodo * (u - epsilon * sin(u)))/(2*M_PI);
}

void PosicionMatriz (){
  matriz_rotacion[0][0] = cos(w);
  matriz_rotacion[0][1] = -sin(w);
  matriz_rotacion[1][0] = sin(w);
  matriz_rotacion[1][1] = cos(w);

  pos_rotacion[0] = matriz_rotacion[0][0] * pos_final[0] + matriz_rotacion[0][1] * pos_final[1] ;
  pos_rotacion[1] = matriz_rotacion[1][0] * pos_final[0] + matriz_rotacion[1][1] * pos_final[1] ;
}

/*...FUNCIONES ESPECIFICAS DE OPENGL...*/

void FijarProyeccion(){
   const GLfloat ratioYX = GLfloat( ventana_tam_y )/GLfloat( ventana_tam_x );

   //CError();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // (3) proyectar en el plano de visión
   glFrustum
   (  -frustum_ancho,
      +frustum_ancho,
      -frustum_ancho*ratioYX,
      +frustum_ancho*ratioYX,
      +frustum_dis_del,
      +frustum_dis_tra
   );


   // (2) situar el origen (0,0,0), en mitad del view frustum
   //     (en la rama negativa del eje Z)
   glTranslatef( 0.0,0.0,-0.5*(frustum_dis_del+frustum_dis_tra));

    // (1) aplicar factor de escala
   glScalef( frustum_factor_escala, frustum_factor_escala,  frustum_factor_escala );

   //CError();
}


// ---------------------------------------------------------------------
// fijar viewport y proyección (viewport ocupa toda la ventana)

void FijarViewportProyeccion(){
   glViewport( 0, 0, ventana_tam_x, ventana_tam_y );
   FijarProyeccion() ;
}

// ---------------------------------------------------------------------
// fija la transformación de vista (posiciona la camara)

void FijarCamara(){

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glRotatef(camara_angulo_x,1,0,0);
   glRotatef(camara_angulo_y,0,1,0);

}

// ---------------------------------------------------------------------
// dibuja los ejes utilizando la primitiva grafica de lineas

void DibujarEjes(){
   const float long_ejes = 30.0 ;

   // establecer modo de dibujo a lineas (podría estar en puntos)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

   // dibujar tres segmentos
   glBegin(GL_LINES);
      // eje X, color rojo
      glColor3f( 1.0, 0.0, 0.0 );
      glVertex3f( -long_ejes, 0.0, 0.0 );
      glVertex3f( +long_ejes, 0.0, 0.0 );
      // eje Y, color verde
      glColor3f( 0.0, 1.0, 0.0 );
      glVertex3f( 0.0, -long_ejes, 0.0 );
      glVertex3f( 0.0, +long_ejes, 0.0 );
      // eje Z, color azul
      glColor3f( 0.0, 0.0, 1.0 );
      glVertex3f( 0.0, 0.0, -long_ejes );
      glVertex3f( 0.0, 0.0, +long_ejes );
   glEnd();

   // bola en el origen, negra
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glColor3f(0.0,0.0,0.0);
   glutSolidSphere(0.01,8,8);

}

// ---------------------------------------------------------------------
// asigna el color de fondo actual a todos los pixels de la ventana

void LimpiarVentana(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// ---------------------------------------------------------------------
// Dibuja los objetos de la escena

void DibujarObjetos(){

		if(toupper(letra) == 'M')
      SistemaSolar.dibuja_Mercurio() ;
		else if(toupper(letra) == 'V')
			SistemaSolar.dibuja_Venus();
		else if(toupper(letra) == 'T')
			SistemaSolar.dibuja_Tierra();
		else if(toupper(letra) == 'A')
			SistemaSolar.dibuja_Marte();
		else if(toupper(letra) == 'J')
			SistemaSolar.dibuja_Jupiter();
		else if(toupper(letra) == 'S')
			SistemaSolar.dibuja_Saturno();
		else if(toupper(letra) == 'U')
			SistemaSolar.dibuja_Urano();
		else if(toupper(letra) == 'N')
			SistemaSolar.dibuja_Neptuno();
    else if(toupper(letra) == 'P')
      SistemaSolar.dibuja_Pluton();
		else if(toupper(letra) == 'I')
			SistemaSolar.dibuja_Sistema();
}


// *********************************************************************
// **
// ** Funciones gestoras de eventos
// **
// *********************************************************************

// F.G. del evento de redibujado (se produce cuando hay que volver a
// dibujar la ventana, tipicamente tras producirse otros eventos)

void FGE_Redibujado(){
   using namespace std ;
   FijarViewportProyeccion() ; // necesario pues la escala puede cambiar
   FijarCamara();
   LimpiarVentana();
   DibujarEjes() ;
   DibujarObjetos();
   glutSwapBuffers();
}

// ---------------------------------------------------------------------
// F.G. del evento de cambio de tamaño de la ventana

void FGE_CambioTamano( int nuevoAncho, int nuevoAlto ){
   // guardar nuevo tamaño de la ventana
   ventana_tam_x  = nuevoAncho;
   ventana_tam_y  = nuevoAlto ;

   // reestablecer frustum, viewport y proyección
   FijarViewportProyeccion();

   // forzar un nuevo evento de redibujado, para actualizar ventana
   glutPostRedisplay();
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla normal
//
// parámetros:
//       tecla: carácter corresondiente a la tecla (minúscula)
//       x_raton, y_raton : posición del ratón al pulsar

void FGE_PulsarTeclaNormal( unsigned char tecla, int x_raton, int y_raton ){
	bool redisp = true ;
	switch (toupper(tecla))  {
	  case '1' :
		  SistemaSolar.velocidad1();
	    break;
	  case '2' :
	    SistemaSolar.velocidad2();
	    break;
	  case '3' :
      SistemaSolar.velocidad3();
      break;
	  case '4' :
      SistemaSolar.velocidad4();
      break;
		case 'M' :
      letra = 'm';
      break;
		case 'V' :
      letra = 'v';
      break;
		case 'T' :
      letra = 't';
      break;
		case 'A' :
      letra = 'a';
      break;
		case 'J' :
      letra = 'j';
		 	break;
		case 'S' :
      letra = 's';
      break;
		case 'U' :
      letra = 'u';
      break;
		case 'N' :
      letra = 'n';
      break;
    case 'P' :
      letra = 'p';
      break;
		case 'I' :
      letra = 'i';
      break;
		case 'C' :
      letra = 'c';
      glutLeaveMainLoop();
      break;
    case 'Q' :
      exit( 0 );
      break ;
		case '+' :
      frustum_factor_escala *= 1.6;
      break;
    case '-' :
      frustum_factor_escala /= 1.6;
      break;
    default:
    	redisp = false ;
      break ;
	}
  using namespace std ;

  // si se ha cambiado algo, forzar evento de redibujado
  if (redisp)
  	glutPostRedisplay();
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla especial
//
// parámetros:
//       tecla: código GLUT de la tecla pulsada
//       x_raton, y_raton : posición del ratón al pulsar


void FGE_PulsarTeclaEspecial( int tecla, int x_raton, int y_raton ){

	bool redisp = true ;
  const float da = 5.0 ; // incremento en grados de ángulos de camara

  switch ( tecla )
  {
     case GLUT_KEY_LEFT:
        camara_angulo_y = camara_angulo_y - da ;
        break;
     case GLUT_KEY_RIGHT:
        camara_angulo_y = camara_angulo_y + da ;
        break;
     case GLUT_KEY_UP:
        camara_angulo_x = camara_angulo_x - da ;
        break;
     case GLUT_KEY_DOWN:
        camara_angulo_x = camara_angulo_x + da ;
        break;
     case GLUT_KEY_PAGE_UP:
        frustum_factor_escala *= 1.05;
        break;
     case GLUT_KEY_PAGE_DOWN:
        frustum_factor_escala /= 1.05;
        break;
     default:
        redisp = false ;
        break ;
	}
  using namespace std ;

  // si se ha cambiado algo, forzar evento de redibujado
  if ( redisp )
     glutPostRedisplay();
}

// *********************************************************************
// **
// ** Funciones de inicialización
// **
// *********************************************************************

// inicialización de GLUT: creación de la ventana, designar FGEs

void Inicializa_GLUT( int argc, char * argv[] ){

   // inicializa variables globales usadas en esta función (y otras)
   ventana_pos_x  = 0 ;
   ventana_pos_y  = 0  ;
   ventana_tam_x  = 1024 ;  // ancho inicial y actual de la ventana, en pixels
   ventana_tam_y  = 800 ;  // alto inicial actual de la ventana, en pixels

   // inicializa glut:
   glutInit( &argc, argv );

   // establece posicion inicial de la ventana:
   glutInitWindowPosition( ventana_pos_x, ventana_pos_y );

   // establece tamaño inicial de la ventana:
   glutInitWindowSize( ventana_tam_x, ventana_tam_y );

   // establece atributos o tipo de ventana:
   glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

   // crea y visualiza una ventana:
   glutCreateWindow("Sistema Solar");

   // establece función gestora del evento de redibujado:
   glutDisplayFunc( FGE_Redibujado );

   // establece función gestora del evento de cambio de tamaño:
   glutReshapeFunc( FGE_CambioTamano );

   // establece función gestora del evento de pulsación de tecla normal:
   glutKeyboardFunc( FGE_PulsarTeclaNormal );

   // establece función gestora del evento de pulsación de tecla especial:
   glutSpecialFunc( FGE_PulsarTeclaEspecial );

   // Let the loop finish when glutLeaveMainLoop() is called
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

// ---------------------------------------------------------------------
// inicialización de OpenGL

void Inicializa_OpenGL( ){
   CError();

   glEnable( GL_DEPTH_TEST );

   // establecer color de fondo: (1,1,1) (blanco)
   glClearColor( 1.0, 1.0, 1.0, 1.0 ) ;

   // establecer color inicial para todas las primitivas
   glColor3f( 0.7, 0.2, 0.4 ) ;

   // establecer ancho de línea
   glLineWidth( 1.0 );

   // establecer tamaño de los puntos
   glPointSize( 2.0 );

   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

   // inicializar parámetros del frustum
   frustum_dis_del         = 0.1 ;
   frustum_dis_tra         = 10.0;
   frustum_ancho           = 0.5*frustum_dis_del ;
   frustum_factor_escala   = 2.0 ;

   // inicializar parámetros de la cámara
   camara_angulo_x = 0.0 ;
   camara_angulo_y = 0.0 ;

   // establecer la cámara, la proyección y el viewport
   FijarViewportProyeccion() ;
   FijarCamara() ;

   CError();
}

// ---------------------------------------------------------------------
// Código de inicialización (llama a los dos anteriores, entre otros)

void Inicializar( int argc, char *argv[] ){

   Inicializa_GLUT( argc, argv ) ;
   Inicializa_OpenGL() ;
   SistemaSolar.dibuja_Mercurio();
}


// *********************************************************************
// **
// ** Función principal
// **
// *********************************************************************

int main( int argc, char *argv[] ){
  while (true){
    sleep (1);

		cout << "\n\tIntroduce un planeta de la lista pulsando la letra correspondiente \n" << "\n\tMercurio --> M\n" << "\tVenus --> V\n" << "\tTierra --> T\n" << "\tMarte --> A\n" << "\tJupiter --> J\n" << "\tSaturno --> S\n"<<  "\tUrano --> U\n" << "\tNeptuno --> N\n";
 	 	cout << "\n\tSu eleccion es: ";
		cin >> letra;

 	 //Comprobamos que se ha introducido una letra de la lista
 		while (!(toupper(letra) == 'M' || toupper(letra) == 'V' || toupper(letra) == 'T' || toupper(letra) == 'A' || toupper(letra) == 'J' || toupper(letra) == 'J' || toupper(letra) == 'S' || toupper(letra) == 'U' || toupper(letra) == 'N')){
			cout << "Letra erronea. Vuelve a introducir una letra de la lista   ";
			cin >> letra;
		}

    cout << "\n\tIntroduce \n\tA --> si quieres calcular la anomalia excentrica a partir del tiempo \n\tT --> si quieres calcular el tiempo a partir de la anomalia excentrica \n";
    cout << "\n\tSu eleccion es: ";
    cin >> letra2;

    while (!(toupper(letra2) == 'A' ||  toupper(letra2) == 'T')){
      cout << "Letra erronea. Vuelve a introducir una letra de la lista   ";
      cin >> letra2;
    }

    if (toupper(letra2)=='A'){
	 	   cout << "\tIntroduce el tiempo en días: ";
	 	   cin >> tiempo;

  		if(toupper(letra) == 'M'){
        periodo = 87.97;
        a = 0.387;
        w = 75.9 ;
  			cout << "\n\t ___Para Mercurio___ : \n";
        calcularExcentricidad(0.206);
  			Posicion(0.206);
  		}
  		else if(toupper(letra) == 'V'){
        periodo = 224.7;
        a = 0.723;
        w = 130.15;
  			cout << "\n\t ___Para Venus___ : \n";
        calcularExcentricidad(0.007);
  			Posicion(0.007);
  		}
  		else if (toupper(letra) == 'T'){
        periodo = 365.26;
        a = 1.00;
        w = 101.22;
  			cout << "\n\t ___Para La Tierra___ : \n";
        calcularExcentricidad(0.017);
  			Posicion(0.017);
  		}
  		else if(toupper(letra) == 'A'){
        periodo = 686.98;
        a = 1.524;
        w = 334.22;
  			cout << "\n\t ___Para Marte___ : \n";
        calcularExcentricidad(0.093);
  			Posicion(0.093);
  		}
  		else if (toupper(letra) == 'J'){
        periodo = 4332.6;
        a = 5.203;
        w = 12.72;
  			cout << "\n\t ___Para Jupiter___ : \n";
        calcularExcentricidad(0.048);
  			Posicion(0.048);
  		}
  		else if (toupper(letra) == 'S'){
        periodo = 10759;
        a = 9.546;
        w = 91.09;
  			cout << "\n\t ___Para Saturno___ : \n";
        calcularExcentricidad(0.056);
  			Posicion (0.056);
  		}
  		else if (toupper(letra) == 'U'){
        periodo = 30687;
        a = 19.20;
        w = 169.05;
  			cout << "\n\t ___Para Urano___ : \n";
        calcularExcentricidad(0.047);
  			Posicion (0.047);
  		}
  		else if (toupper(letra) == 'N'){
        periodo = 60784;
        a = 30.09;
        w = 43.83;
  			cout << "\n\t ___Para Neptuno___ : \n";
        calcularExcentricidad(0.009);
  			Posicion(0.009);
  		}

   	 	calcularMomentoAngular();
   	 	calcularEnergia();
  		PosicionMatriz();

  		cout << "\n\tLa posicion es: x("<<tiempo << ") = ["<< pos_final[0] <<" , "<< pos_final[1] << " ]" ;
   	 	cout << "\n\tLa energia es : " << energia ;
   	 	cout << "\n\tEl momento angular es c("<<tiempo<<"):[ " << momento[0]<<" , " << momento[1]<<" , "<< momento[2]<<" ]";
   	 	cout << "\n\tLa anomalia excentrica es : " << u;
      cout << "\n\tLa posicion con rotación es : x("<<tiempo << ") = ["<< pos_rotacion[0] <<" , "<< pos_rotacion[1] << " ]" ;
      cout << "\n\tEl periodo es : " << periodo;
      cout << "\n\tEl semieje mayor es : " << a;
      cout << "\n\tEpsilon : " << Glepsilon;

    } else if (toupper(letra2)=='T'){

        cout << "\tIntroduce la anomalia excentrica: ";
        cin >> u;

        if(toupper(letra) == 'M'){
          periodo = 87.97;
          a = 0.387;
          w = 75.9;
          cout << "\n\t ___Para Mercurio___ : \n";
          calcularTiempo(0.206);
          Posicion(0.206);
        }
        else if(toupper(letra) == 'V'){
          periodo = 224.7;
          a = 0.723;
          w = 130.5;
          cout << "\n\t ___Para Venus___ : \n";
          calcularTiempo(0.007);
          Posicion(0.007);
        }
        else if (toupper(letra) == 'T'){
          periodo = 365.26;
          a = 1.00;
          w = 101.22;
          cout << "\n\t ___Para La Tierra___ : \n";
          calcularTiempo(0.017);
          Posicion(0.017);
        }
        else if(toupper(letra) == 'A'){
          periodo = 686.98;
          a = 1.524;
          w = 334.22;
          cout << "\n\t ___Para Marte___ : \n";
          calcularTiempo(0.093);
          Posicion(0.093);
        }
        else if (toupper(letra) == 'J'){
          periodo = 4332.6;
          a = 5.203;
          w = 12.72;
          cout << "\n\t ___Para Jupiter___ : \n";
          calcularTiempo(0.048);
          Posicion(0.048);
        }
        else if (toupper(letra) == 'S'){
          periodo = 10759;
          a = 9.546;
          w = 91.09;
          cout << "\n\t ___Para Saturno___ : \n";
          calcularTiempo (0.056);
          Posicion(0.056);
        }
        else if (toupper(letra) == 'U'){
          periodo = 30687;
          a = 19.20;
          w = 169.05;
          cout << "\n\t ___Para Urano___ : \n";
         calcularTiempo (0.047);
         Posicion(0.047);
        }
        else if (toupper(letra) == 'N'){
          periodo = 60784;
          a = 30.09;
          w = 43.83;
          cout << "\n\t ___Para Neptuno___ : \n";
          calcularTiempo (0.009);
          Posicion(0.009);
        }

        calcularMomentoAngular();
        calcularEnergia();
        PosicionMatriz();

        cout << "\n\tEl tiempo es : " << tiempo;
        cout << "\n\tLa posicion es: x("<<tiempo << ") = ["<< pos_final[0] <<" , "<< pos_final[1] << " ]" ;
        cout << "\n\tLa energia es : " << energia ;
        cout << "\n\tEl momento angular es c("<<tiempo<<"):[ " << momento[0]<<" , " << momento[1]<<" , "<< momento[2]<<" ]";
        cout << "\n\tLa posicion con rotación es : x("<<tiempo << ") = ["<< pos_rotacion[0] <<" , "<< pos_rotacion[1] << " ]" ;
        cout << "\n\tEl periodo es : " << periodo;
        cout << "\n\tEl semieje mayor es : " << a;
        cout << "\n\tEpsilon : " << Glepsilon;

      }

  		cout << "\n\n\n\n\t...A continuacion, dibujamos el planeta\n";

  		sleep(3);

     	cout << "\n\t\tControles:\n\tC para salir del dibujo y continuar con el programa \n\tQ para terminar el programa \n\t+/- para ampliar y reducir \n\t1-4 velocidades de giro\n\tflechas para cambiar angulo de visión\n"<< "\n\tSi desea visualizar la rotacion de otro planeta pulse: \n" << "\n\tMercurio --> M\n" << "\tVenus --> V\n" << "\tTierra --> T\n" << "\tMarte --> M\n" << "\tJupiter --> J\n" << "\tSaturno --> S\n"<<  "\tUrano --> U\n" << "\tNeptuno --> N \n  " << "\tPluton --> P \n  " << "\tTodos --> I \n  ";

     	Inicializar(argc,argv) ;
  		glutMainLoop();
  }

   return 0;
}
