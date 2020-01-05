#pragma once
class Textura
{
private:
	int numero;
	unsigned int identificador;
	Textura(unsigned int identificador, int numero);

public:
	static int numeroTotal;

	Textura(const char *fileName, bool anisotropico = true);
	~Textura();
	unsigned int loadTex(const char *fileName, bool anisotropico = true);
	void enviarAlShader(unsigned int identificadorUniforme);
	void aplicarAnisotropico();
	Textura* clone();
};