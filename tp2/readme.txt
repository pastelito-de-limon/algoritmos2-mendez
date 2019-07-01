	__/\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\______/\\\\\\\\\_____
	 _\///////\\\/////__\/\\\/////////\\\__/\\\///////\\\___
	  _______\/\\\_______\/\\\_______\/\\\_\///______\//\\\__
	   _______\/\\\_______\/\\\\\\\\\\\\\/____________/\\\/___
	    _______\/\\\_______\/\\\/////////___________/\\\//_____
	     _______\/\\\_______\/\\\_________________/\\\//________
	      _______\/\\\_______\/\\\_______________/\\\/___________
	       _______\/\\\_______\/\\\______________/\\\\\\\\\\\\\\\_
	        _______\///________\///______________\///////////////__
			
Alumna:

+-+-+-+-+-+-+-+ +-+-+-+-+-+-+
|M|I|C|A|E|L|A| |J|U|A|R|E|Z|
+-+-+-+-+-+-+-+ +-+-+-+-+-+-+


Cátedra Méndez.

TP2 "La venganza de Arya" -- Programa de interacción con el usuario por consola.
Juego basado en la serie televisiva de HBO "Game of Thrones".

Argumento: Arya tiene una lista de personas a quien matar por venganza, y recorre muchas ciudades de a una a la vez tratando de matarlas. Deberá tratar de no errar ejecutando inocentes, pero pudiendo eliminar también a personas culpables con las que no tiene una vendetta personal.

Caso de prueba de ejemplo para ganar:
a i s a i s a i s i s i s i s a a i s a a i s

Casos de prueba de ejemplo para perder:
a a 
a i s a a
a i s a i s a a
a i s a i s a i s i s a i s a a i s a a 
f f f f f

Compilación:
gcc *.c -Wall -Werror -Wconversion -std=c99 -o la_venganza_de_arya

Ejecución:
./la_venganza_de_arya