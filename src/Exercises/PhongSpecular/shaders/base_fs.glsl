#version 410

layout(location=0) out vec4 vFragColor;
in vec2 vertex_texture_coordinates;
in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D shininess_map;

layout(std140) uniform Light {
    vec3 position_in_vs;
    vec3 color;
    vec3 a;
    vec3 ambient;
} light;

layout(std140) uniform Material {
    vec3 Kd;
    uint Kd_map;
    vec3 Ks;
    uint Ks_map;
    float Ns;
    uint Ns_map;
} material;

vec3 specular_color = vec3(1.0, 1.0, 1.0);
vec4 diffuse_color;
float shininess = 500.0;
float M_PI = 3.1415926;

void main() {
    vec3 normal = normalize(vertex_normal_in_vs);
    if (!gl_FrontFacing) {
        normal = -normal;
    }
    vec3 light_vector = light.position_in_vs - vertex_position_in_vs;
    float r = length(light_vector);
    light_vector/=r;
    float attenuation = 1.0f / (light.a[0]+light.a[1]*r+light.a[2]*r*r);
    float light_in = max(0.0f, dot(normal, light_vector)) * attenuation;
    vec4 diffuse_color = texture(diffuse_map, vertex_texture_coordinates);
    vFragColor.a = diffuse_color.a;
    vFragColor.rgb = diffuse_color.rgb * light.ambient.rgb;
    diffuse_color.rgb /= M_PI;

    vec3 view_vector = normalize(-vertex_position_in_vs);
    vec3 half_vector = normalize(view_vector + light_vector);

    if (material.Kd_map>0) {
        diffuse_color.a = texture(diffuse_map, vertex_texture_coordinates).a;
        diffuse_color.rgb = texture(diffuse_map, vertex_texture_coordinates).rgb*material.Kd;
    } else {
        diffuse_color.a = 1;
        diffuse_color.rgb = material.Kd;
    }
    if (material.Ks_map>0) {
        specular_color = texture(specular_map, vertex_texture_coordinates).rgb*material.Ks;
    } else {
        specular_color = material.Ks;
    }
    if (material.Ns_map>0) {
        shininess = texture(shininess_map, vertex_texture_coordinates).r*material.Ns;
    } else {
        shininess = material.Ns;
    }
    float specular = ((shininess + 8.0f) / M_PI * 8.0f) * (pow(max(dot(half_vector, normal), 0.0f), shininess));
    vFragColor.rgb += light_in * diffuse_color.rgb;
    vFragColor.rgb += light_in * light.color * specular * specular_color;
}
