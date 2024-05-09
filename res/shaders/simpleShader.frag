#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture0;
uniform vec2 direction;


vec2 flipTextureHorizontal(vec2 texCoord) {
    return vec2(1.0 - texCoord.x, texCoord.y);
}

vec2 flipTextureVertical(vec2 texCoord) {
    return vec2(texCoord.x, 1.0 - texCoord.y);
}

void main(){
    vec2 flippedTexCoord = flipTextureVertical(TexCoord);
    if(direction.x < 0.0)
        flippedTexCoord = flipTextureHorizontal(flippedTexCoord);

    vec4 texColor = texture(ourTexture0, flippedTexCoord);
    if (texColor.a < 0.9)
        discard;
    FragColor = texColor;
}
