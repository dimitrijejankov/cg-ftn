#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <GL/gl.h>
#include "vector_buffer.h"
#include "../utilities/data_util.h"

typedef struct {

    GLuint vertex_buffer, normal_buffer;
    GLuint vertex_shader, fragment_shader, program;

    struct {
        GLuint position;
        GLuint normal;
    } attributes;

    struct {
        GLint normal_matrix;
        GLint mvp_matrix;
    } uniform;

    GLuint vertex_count;

    float center[3];

} geometry;

extern float step_size;

void marching_cubes(float threshold, scan_data *data, geometry* out);
void render_geometry(geometry *in, mat4_t model, mat4_t view, mat4_t projection);

extern const int cube_vertices[8][3];
extern const int cube_edge_connections[12][2];
extern const int cube_edge_directions[12][3];
extern int cube_edge_flags[256];
extern int triangle_connection_table[256][16];

#endif