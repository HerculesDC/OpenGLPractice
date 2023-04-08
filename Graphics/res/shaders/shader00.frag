#version 450 core
in vec4 vertexOutData;

//CHECK THIS: https://www.khronos.org/opengl/wiki/Shader_Compilation#Interface_matching
in vec2 texOutData; //has to take in data from shader, even if not using
out vec4 fragColor;

void main(){
	fragColor = vec4(1.0-vertexOutData.x, 1.0-vertexOutData.y, 1.0-vertexOutData.z, 1.0);
}