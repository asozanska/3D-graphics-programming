#version 410

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texture_coordinates;
layout(std140) uniform Transformations {
    mat4 PVM;
};
out vec2 vertex_texture_coordinates;

void main() {
    gl_Position = PVM * a_vertex_position;
    vertex_texture_coordinates = a_vertex_texture_coordinates;
}
