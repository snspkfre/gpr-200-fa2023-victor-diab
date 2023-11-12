#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition;//Per-fragment interpolated world position
	vec3 WorldNormal;//Per-fragment interpolated world normal
}fs_in;

uniform sampler2D _Texture;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

uniform vec3 camPos;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	float intensity = 0.5;
	float iDif = intensity * max(dot(normal, normalize(_Lights[0].position - fs_in.WorldPosition)), 0);
	vec3 r = 2 * dot(normalize(_Lights[0].position - fs_in.WorldPosition), normal) * normal - (_Lights[0].position - fs_in.WorldPosition);
	float iSpec = 0.1 * max(dot(r, camPos), 0);

	FragColor = texture(_Texture,fs_in.UV) * (iSpec + iDif);
}