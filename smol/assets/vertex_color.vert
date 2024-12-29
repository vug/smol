#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(std140, binding = 0) uniform UniformData {
  mat4 view;
  mat4 projection;
} uniforms;

layout(location = 0) out vec3 vertexColor;

void main()
{
	gl_Position = uniforms.projection * uniforms.view * vec4(position, 1.0);
	vertexColor = color;
}