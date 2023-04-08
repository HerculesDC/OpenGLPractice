#version 450 core
layout (location = 0) in vec4 vertexInData;
layout (location = 1) in vec2 texInData;
layout (location = 2) uniform mat4 vertTransfMatrix;
layout (location = 3) uniform mat4 imgTransfMatrix;

//vec4 test = vec4(0.05, 0.0, 0.0, 0.0); //inaccessible in program
in vec4 test; //unused, but adding it to vertexInData shrinks the polygons

out vec4 vertexOutData; //inaccessible in program
out vec2 texOutData;

void main(){
	vec4 vertApplied = vertTransfMatrix * vertexInData;
    gl_Position = vertApplied;
    vertexOutData = vertApplied;
	vec4 imgApplied = imgTransfMatrix * vec4(texInData.x, texInData.y, 1.0, 1.0);
	texOutData = vec2(imgApplied.x, imgApplied.y);
}
