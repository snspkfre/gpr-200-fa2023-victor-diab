#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float iTime;
uniform float _moveSpeed;
uniform float _charSize;

void main(){
	UV = vUV;
	vec3 pos = vPos;
	float x = mod(iTime * _moveSpeed, 5);
	float movementX = abs(mod(iTime * _moveSpeed, 10) - 5) - 2.5;
	float movementY = -x * x + 5 * x - 3;

	pos += vec3(movementX, movementY, 0.0) * vec3(0.2);
	gl_Position = vec4(pos, 1.5 / _charSize);
}