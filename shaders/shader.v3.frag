#version 330 core

uniform sampler2D colorTex; 
uniform sampler2D specularTex;
uniform sampler2D emiTex;
uniform sampler2D normalTex;


uniform float Anglel1;// = 0.08f;
uniform vec3 Il1;// = vec3(0.0,0.0,8.0);
uniform vec3 Il2;// = vec3(1.5);
uniform vec3 Il3;// = vec3(0.3);
 

in vec3 LVectorl1 ;//= vec3(-3.0,3.0,3.0); //En coordenadas del mundo. Poco habitual, en general se usan coordenadas de la camara
in vec3 direccionLuz1;// =normalize (-Posl1); //hacemos que apunte al cubo
in vec3 LVectorl2;
in vec3 direccionLuz3;

in vec3 VERT;
in vec2 texCoord;

out vec4 outColor;

//Prop. luz
//Prop. luz global
vec3 Ia = vec3(0.1);


//Prop. niebla
vec3 fogColor = vec3(0.5);
float fogF =0.002;


//Prop. Obj.
vec3 ka;
vec3 kd;
vec3 ks;
float n; //glossiness
vec3 NText;
vec3 Pos;
vec3 ke;

in vec3 PosText;


vec3 shadePuntual (vec3 Il, vec3 Li){
	//difusa
	vec3 L = normalize(Li);
	float dist = length(Li);
	float decay = min((1/(1 + 0.1*dist + 0.03*pow(dist,2))), 1);

	vec3 Id = Il*kd*dot(L,NText);
	
	//specular	
	vec3 V=normalize(-Pos);
	vec3 R=normalize(reflect(-L,NText));
	float sfactor = max(dot(R,V),0.0001);
	sfactor = pow (sfactor, n);
	vec3 Is = Il*ks*sfactor;
	return (clamp(Id,0,1)+ clamp (Is,0,1))*decay;

}


vec3 shadeFocal (vec3 Il, vec3 L,vec3 dir, float angleAp){
	//difusa
	vec3 L1 = normalize(L);
	float dist = length(L);
	float decay = min((1/(1 + 0.1*dist + 0.03*pow(dist,2))), 1);
	vec3 Id = Il*kd*dot(L,NText);
	
	//specular
	vec3 V=normalize(-Pos);
	vec3 R=normalize(reflect(-L1,NText));
	float sfactor = max(dot(R,V),0.01);
	sfactor = pow (sfactor, n);
	vec3 Is = Il*ks*sfactor;

	//atenuacionfocal
	float cosDL=dot(dir,-L1) - cos(angleAp);
	if(cosDL>0)		
		return (clamp(Id,0,1) + clamp (Is,0,1))*decay* pow(cosDL/(1-cos(angleAp)),0.9);
	else
		return vec3(0);

}



vec3 shadeDireccion(vec3 Il, vec3 dir){
	//difusa

	vec3 L = -dir;
	vec3 Id = Il*kd*dot(L,NText);
	
	//specular
	vec3 V=normalize(-Pos);
	vec3 R=normalize(reflect(-L,NText));
	float sfactor = (dot(R,V),0.01);
	sfactor = pow (sfactor, n);
	vec3 Is = Il*ks*sfactor;

	return clamp(Id,0,1) + clamp (Is,0,1);
	}


float shadeNiebla(float f){ //cálculo del float asociado a la niebla,
	float d=length(Pos);
	return clamp(exp(-f*pow(d,2)),0,1);
}





vec3 shade(){
	vec3 c = vec3(0.0);
	
	//ambiental
	c+=Ia*ka;	
	
	c+=shadeFocal(Il1,LVectorl1,direccionLuz1,Anglel1);
	c+=shadePuntual(Il2,LVectorl2);	
	c+=shadeDireccion(Il3,direccionLuz3);


	//emisiva
	c+= ke;

	//adición de niebla, mezclando el color original con uno de la niebla en base a un float que evoluciona de forma exponencial cuadrática
	return mix(fogColor,c,shadeNiebla(fogF));
	//return NText;
	
}




void main()
{

	ka=texture(colorTex,texCoord).xyz;
	kd=ka;
	ks=texture(specularTex,texCoord).xyz;
	n=100;
	ke=texture(emiTex,texCoord).xyz;

	//NText=normalize(NORM);
	NText=2*normalize(texture(normalTex,texCoord).xyz)-vec3(1.0);//En el libro pone que se codifcan asi para poder lograr valores negativos
	Pos=VERT;

	//outColor = vec4(( mix(shade(),fogColor,shadeNiebla(fogF)) ),0);  
	outColor = vec4(shade(),0);
	//outColor = vec4(LVectorl1,0);

}
