**Sistema Solar** Individual
==================
## Programa realizado en c++ y openGl


 ### **Parte 1**

Programa que nos calcula la energia, momento angular y excentricidad de un planeta en un determinado tiempo los cuales introducimos por pantalla.

Compilamos y ejecutamos con :

	g++ nombrearchivo.cpp -o nombreejecutable
    ./nombreejecutable

 ### **Parte 2**

Programa en openGl  que nos dibujará las trayectorias de los planetas alrededor del Sol.

Compilamos y ejecutamos con :

	make

 ### **Final**

Programa que enlaza los ejecutables de parte1.cpp y parte2 para que en todo momento el usuario decida si quiere ejecutar uno u otro. Una vez que el usuario sale de uno de los programas vuelve a preguntar que programa queremos o si queremos salir.

Compilamos y ejecutamos con :

	g++ nombrearchivo.cpp -o nombreejecutable
    ./nombreejecutable

Este programa enlaza de nombre del ejecutable de parte1 a mecanica.cpp y el ejecutable de parte2 llamado mecanica2
