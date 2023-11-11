#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

uniform sampler2D _Texture;

struct Light
{
	vec3 position;
	vec3 color;
};
uniform Light _Light;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	float intensity = 10.0;
	float i = intensity * max(dot(normal, normalize(_Light.position - fs_in.WorldPosition)), 0);

	FragColor = texture(_Texture,fs_in.UV);
}