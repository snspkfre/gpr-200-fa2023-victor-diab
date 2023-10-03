#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _BG1Texture;
uniform sampler2D _BG2Texture;
uniform sampler2D _NoiseTexture;
uniform float iTime;
uniform float _DistStrength;
uniform float _DistSpeed;
uniform float _Tiling;

void main(){
	float normalizedTime = abs(mod(iTime * 0.5 * _DistSpeed, 2) - 1) - 0.5;

	float distort = texture(_NoiseTexture, UV).r * normalizedTime;

	vec2 uv = UV;
	uv += distort * _DistStrength;
	
	vec4 textureA = texture(_BG1Texture, uv * _Tiling);
	vec4 textureB = texture(_BG2Texture, uv * _Tiling);
	vec3 color = mix(textureA.rgb, textureB.rgb, textureB.a * 0.5);
	FragColor = vec4(color, 1.0);
}
