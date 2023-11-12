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
uniform float shininess;
uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform int mode;
uniform int numLights;
uniform float lightPower;

void main(){
	vec3 ambient = vec3(1.0) * ambient;
	vec3 sumColor = vec3(0);
	for(int i = 0; i < numLights; i++)
	{
		vec3 normal = normalize(fs_in.WorldNormal);
		vec3 lightDirection = _Lights[i].position - fs_in.WorldPosition;
		lightDirection = normalize(lightDirection);
		float dist = length(lightDirection);
		dist = dist * dist;
		vec3 dif = _Lights[i].color * diffuse;
		vec3 specCol = _Lights[i].color * specular;

		float iDif = max(dot(normal, lightDirection), 0);
		float spec = 0.0;
		if(iDif > 0)
		{
			vec3 viewDir = normalize(camPos - fs_in.WorldPosition);
			if(mode == 0)
			{
				vec3 halfDir = normalize(lightDirection + viewDir);
				float specAngle = max(dot(halfDir, normal), 0.0);
				spec = pow(specAngle, shininess);
			}
			else
			{
				vec3 r = reflect(-lightDirection, normal);
				float specAngle = max(dot(r, viewDir), 0.0);
				spec = pow(specAngle, shininess/4.0);
			}
		}
		sumColor += dif * iDif * _Lights[i].color * lightPower / dist + 
					 specCol * spec * _Lights[i].color * lightPower / dist;
	}
	vec3 color = ambient + sumColor;

	vec4 textColor = texture(_Texture,fs_in.UV);
	FragColor = vec4(textColor.rgb * color, textColor.a);
}