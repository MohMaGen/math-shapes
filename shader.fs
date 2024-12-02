#version 330
in vec2 fragTexCoord;
in vec3 fragPosition;

uniform vec3 viewPos;
uniform vec4 fogColor;
uniform vec4 voxelColor;

out vec4 finalColor;

void main() {
    float dist      = length(viewPos - fragPosition) - 2.;
    float fogFactor = 1 / exp(dist * dist * 0.2);
    fogFactor       = clamp(fogFactor, 0.0, 1.0);
    finalColor      = mix(fogColor, voxelColor, fogFactor);
}
