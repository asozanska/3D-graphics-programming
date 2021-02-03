#version 410

layout(location=0) out vec4 vFragColor;
in vec2 vertex_texture_coordinates;
in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;
uniform sampler2D diffuse_map;

layout(std140) uniform Light {
    vec3 position_in_vs;
    vec3 color;
    vec3 a;
    vec3 ambient;
} light;

vec3 specular_color = vec3(1.0, 1.0, 1.0);
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
    float specular = ((shininess + 8.0f) / M_PI * 8.0f) * (pow(max(dot(half_vector, normal), 0.0f), shininess));
    vFragColor.rgb += light_in * light.color * specular;

}
