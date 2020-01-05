#version 330 core

in vec3 NORM;
in vec3 VERT;
in vec2 texCoord;

out vec4 outColor;

//Prop. luz
//Prop. luz global
vec3 Ia = vec3(0.1);

//Prop. luz focal
uniform vec3 Il1; //= vec3(0.0,0.0,4.0);//azul
uniform vec3 Posl1; //= vec3(-3.0,3.0,3.0); //En coordenadas del mundo. Poco habitual, en general se usan coordenadas de la camara
uniform vec3 Dirl1; //= normalize(-Posl1); //hacemos que apunte al cubo
uniform float Anglel1;// = 0.18f;

//Prop. luz puntual
uniform vec3 Il2; //= vec3(1.5,0.0,0.0);//rojo
uniform vec3 Posl2; //= vec3(3.0,0.0,3.0); //En coordenadas del mundo. Poco habitual, en general se usan coordenadas de la camara

//Prop. luz direccional
uniform vec3 Il3; //= vec3(0.0,8.0,0.0);//verde
uniform vec3 Dirl3; //= vec3(0.0,0.1,0.0); //En coordenadas del mundo. Poco habitual, en general se usan coordenadas de la camara

uniform sampler2D colorTex; 
uniform sampler2D emiTex;
uniform sampler2D specularTex;

//Prop. Obj.
vec3 ka;
vec3 kd;
vec3 ks;
float n; //glossiness
vec3 N;
vec3 Pos;
vec3 ke;


vec3 shadePuntual (vec3 Il, vec3 Posl){
	//difusa
	vec3 L = normalize(Posl - Pos);
	float dist = length(Posl - Pos);
	float decay = min((1/(1 + 0.1*dist + 0.03*pow(dist,2))), 1);

	vec3 Id = Il*kd*dot(L,N);
	
	//specular
	vec3 V=normalize(-Pos);
	vec3 R=normalize(reflect(-L,N));
	float sfactor = max(dot(R,V),0.0001);
	sfactor = pow (sfactor, n);
	vec3 Is = Il*ks*sfactor;
	return (clamp(Id,0,1) + clamp (Is,0,1))*decay;

}


vec3 shadeFocal (vec3 Il, vec3 Posl,vec3 dir, float angleAp){
	//difusa
	vec3 L = normalize(Posl - Pos);
	float dist = length(Posl - Pos);
	float decay = min((1/(1 + 0.1*dist + 0.03*pow(dist,2))), 1);
	vec3 Id = Il*kd*dot(L,N);
	
	//specular
	vec3 V=normalize(-Pos);
	vec3 R=normalize(reflect(-L,N));
	float sfactor = max(dot(R,V),0.0001);
	sfactor = pow (sfactor, n);
	vec3 Is = Il*ks*sfactor;
	
	//atenuacionfocal
	float cosDL=dot(dir,-L) - cos(angleAp);
	if(cosDL>0)		
		return (clamp(Id,0,1) + clamp (Is,0,1))*decay* pow(cosDL/(1-cos(angleAp)),0.9);
	else
		return vec3(0);

}


vec3 shadeDireccional (vec3 Il, vec3 dir){
	//difusa
	vec3 L = -dir;
	vec3 Id = Il*kd*dot(L,N);
	
	//specular
	vec3 V=normalize(-Pos);
	vec3 R=normalize(reflect(-L,N));
	float sfactor = max(dot(R,V),0.0001);
	sfactor = pow (sfactor, n);
	vec3 Is = Il*ks*sfactor;

	return clamp(Id,0,1) + clamp (Is,0,1);
}


vec3 shade(){
	vec3 c = vec3(0.0);
	
	//ambiental
	c+=Ia*ka;

	c+=shadeFocal(Il1,Posl1,Dirl1,Anglel1);
	c+=shadePuntual(Il2,Posl2);
	c+=shadeDireccional(Il3,Dirl3);

	//emisiva
	c+= ke;

	return c;

}


void main()
{
	//pasamos las coordenadas al sistema de referencia de la camara (estaban expresadas en el mundo).

	//ka=color;
	//kd=color;
	ka=texture(colorTex,texCoord).xyz;
	kd=ka;
	ks=texture(specularTex,texCoord).xyz;
	n=100;
	ke=texture(emiTex,texCoord).xyz;

	N=normalize(NORM);
	Pos=VERT;

	outColor = vec4(shade(),0);   

}
