#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPosition;

uniform vec3 viewPos;
uniform vec4 fogColor;

out vec4 finalColor;

void main() {
    float dist      = length(viewPos - fragPosition);
    float fogFactor = 1 / exp(dist*dist * 0.1);
    fogFactor = clamp(fogFactor, 0.0, 0.7);
    finalColor = mix(fogColor, fragColor, fogFactor);
}
