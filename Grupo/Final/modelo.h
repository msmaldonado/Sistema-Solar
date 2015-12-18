
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

class Modelo{

private:
    
	GLfloat rdS,rd1,rd2,rd3,rd4,rd5,rd6,rd7,rd8,rd9; //radio de los planetas
	GLfloat rotA,rot1,rot2,rot3,rot4,rot5,rot6,rot7,rot8,rot9; //como rota con respecto a x,z
    
public:

	Modelo();
	~Modelo();
	void dibuja_Sistema();
	void dibuja_Mercurio();
	void dibuja_Venus();
	void dibuja_Tierra();
	void dibuja_Marte();
	void dibuja_Jupiter();
	void dibuja_Saturno();
	void dibuja_Urano();
	void dibuja_Neptuno();
	void dibuja_Pluton();
	void sol();
	void mercurio();
	void venus();
	void tierra();
	void marte();
	void jupiter();
	void saturno();
	void urano();
	void anillos();
	void neptuno();
	void pluton();
	void estrellas();
	void velocidad1();
	void velocidad2();
	void velocidad3();
	void velocidad4();
};
