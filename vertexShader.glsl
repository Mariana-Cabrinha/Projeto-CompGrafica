#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 fragNormal;
out vec3 fragLightDir;
out vec3 fragViewDir;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

void main()
{
    vec4 worldPos = modelMatrix * vec4(position, 1.0);
    vec4 viewPos = viewMatrix * worldPos;

    gl_Position = projectionMatrix * viewPos;
    fragNormal = normalize((normalMatrix * vec4(normal, 0.0)).xyz);
    fragLightDir = normalize(lightPosition - worldPos.xyz);
    fragViewDir = normalize(-viewPos.xyz);
}
