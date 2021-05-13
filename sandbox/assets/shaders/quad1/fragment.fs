#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 texCoord;
in flat int texUnit1;
in flat int texUnit2;
in vec4 tint;

uniform sampler2D[16] u_textures;

void main()
{
	 colour = mix(texture(u_textures[texUnit1], texCoord), texture(u_textures[texUnit2], texCoord), 0.2) * tint;
}