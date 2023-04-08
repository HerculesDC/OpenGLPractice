#version 450 core
layout (location = 0) in vec4 vertexInData;
layout (location = 1) in vec2 texInData;
//vec4 test = vec4(0.05, 0.0, 0.0, 0.0); //inaccessible in program
in vec4 test; //unused, but adding it to vertexInData shrinks the polygons

out vec4 vertexOutData; //inaccessible in program
out vec2 texOutData;

void main(){
    gl_Position = vertexInData;
    vertexOutData = vertexInData;
	texOutData = texInData;
}
