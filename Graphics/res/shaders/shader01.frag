#version 450 core
out vec4 fragColor;
//layout (location = 2) uniform float angle; //COMMENTED OUT
in vec4 vertexOutData;
in vec2 texOutData; //from vertex shader
layout (location = 3) uniform sampler2D floorTexture;
layout (location = 4) uniform sampler2D snakeTexture;

void main(){
	fragColor = mix(texture(floorTexture, texOutData), texture(snakeTexture, texOutData), 0.5);

	//fragColor = texture(snakeTexture, texOutData.xy);
}
	
	
	
	//fragColor = vec4(0.5+0.5*vertexOutData.y, 0.5+0.5*vertexOutData.x, 0.5, 1.0);
	//fragColor = vec4(0.5+(0.5*sin(angle)), 0.5+(0.5*cos(angle)), texOutData.x, 1.0);