#version 330 core
out vec4 FragColor;

in vec3 colorInfo;
in vec2 TexCoord;

uniform sampler2D bgTexture;
uniform sampler2D fgTexture;

void main() {
    FragColor = mix(texture(bgTexture, TexCoord), texture(fgTexture, vec2(-TexCoord.s, TexCoord.t)), 0.5);
}