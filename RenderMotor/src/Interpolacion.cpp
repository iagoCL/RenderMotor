#include "Interpolacion.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Interpolacion::Interpolacion()
{
	t = 0;
	radio = 5.0f;
	M_PI = 3.141516;
}

Interpolacion::Interpolacion(float FRadio)
{
	t = 0;
	radio = FRadio;
	M_PI = 3.141516;
}


Interpolacion::~Interpolacion()
{
}
void Interpolacion::incrementar(float velocidad) {
	t += velocidad;
	if (t >= 4.0f) t -= 4.0f;
}

glm::vec3 Interpolacion::bezier() {

	/*
	Devuelve el vector necesario para dibujar una circunferencia de radio 8 centada en el 0,0 y que se deplaza por el plano yz mientras que orbita alrededor del eje x
	La circunferencia es una aproximacion (es imposible de interpolar exactamente con una curva de bezier)
	Se divide en 4 tramos, si se usaran más tramos mejoraria la aproximacion
	*/

	/*
	Existen 4 tramos
	Tramo 1 Pasa por 0 y 2; 1 es el cruce de ambas tangentes
	Tramo 2 Pasa por 2 y 4; 3 es el cruce de ambas tangentes
	Tramo 3 Pasa por 4 y 6; 5 es el cruce de ambas tangentes
	Tramo 4 Pasa por 6 y 8; 7 es el cruce de ambas tangentes

	La curva pasa por los puntos 0 2 4 6 8
	Los puntos de control indican la curvutura y son el cruce de tangentes 1 3 5 7
	*/

	static glm::vec3 puntos[9];
	puntos[0] = glm::vec3(0.0f, 0.0f, radio);
	puntos[1] = glm::vec3(0.0f, radio, radio); //punto control - cruce tangentes en 0 y 2
	puntos[2] = glm::vec3(0.0f, radio, 0.0f);

	puntos[3] = glm::vec3(0.0f, radio, -radio); //punto control - cruce tangentes en 2 y 4 
	puntos[4] = glm::vec3(0.0f, 0.0f, -radio);

	puntos[5] = glm::vec3(0.0f, -radio, -radio); //punto control - cruce tangentes en 4 y 6
	puntos[6] = glm::vec3(0.0f, -radio, 0.0f);

	puntos[7] = glm::vec3(0.0f, -radio, radio); //punto control - cruce tangentes en 6 y 8
	puntos[8] = glm::vec3(0.0f, 0.0f, radio); //se repite el ultimo punto para que sea cerrada

	float t2 = t - int(t);
	int t3 = 2 * int(t);

	return pow((1 - t2), 2)*puntos[t3] + 2.0f*(1.0f - t2)*t2*puntos[t3 + 1] + pow(t2, 2)*puntos[t3 + 2];

}

glm::vec3 Interpolacion::splinesCubicos() {

	/*
	Devuelve el vector necesario para dibujar una circunferencia de radio 5 centada en el 0,0 y que se deplaza por el plano xy, orbitando alrededor del eje z
	La circunferencia es una aproximacion (es imposible de interpolar exactamente con un spline cubico
	Se divide en 4 tramos, si se usaran más tramos mejoraria la aproximacion
	*/

	/*
	Existen 4 tramos
	Tramo 1 Pasa por 0 y 1;
	Tramo 2 Pasa por 1 y 2;
	Tramo 3 Pasa por 2 y 3;
	Tramo 4 Pasa por 3 y 4;

	No es posible emplear el (0.0f,5.0f,0.0f) o el (0.0f,-5.0f,0.0f) por ser la tangete / derivada de este punto r/0; lo que causa que en tramos interpolemos la x y en otros interpolemos la y;
	Si no se cogen más 4 puntos equidistantes queda achatada y ligeramente asimetrica  ya que algunos tramos estan mejor interpolados que otros
	*/

	static float puntoxy = radio*cos(M_PI / 4);//al escoger 45,135,215,315 se puede reutilizar este valor para las coordenadas x e y de los puntos
	static glm::vec3 puntos[5];
	puntos[0] = glm::vec3(puntoxy, puntoxy, 0.0f); // primer cuadrante - 45
	puntos[1] = glm::vec3(-puntoxy, puntoxy, 0.0f); //segundo cuadrante - 135
	puntos[2] = glm::vec3(-puntoxy, -puntoxy, 0.0f); //tercer cuadrante - 215
	puntos[3] = glm::vec3(puntoxy, -puntoxy, 0.0f); //cuarto cuadrante - 315
	puntos[4] = glm::vec3(puntoxy, puntoxy, 0.0f); //primer cuadrante - 45, se repite el 0 para que sea cerrada

	static float pendiente = tan(M_PI / 4); //al escoger 45,135,215,315 se puede reutilizar este valor para las pendientes de los puntos
	static float derivada[5];
	derivada[0] = -pendiente; //punto 0 - 45  deg
	derivada[1] = pendiente; //punto 1 - 135  deg
	derivada[2] = -pendiente; //punto 2 - 215 deg
	derivada[3] = pendiente; //punto 3 - 315  deg
	derivada[4] = -pendiente; //punto 4 - 45  deg

	float valory, valorx, valorz;

	float t2 = t - int(t);
	int t3 = int(t);

	if (t3 == 0 || t3 == 2) { //en este tramo se interpola con splines cubicos la y
		float tempa = derivada[t3] * (puntos[t3 + 1].x - puntos[t3].x) - (puntos[t3 + 1].y - puntos[t3].y); //Una posible mejora sería declararlas como array static ya que para los mismos puntos de control/derivadas no varian
		float tempb = (puntos[t3 + 1].y - puntos[t3].y) - derivada[t3 + 1] * (puntos[t3 + 1].x - puntos[t3].x);

		valory = (1 - t2)*puntos[t3].y + t2*puntos[t3 + 1].y + (1 - t2)*t2*(tempa*(1 - t2) + tempb*t2);
		valorx = puntos[t3].x + t2*(puntos[t3 + 1].x - puntos[t3].x);
	}
	else { //en este tramo se interpola con splines cubicos la x
		float tempa = derivada[t3] * (puntos[t3 + 1].y - puntos[t3].y) - (puntos[t3 + 1].x - puntos[t3].x); //Una posible mejora sería declararlas como array static ya que para los mismos puntos de control/derivadas no varian
		float tempb = (puntos[t3 + 1].x - puntos[t3].x) - derivada[t3 + 1] * (puntos[t3 + 1].y - puntos[t3].y);

		valorx = (1 - t2)*puntos[t3].x + t2*puntos[t3 + 1].x + (1 - t2)*t2*(tempa*(1 - t2) + tempb*t2);
		valory = puntos[t3].y + t2*(puntos[t3 + 1].y - puntos[t3].y);
	}

	valorz = puntos[t3].z + t2*(puntos[t3 + 1].z - puntos[t3].z); // como no varía se pondría poner 0.0f

	return glm::vec3(valorx, valory, valorz);
}

