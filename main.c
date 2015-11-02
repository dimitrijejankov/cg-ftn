#include <GL/glew.h>
#include <GL/glut.h>
#include "geometry/marching_cubes.h"
#include "configuration.h"

geometry g;

mat4_t projection;
mat4_t view;
mat4_t model;

float up[3] = {0.0f, 1.0f, 0.0f};
float center[3] = {0.0f, 0.0f,0.0f};
float eye[3] = {0.0f, 0.0f, -40.0f};

void InitGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2 , 0.6, 0.0);

    projection = mat4_create(projection);

    view = mat4_create(view);

    model = mat4_create(model);
    model = mat4_identity(model);

    scan_data data = load_scan_data(filename, data_width, data_height, data_length);
    marching_cubes(threshold, &data, debug, &g);
    model = mat4_translate(model, g.center, model);
    free(data.data);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4_identity(view);
    view = mat4_lookAt(eye, center, up, view);

    render_geometry(&g, model, view, projection);

    if (debug)
    {
        render_debug(&g, model, view, projection);
    }

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    glViewport(0, 0, width, height);

    mat4_identity(projection);
    projection = mat4_perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.01f, 100000.0f , projection);

    mat4_identity(view);
    view = mat4_lookAt(eye, center, up, view);
}

void keyboard(unsigned char key, int x, int y)
{
    float direction[3];
    float rotation[16];
    mat4_identity(rotation);

    switch (key) {
        case 'q':
            vec3_subtract(center, eye, direction);
            vec3_normalize(direction, direction);
            vec3_add(eye, direction, eye);
            break;
        case 'e':
            vec3_subtract(center, eye, direction);
            vec3_normalize(direction, direction);
            vec3_subtract(eye, direction, eye);
            break;
        case 'a':
            mat4_rotateY(rotation, 0.1f, rotation);
            mat4_multiply(rotation, model, model);
            break;
        case 'd':
            mat4_rotateY(rotation, -0.1f, rotation);
            mat4_multiply(rotation, model, model);
            break;
        case 'w':
            mat4_rotateX(rotation, 0.1f, rotation);
            mat4_multiply(rotation, model, model);
            break;
        case 's':
            mat4_rotateX(rotation, -0.1f, rotation);
            mat4_multiply(rotation, model, model);
            break;
        case 27:
            exit(0);
        default:
            break;
    }
}

void shutdown()
{
}

void main(int argc, char **argv)
{
    parse_args(argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Gomputer graphics ftn"); //

    glewInit();
    InitGL();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    shutdown();
}

