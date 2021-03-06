#include "modelo.h"

/*Constructor de modelo que inicializa las rotaciones*/
Modelo :: Modelo(){
rotA=0,rot1=0,rot2=0,rot3=0,rot4=0,rot5=0,rot6=0,rot7=0,rot8=0,rot9=0; //como rota con respecto a x,z
rdS=30,rd1=6, rd2=12,rd3=9,rd4=6,rd5=18,rd6=15,rd7=12,rd8=12,rd9=3; //radio de los planetas

}

Modelo :: ~Modelo(){
}
/*Método para crear los planetas*/
void Modelo::sol(){
     glColor3f(1.0,1.0,0.0);
     glutSolidSphere(rdS,60,60);
}
void Modelo::mercurio(){
     glColor3f(0.4,0.4,0.4);
     glutSolidSphere(rd1,60,60);
}
void Modelo::venus(){
     glColor3f(0.2,0.4,0.3);
     glutSolidSphere(rd2,60,60);
}
void Modelo::tierra(){
     glColor3f(0.0,0.4,1.0);
     glutSolidSphere(rd3,60,60);
}
void Modelo::marte(){
     glColor3f(1.0,0.0,0.0);
     glutSolidSphere(rd4,60,60);
}
void Modelo::jupiter(){
     glColor3f(0.2,0.2,0.2);
     glutSolidSphere(rd5,60,60);
}
void Modelo::saturno(){
     glColor3f(1.0,0.4,0.0);
     glutSolidSphere(rd6,60,60);
}
void Modelo::urano(){
     glColor3f(0.2,1.0,0.2);
     glutSolidSphere(rd7,60,60);
}
void Modelo::anillos(){
     glColor3f(1,0.6,0);
     glRotatef(rotA,1,0,1);
     glutSolidTorus(3, 20, 10,50);
}
void Modelo::neptuno(){
     glColor3f(0.0,0.0,1.0);
     glutSolidSphere(rd8,60,60);
}
void Modelo::pluton(){
     glColor3f(0.6,0.7,0.5);
     glutSolidSphere(rd9,60,60);
}

void Modelo::estrellas(){
    int i,j,k; srand(time(NULL));
    int h=0;
         glBegin(GL_POINTS);
         while(h<100){
         i=(rand()%300);
         j=(rand()%300);
         k=(rand()%300);
           glColor3f(1,1,1);
           glVertex3f(i,j,k);
           glVertex3f(i,-j,k);
           glColor3f(0,1,0);
           glVertex3f(-i,-j,k);
           glColor3f(0,0,1);
           glVertex3f(-i,j,-k);
           h++;
       }
    glEnd();

}

void Modelo::dibuja_Sistema(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

	estrellas();
	sol();
	
	//rotacion y traslacion para mercurio
	glPushMatrix();     
	glRotatef(rot1,0,1,0);
	glTranslatef(-40,0,0);
	mercurio();
	glPopMatrix();

	//rotacion y traslacion para venus
	glPushMatrix();
	glRotatef(rot2,0,1,0);
	glTranslatef(60,0,0);
	venus();
  glPopMatrix();
	
	//rotacion y tralacion para tierra
	glPushMatrix();
	glRotatef(rot3,0,1,0);
	glTranslatef(0,0,-80);
	tierra();
	glPopMatrix();

	//rotacion y traslacion para marte
	glPushMatrix();
	glRotatef(rot4,0,1,0);
	glTranslatef(100,0,0);
	marte();
	glPopMatrix();

	//rotacion y traslacion para jupiter
	glPushMatrix();
	glRotatef(rot5,0,1,0);
	glTranslatef(-120,0,0);
	jupiter();
	glPopMatrix();

	//rotacion y traslacion para saturno y anillo
	glPushMatrix();
	glRotatef(rot6,0,1,0);
	glTranslatef(0,0,160);
	saturno();
	anillos();
	glPopMatrix();

	//rotacion y traslacion para urano
	glPushMatrix();
	glRotatef(rot7,0,1,0);
	glTranslatef(-170,0,0);
	urano();
	glPopMatrix();

	//rotacion y traslacion para neptuno
	glPushMatrix();
	glRotatef(rot8,0,1,0);
	glTranslatef(195,0,0);
	neptuno();
	glPopMatrix();

	//Rotacion y translacion para Pluton
	glPushMatrix();
	glRotatef(rot9,0,1,0);
	glTranslatef(215,0,0);
	pluton();
	glPopMatrix();

	glutSwapBuffers();
	
}

void Modelo::dibuja_Mercurio(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

	estrellas();
	sol();

	//rotacion y traslacion para mercurio
	glPushMatrix();     
	glRotatef(rot1,0,1,0);
	glTranslatef(-40,0,0);
	mercurio();
	glPopMatrix();
	glutSwapBuffers();
}

void Modelo::dibuja_Venus(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

	estrellas();
	sol();

	//rotacion y traslacion para venus
	glPushMatrix();
	glRotatef(rot2,0,1,0);
	glTranslatef(60,0,0);
	venus();
	glPopMatrix();
	glutSwapBuffers();
}

void Modelo::dibuja_Tierra(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

	estrellas();
	sol();

  //rotacion y tralacion para tierra
	glPushMatrix();
  glRotatef(rot3,0,1,0);
  glTranslatef(0,0,-80);
  tierra();
  glPopMatrix();
  glutSwapBuffers();
}

void Modelo::dibuja_Marte(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  estrellas();
  sol();

	//rotacion y traslacion para marte
  glPushMatrix();
  glRotatef(rot4,0,1,0);
  glTranslatef(100,0,0);
  marte();
  glPopMatrix();
  glutSwapBuffers();
}

void Modelo::dibuja_Jupiter(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  estrellas();
  sol();

  //rotacion y traslacion para jupiter
  glPushMatrix();
  glRotatef(rot5,0,1,0);
  glTranslatef(-120,0,0);
  jupiter();
  glPopMatrix();
  glutSwapBuffers();
}

void Modelo::dibuja_Saturno(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  estrellas();
  sol();

  //rotacion y traslacion para saturno y anillo
  glPushMatrix();
  glRotatef(rot6,0,1,0);
  glTranslatef(0,0,140);
  saturno();
  anillos();
  glPopMatrix();
  glutSwapBuffers();
}

void Modelo::dibuja_Urano(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  estrellas();
  sol();

  //rotacion y traslacion para urano
  glPushMatrix();
  glRotatef(rot7,0,1,0);
  glTranslatef(-170,0,0);
  urano();
  glPopMatrix();
  glutSwapBuffers();
}

void Modelo::dibuja_Neptuno(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  estrellas();
  sol();

  //rotacion y traslacion para neptuno
  glPushMatrix();
  glRotatef(rot8,0,1,0);
  glTranslatef(195,0,0);
  neptuno();
  glPopMatrix();
  glutSwapBuffers();
}

void Modelo::dibuja_Pluton(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  estrellas();
  sol();

  //rotacion y translacion para Pluton
  glPushMatrix();
  glRotatef(rot9,0,1,0);
  glTranslatef(215,0,0);
  pluton();
  glPopMatrix();
  glutSwapBuffers();
}
    
/*Método para los giros de los planetas con distintas velocidades*/

void Modelo::velocidad1(){
  rotA = rotA+4.0*0.03;
  rot1 = rot1+4.0*0.1;
  rot2 = rot2+4.0*0.015;
  rot3 = rot3+4.0*0.013;
  rot4 = rot4+4.0*0.05;
  rot5 = rot5+4.0*0.04;
  rot6 = rot6+4.0*0.03;
  rot7 = rot7+4.0*0.02;
  rot8 = rot8+4.0*0.01;
  rot9 = rot9+4.0*0.023;
}

void Modelo::velocidad2(){
  rotA = rotA+ 8.0*0.03;
  rot1 = rot1+8.0*0.1;
  rot2 = rot2+8.0*0.015;
  rot3 = rot3+8.0*0.013;
  rot4 = rot4+8.0*0.05;
  rot5 = rot5+8.0*0.04;
  rot6 = rot6+8.0*0.03;
  rot7 = rot7+8.0*0.02;
  rot8 = rot8+8.0*0.01;
  rot9 = rot9+8.0*0.023;
}

void Modelo::velocidad3(){
  rotA = rotA+16.0*0.03;
  rot1 = rot1+16.0*0.1;
  rot2 = rot2+16.0*0.015;
  rot3 = rot3+16.0*0.013;
  rot4 = rot4+16.0*0.05;
  rot5 = rot5+16.0*0.04;
  rot6 = rot6+16.0*0.03;
  rot7 = rot7+16.0*0.02;
  rot8 = rot8+16.0*0.01;
  rot9 = rot9+16.0*0.023;
}

void Modelo::velocidad4(){
  rotA = rotA+30.0*0.03;
  rot1 = rot1+30.0*0.1;
  rot2 = rot2+30.0*0.015;
  rot3 = rot3+30.0*0.013;
  rot4 = rot4+30.0*0.05;
  rot5 = rot5+30.0*0.04;
  rot6 = rot6+30.0*0.03;
  rot7 = rot7+30.0*0.02;
  rot8 = rot8+30.0*0.01;
  rot9 = rot9+30.0*0.023;
}

