#version 330 core
layout(location = 0) in vec3 pos;

uniform vec4 vertexColor;

void main() {
    gl_Position = vec4(pos, 1.0);
} 