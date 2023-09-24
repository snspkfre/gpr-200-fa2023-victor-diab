#version 450
out vec4 FragColor;
in vec2 UV;
uniform float iTime;
void main(){
	float t = sin(UV.x*6.28+iTime)*0.5 + 0.5;
    FragColor = vec4(t,t,t,1.0);
	//FragColor = vec4(UV,0.0,1.0);
}
