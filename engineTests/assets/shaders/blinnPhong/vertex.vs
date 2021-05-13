#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aModel;
layout (location = 9) in int aTexUnit;
layout (location = 10) in vec4 aTint;

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	flat int TexUnit;
	vec4 Tint;
} vs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
};

void main()
{
    // Transform the vertex positions to world space
    vs_out.FragPos = vec3(aModel * vec4(aPos, 1.0));

    // Transform the normals into world space by using the Normal Matrix
    vs_out.Normal = mat3(transpose(inverse(aModel))) * aNormal;

    // Output the texture coordinates
    vs_out.TexCoords = aTexCoords;
	
	// Output the texture unit
	vs_out.TexUnit = aTexUnit;
	
	// Output the tint
	vs_out.Tint = aTint;

    gl_Position = u_projection * u_view * vec4(vs_out.FragPos, 1.0);
}