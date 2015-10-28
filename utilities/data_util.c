#include "data_util.h"
#include <stdio.h>
#include <stdlib.h>

scan_data load_scan_data(const char *filename, size_t width, size_t height, size_t length)
{
    scan_data ret;
    FILE *f = fopen(filename, "r");

    if(f == NULL)
    {
        printf("File opening failed!");
        exit(0);
    }

    fseek(f, 0L, SEEK_END);
    long fs = ftell(f);
    fseek(f, 0L, SEEK_SET);

    if(fs != width * height * length * sizeof(int8_t))
    {
        printf("File does not match the given parameters");
        exit(0);
    }

    int8_t *data = malloc(width * height * length * sizeof(int8_t));
    fread(data, sizeof(int8_t), width * height * length, f);

    ret.height = height;
    ret.width = width;
    ret.length = length;
    ret.data = data;

    return ret;
}

scan_data downsample(scan_data *source, size_t skip){

    scan_data ret;

    ret.width = source->width/skip;
    ret.height = source->height/skip;
    ret.length = source->length/skip;

    ret.data = malloc(sizeof(int8_t) * ret.width * ret.height * ret.length);

    int ii = 0;
    int jj = 0;
    int kk = 0;

    for(size_t i = 0; i < source->width; i += skip)
    {
        jj = 0;
        for(size_t j = 0; j < source->width; j += skip)
        {
            kk = 0;
            for(size_t k = 0; k < source->width; k += skip)
            {
                ret.data[ii + jj * ret.width + kk * ret.width * ret.length] = source->data[i + j * source->width + k * source->width * source->length];
                kk++;
            }
            jj++;
        }
        ii++;
    }

    return ret;
}