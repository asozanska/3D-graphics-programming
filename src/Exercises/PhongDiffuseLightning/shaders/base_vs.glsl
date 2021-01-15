#version 410

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texture_coordinates;
layout(location=2) in  vec3 a_vertex_normal;
layout(std140) uniform Transformations {
    mat4 P;
    mat4 VM;
    mat3 N;
};
out vec2 vertex_texture_coordinates;
out vec3 vertex_normal_in_vs;
out vec3 vertex_position_in_vs;


void main() {
    gl_Position = P * VM * a_vertex_position;
    vertex_texture_coordinates = a_vertex_texture_coordinates;
    vertex_position_in_vs = (VM * a_vertex_position).rgb;
    vertex_normal_in_vs = normalize(N*a_vertex_normal);

}
