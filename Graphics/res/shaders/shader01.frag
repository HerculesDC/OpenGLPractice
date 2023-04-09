#version 450 core
out vec4 fragColor;
layout (location = 7) uniform float angle;
in vec4 vertexOutData;
in vec2 texOutData; //from vertex shader
layout (location = 8) uniform sampler2D floorTexture;
layout (location = 9) uniform sampler2D snakeTexture;

void main(){
	vec4 temp = mix(texture(floorTexture, texOutData), texture(snakeTexture, texOutData), 0.5+(0.5*cos(angle)));
	fragColor = vec4(temp.xyz, 0.5+(0.5*sin(angle)));
}
	//fragColor = texture(snakeTexture, texOutData.xy);
	//fragColor = vec4(0.5+0.5*vertexOutData.y, 0.5+0.5*vertexOutData.x, 0.5, 1.0);
	//fragColor = vec4(0.5+(0.5*sin(angle)), 0.5+(0.5*cos(angle)), texOutData.x, 1.0);