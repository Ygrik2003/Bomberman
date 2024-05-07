#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 clr;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float screenRation;

float ax = screenRation * 2. / 30;
float ay = -2. / 30;

void main(){
    TexCoord = aTexCoord;
    // TexCoord = vec2(sin(aTexCoord.x), sin(aTexCoord.y));

    gl_Position = vec4(ax * aPos.x - 1, ay * aPos.y + 1, 0.0, 1.0);
}