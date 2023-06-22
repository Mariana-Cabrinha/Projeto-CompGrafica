#version 330 core

in vec3 fragNormal;
in vec3 fragLightDir;
in vec3 fragViewDir;

out vec4 fragColor;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(fragLightDir);
    vec3 V = normalize(fragViewDir);
    vec3 R = reflect(-L, N);

    vec4 ambient = ambientColor;
    vec4 diffuse = diffuseColor * max(dot(N, L), 0.0);
    vec4 specular = specularColor * pow(max(dot(R, V), 0.0), shininess);

    fragColor = ambient + diffuse + specular;
}
