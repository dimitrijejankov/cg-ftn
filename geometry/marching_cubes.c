#include <stdio.h>
#include <GL/glew.h>
#include "marching_cubes.h"
#include "../utilities/glutil.h"

float get_intersection_offset(float a, float b, float desired) {
    float delta = b - a;

    if (delta == 0.0) {
        return 0.5;
    }
    return (desired - a) / delta;
}

float sample(size_t x, size_t y, size_t z, scan_data *data)
{
    if(x < 0 || y < 0 || z < 0 || x >= data->width || y >= data->height || z >= data->length)
        return 0.0f;

    return (float) data->data[x + y * data->width + z * data->width * data->height];
}

void get_normal(vec3_t normal, size_t x, size_t y, size_t z, scan_data *data) {
    normal[0] = sample(x - 1, y, z, data) - sample(x + 1, y, z, data);
    normal[1] = sample(x, y - 1, z, data) - sample(x, y + 1, z, data);
    normal[2] = sample(x, y, z - 1, data) - sample(x, y, z + 1, data);

    vec3_normalize(normal, normal);
}

void calculate_normal(vec3_t normal, int edge, size_t x, size_t y, size_t z, float offset, scan_data *data)
{
    vec3 v1;
    vec3 v2;

    get_normal((vec3_t) &v1, (x + cube_vertices[cube_edge_connections[edge][0]][0]),
                             (y + cube_vertices[cube_edge_connections[edge][0]][1]),
                             (z + cube_vertices[cube_edge_connections[edge][0]][2]),
                             data);

    get_normal((vec3_t) &v2, (x + cube_vertices[cube_edge_connections[edge][1]][0]),
                             (y + cube_vertices[cube_edge_connections[edge][1]][1]),
                             (z + cube_vertices[cube_edge_connections[edge][1]][2]),
                             data);

    normal[0] = v1[0] + (v2[0] - v1[0]) * offset;
    normal[1] = v1[1] + (v2[1] - v1[1]) * offset;
    normal[2] = v1[2] + (v2[2] - v1[2]) * offset;

    vec3_normalize(normal, normal);
}

void march_cube(size_t x, size_t y, size_t z, float scale, float threshold, scan_data *data, vector_buffer *vertices, vector_buffer *normals) {

    int corner, vertex, vertex_test, edge, triangle, flag_index, edge_flags;
    float offset;
    float cube_values[8];

    vec3 edge_vertex[12];
    vec3 edge_norm[12];
    vec3 points;
    vec3 normal;

    for (vertex = 0; vertex < 8; vertex++) {
        cube_values[vertex] = sample(x + cube_vertices[vertex][0],
                                     y + cube_vertices[vertex][1],
                                     z + cube_vertices[vertex][2],
                                     data);
    }

    flag_index = 0;
    for (vertex_test = 0; vertex_test < 8; vertex_test++) {
        if (cube_values[vertex_test] <= threshold)
            flag_index |= 1 << vertex_test;
    }


    edge_flags = cube_edge_flags[flag_index];


    if (edge_flags == 0) {
        return;
    }

    for (edge = 0; edge < 12; edge++) {
        if (edge_flags & (1 << edge)) {
            offset = get_intersection_offset(cube_values[cube_edge_connections[edge][0]],
                                             cube_values[cube_edge_connections[edge][1]], threshold);

            edge_vertex[edge][0] = (x + cube_vertices[cube_edge_connections[edge][0]][0] + offset * cube_edge_directions[edge][0]) * scale;
            edge_vertex[edge][1] = (y + cube_vertices[cube_edge_connections[edge][0]][1] + offset * cube_edge_directions[edge][1]) * scale;
            edge_vertex[edge][2] = (z + cube_vertices[cube_edge_connections[edge][0]][2] + offset * cube_edge_directions[edge][2]) * scale;


            calculate_normal((vec3_t) &edge_norm[edge], edge, x, y, z, offset, data);
        }
    }

    for (triangle = 0; triangle < 5; triangle++) {
        if (triangle_connection_table[flag_index][3 * triangle] < 0)
            break;

        for (corner = 0; corner < 3; corner++) {
            vertex = triangle_connection_table[flag_index][3 * triangle + corner];

            vec3_set((vec3_t) &edge_vertex[vertex], (vec3_t) &points);
            vec3_set((vec3_t) &edge_norm[vertex], (vec3_t) &normal);

            vector_append(vertices, (vec3_t) &points);
            vector_append(normals, (vec3_t) &normal);
        }
    }
}

void marching_cubes(float threshold, scan_data *data, geometry* out) {

    float temp[3];

    vector_buffer vertices;
    vector_buffer normals;

    vector_init(&vertices);
    vector_init(&normals);

    for (size_t x = 0; x < data->length; x++)
        for (size_t y = 0; y < data->width; y++)
            for (size_t z = 0; z < data->height; z++) {
                march_cube(x, y, z, step_size, threshold, data, &vertices, &normals);
            }

    out->vertex_count = (GLuint) vertices.size;
    out->attributes.position = 0;
    out->attributes.normal = 1;

    make_buffer(out->attributes.position, &out->vertex_buffer, vertices.data , (GLsizei) (out->vertex_count * 3 * sizeof(float)));
    make_buffer(out->attributes.normal, &out->vertex_buffer, normals.data , (GLsizei) (out->vertex_count * 3 * sizeof(float)));

    out->vertex_shader = make_shader(GL_VERTEX_SHADER, "shaders/diffuse_shader.v.glsl");
    out->fragment_shader = make_shader(GL_FRAGMENT_SHADER, "shaders/diffuse_shader.f.glsl");

    out->program = make_program(out->vertex_shader, out->fragment_shader);

    glBindAttribLocation(out->program, out->attributes.position, "position");
    glBindAttribLocation(out->program, out->attributes.normal, "normal");

    glLinkProgram(out->program);

    out->uniform.mvp_matrix = glGetUniformLocation(out->program, "mvp_matrix");
    out->uniform.normal_matrix = glGetUniformLocation(out->program, "normal_matrix");

    out->center[0] = 0.0f;
    out->center[1] = 0.0f;
    out->center[2] = 0.0f;

    for(size_t i = 0; i < out->vertex_count; ++i)
    {
        vec3_scale(vertices.data[i], -1.0f / out->vertex_count, temp);
        vec3_add(out->center, temp, out->center);
    }

    vector_free(&vertices);
    vector_free(&normals);
}

void render_geometry(geometry *in, mat4_t model, mat4_t view, mat4_t projection)
{
    glUseProgram(in->program);

    float mvp[16];
    mat4_multiply(projection, view, mvp);
    mat4_multiply(mvp, model , mvp);

    float normal_matrix[16];
    mat4_multiply(view, model , normal_matrix);

    mat4_inverse(normal_matrix, normal_matrix);
    mat4_transpose(normal_matrix, normal_matrix);

    glUniformMatrix4fv(in->uniform.mvp_matrix, 1, GL_FALSE, mvp);
    glUniformMatrix4fv(in->uniform.normal_matrix, 1, GL_FALSE, normal_matrix);

    glDrawArrays(GL_TRIANGLES, 0, in->vertex_count);
}