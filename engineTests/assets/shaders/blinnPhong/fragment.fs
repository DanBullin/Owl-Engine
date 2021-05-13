#version 440 core
			
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    flat int TexUnit;
    vec4 Tint;
} fs_in;

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

uniform sampler2D[32] u_diffuseMap;

void main()
{    
    float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * u_lightColour;

	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(u_lightPos - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColour;

	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * u_lightColour;  
	
	FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(u_diffuseMap[fs_in.TexUnit], fs_in.TexCoords) * fs_in.Tint;
}