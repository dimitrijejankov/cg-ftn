#ifndef VECTOR_BUFFER_H
#define VECTOR_BUFFER_H

#include "../math/gl_matrix.h"

#define VECTOR_INITIAL_CAPACITY 100

typedef struct {
    int size;
    int capacity;
    vec3 *data;
} vector_buffer;

void vector_init(vector_buffer *vector);

void vector_append(vector_buffer *vector, vec3_t value);

vec3_t vector_get(vector_buffer *vector, int index);

void vector_set(vector_buffer *vector, int index, vec3_t value);

void vector_double_capacity_if_full(vector_buffer *vector);

void vector_free(vector_buffer *vector);

#endif