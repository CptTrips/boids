#version 450

/*
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;
*/

layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec3 inColor;
//layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
//layout(location = 1) out vec2 fragTexCoord;


void main() {

    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    gl_Position = vec4(inPosition.xy, 0.5, 1.0);

    gl_PointSize = 3.0;

    fragColor = vec3(1.0, 0.0, 0.0);

    //fragTexCoord = inTexCoord;
}