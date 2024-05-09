#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 clr;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float screenRatio;
uniform float dt;
uniform vec2 direction;

float ax = screenRatio * 2. / 16;
float ay = -2. / 16;

void main(){
    TexCoord = aTexCoord;
    // TexCoord = vec2(sin(aTexCoord.x), sin(aTexCoord.y));

    gl_Position = vec4( ax * (aPos.x - direction.x * (1 - dt)) - 1,
                        ay * (aPos.y - direction.y * (1 - dt)) + 1, 
                        0.0, 1.0);
}