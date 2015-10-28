#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "geometry/marching_cubes.h"

geometry g;

mat4_t projection;
mat4_t view;
mat4_t model;

int debug = 0;
float threshold = 90;

size_t data_width;
size_t data_height;
size_t data_length;
char *filename;

float rotation = 0.0f;
float scale = 0.05f;

void InitGL()
{

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2 , 0.6, 0.0);

    projection = mat4_create(projection);
    view = mat4_create(view);
    model = mat4_create(model);

    model = mat4_identity(model);

    scan_data data = load_scan_data(filename, data_width, data_height, data_length);
    marching_cubes(threshold, &data, &g);
    free(data.data);
}

float x = 0.0f;
float y = -0.5f;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float p[] = {0.0f, 0.0f, -1.5f};
    p[0] = x;
    p[1] = y;

    float s[] = {0.0f, 0.0f, -1.5f};
    s[0] = scale;
    s[1] = scale;
    s[2] = scale;

    mat4_identity(model);
    model = mat4_translate(model, p, model);
    model = mat4_rotateX(model, rotation, model);
    model = mat4_scale(model, s, model );

    render_geometry(&g, model, view, projection);

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

    glLoadIdentity();
    mat4_identity(view);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
            rotation += 2.0f;
            break;
        case 's':
            rotation -= 2.0f;
            break;
        case 'a':
            scale *= 1.1f;
            break;
        case 'd':
            scale /= 1.1f;
            break;
        case 27:
            exit(0);
        default:
            break;
    }
}

void arrow_keys(int a_keys, int t, int w)
{
    switch (a_keys) {
        case GLUT_KEY_UP:
            x += 0.01;
            break;
        case GLUT_KEY_DOWN:
            x -= 0.01;
            break;
        case GLUT_KEY_LEFT:
            y += 0.01;
            break;
        case GLUT_KEY_RIGHT:
            y -= 0.01;
            break;
        default:
            break;
    }
}

void shutdown()
{
}

void parse_arg(char *arg)
{
    int match = 0;

    if(strcmp(arg, "debug=true") == 0)
    {
        debug = 1;
        match = 1;
    }

    if(strncmp(arg, "trashold=", 9) == 0)
    {
        sscanf(arg+=9, "%f", &threshold);
        match = 1;
    }


    if(match != 1)
    {
        printf("Argument %s not recognized", arg);
        exit(0);
    }
}

void parse_args(int argc, char **argv)
{
    if(argc < 5)
    {
        printf("At lease 4 arguments must be supplied");
        exit(0);
    }

    filename = argv[1];

    sscanf(argv[2], "%lu", &data_width);
    sscanf(argv[3], "%lu", &data_height);
    sscanf(argv[4], "%lu", &data_length);

    if(argc > 5)
    {
        for(int i = 5; i < argc; ++i)
            parse_arg(argv[i]);
    }

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
    glutSpecialFunc(arrow_keys);

    glutMainLoop();


    shutdown();

}

