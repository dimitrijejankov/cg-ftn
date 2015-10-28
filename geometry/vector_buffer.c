// vector.c

#include <stdio.h>
#include <stdlib.h>
#include "../geometry/vector_buffer.h"


void vector_init(vector_buffer *vector) {
    vector->size = 0;
    vector->capacity = VECTOR_INITIAL_CAPACITY;
    vector->data = malloc(sizeof(vec3) * vector->capacity);
}

void vector_append(vector_buffer *vector, vec3_t value) {
    vector_double_capacity_if_full(vector);
    vec3_set(value, (vec3_t) &vector->data[vector->size++]);
}

vec3_t vector_get(vector_buffer *vector, int index) {
    if (index >= vector->size || index < 0) {
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }
    return (vec3_t) &vector->data[index];
}

void vector_set(vector_buffer *vector, int index, vec3_t value) {
    while (index >= vector->size) {
        vector_append(vector, 0);
    }
    vec3_set(value, (vec3_t) &vector->data[index]);
}

void vector_double_capacity_if_full(vector_buffer *vector) {
    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;
        vector->data = realloc(vector->data, sizeof(vec3) * vector->capacity);
    }
}

void vector_free(vector_buffer *vector) {
    free(vector->data);
}