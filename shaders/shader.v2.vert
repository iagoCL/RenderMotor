#version 330 core

in vec3 inPos;	
in vec3 inNormal;
in vec2 inTexCoord;	

uniform mat4 modelViewProj;
uniform mat4 normal;
uniform mat4 modelView;

out vec3 NORM;
out vec3 VERT;
out vec2 texCoord;




void main()
{
	NORM=(normal*vec4(inNormal,0)).xyz;
	VERT=(modelView * vec4(inPos,1)).xyz;

	texCoord=inTexCoord;

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
