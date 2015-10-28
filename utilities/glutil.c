#include <GL/glew.h>
#include <GL/glut.h>
#include "util.h"
#include <stdio.h>

void show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog) {
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}


void make_buffer(GLuint attribute, GLuint *vertex_buffer, const void *buffer_data, GLsizei buffer_size) {

    glGenBuffers(1, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attribute);
    glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer);
}

GLuint make_shader(GLenum type, const char *filename) {
    GLint length;
    GLchar *source = file_contents(filename, &length);
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **) &source, &length);
    free(source);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s:\n", filename);
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint make_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    return program;
}