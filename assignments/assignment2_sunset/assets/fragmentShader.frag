#version 450
out vec4 FragColor;
in vec2 UV;
//defining variables set outside of the fragment shader
uniform float iTime;
uniform vec2 iResolution;
uniform vec3 dayColor[1];
uniform vec3 nightColor[1];
uniform vec3 mountainColor[1];
uniform float sunSpeed;
uniform float sunRadius;
uniform vec3 sunColor[1];

vec2 uv;

//function for drawing a circle
float circleSDF(vec2 p, float r){
    return length(p)-r;
}

//rectangle draw
float rectBnds(vec2 size, vec2 pos)
{
    float obj = step(pos.x + size.x, uv.x) + step(uv.x, pos.x);
    obj = obj + step(uv.y, pos.y) + step(pos.y + size.y, uv.y);
    obj = 1.0 - obj;
    return obj < 1.0 ? 0.0 : 1.0;
}

void main(){
    //setting uv
	uv = UV * 2.0 - 1.0;

    //setting day color and night color
    vec3 dayColor = vec3(dayColor[0]);
    vec3 nightColor = vec3(nightColor[0]);

    //setting time variable for other 
    float n = mod((iTime * sunSpeed) / 2.0, 4.0) - 2.0;

    //setting sun position
    vec2 sunPos = vec2(0.0, 0.5 + (-n * n));
    
    //setting background color
    vec3 color = mix(dayColor, nightColor, uv.y - 0.5 - (-n * n));
    uv.x *= iResolution.x / iResolution.y;
    //making the sun and adding it to the color
    float d = circleSDF(uv - sunPos,sunRadius);
    d = smoothstep(-0.05, 0.05, d);
    vec3 sunColor = vec3(sunColor[0]);
    color = mix(sunColor, color, d);
    
    //x and y for sea
    float otherX = uv.x * 10.0 - 5.0;
    float otherY = uv.y * 10.0;
    
    float sea = 1.0 - step(1.0 - abs(cos(otherX + iTime * sunSpeed)), otherY + 9.0);
    float sea2 = 1.0 - step(1.0 - abs(cos(otherX + iTime * sunSpeed * 1.3)), otherY + 9.0 + sin(iTime * 2.0) / 2.0);
    
    //setting buildings
    float building1 = rectBnds(vec2(0.1, 0.3), vec2(0.4, -0.12));
    float building2 = rectBnds(vec2(0.1, 0.45), vec2(0.45,-0.12));
    float roof = circleSDF(uv - vec2(0.5,0.34), 0.05);
    roof = 1.0 - step(0.0, roof);
    
    //drawing sea and buildings
    color = mix(color, vec3(1.0, 1.0, 1.0), sea2);
    color = mix(color, vec3(0.0, 0.0, 0.7), sea);
    color = mix(color, vec3(0.2), building1);
    color = mix(color, vec3(0.2), building2);
    color = mix(color, vec3(0.2), roof);
    //setting and drawing mountain
    float mountain = 1.0 - step(sin(otherX * 4.0) + -(otherX) * (otherX), otherY);
    color = mix(color, vec3(mountainColor[0]), mountain);
    
    //drawing frag color
    FragColor = vec4(color,1.0);
}
