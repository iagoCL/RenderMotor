#version 330 core

in vec3 inPos;	
in vec3 inNormal;
in vec2 inTexCoord;	
in vec3 inTangent;

uniform mat4 modelViewProj;
uniform mat4 view;
uniform mat4 normal;
uniform mat4 modelView;

out vec3 VERT;
out vec2 texCoord;

out vec3 direccionLuz3;
out vec3 LVectorl1;
out vec3 direccionLuz1; 
out vec3 LVectorl2;

//propiedades de la luz
//direccional
uniform vec3 Dirl3;// = vec3(0.0,-1.0,-1.0);

//focal
uniform vec3 Posl1;//= vec3(-3.0,4.0,10.0);
uniform vec3 Dirl1;//= vec3(+3.0,-4.0,-10.0);

//luz puntual
uniform vec3 Posl2;// = vec3(3.0,0.0,3.0); //En coordenadas del mundo. Poco habitual, en general se usan coordenadas de la camara

//Focal 1
//Puntual 2
//Dir 3

void main()
{
	vec3 NORM=(normal*vec4(inNormal, 0)).xyz;
	VERT=(modelView*vec4(inPos, 1)).xyz;
	vec3 TANG=(modelView * vec4(inTangent, 0)).xyz;
	

	vec3 biTangent = normalize(cross(NORM,TANG));//en realidad no es necesario normalizar ya que son unitarios

	mat3 cambioDeBase = transpose(mat3(TANG,biTangent,NORM));

	texCoord=inTexCoord;

	
	//normalizamos todos los vectores en el shader de fragmentos 
	//luz direccional
	direccionLuz3=Dirl3;

	//luz Focal
	LVectorl1=cambioDeBase*(Posl1-VERT);
	direccionLuz1 = cambioDeBase*Dirl1;

	//luz puntual
	LVectorl2=cambioDeBase*(Posl2-VERT);

	VERT=cambioDeBase*VERT;

	gl_Position =  modelViewProj * vec4 (inPos,1.0);

}