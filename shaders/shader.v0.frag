#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;

uniform vec3 Il1;
uniform vec3 Posl1; 

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 1700.0;
vec3 Ke;

//Propiedades de la luz
vec3 Ia = vec3 (0.05);

vec3 Id = Il1;
vec3 Is = Il1;


vec3 shade();

void main()
{
	Ka = color;
	Kd = color;
	Ke = vec3(0.0);
	Ks = vec3 (1.0);

	N = normalize (norm);
	
	outColor = vec4(shade(), 1.0);   
}

vec3 shade()
{
	vec3 c = vec3(0.0);
	c = Ia * Ka;

	vec3 L = normalize (Posl1 - pos);
	vec3 diffuse = Id * Kd * dot (L,N);
	c += clamp(diffuse, 0.0, 1.0);
	
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	vec3 specular = Is*Ks*pow(factor,alpha);
	c += clamp(specular, 0.0, 1.0);

	c+=Ke;
	
	return c;
}
