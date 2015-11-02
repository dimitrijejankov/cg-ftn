#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int debug = 0;
float threshold = 90;
char *filename = NULL;

size_t data_width = 0;
size_t data_height = 0;
size_t data_length = 0;

void parse_arg(char *arg)
{
    int match = 0;

    if(strcmp(arg, "debug=true") == 0)
    {
        debug = 1;
        match = 1;
    }

    if(strcmp(arg, "debug=false") == 0)
    {
        debug = 0;
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