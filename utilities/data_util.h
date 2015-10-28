#ifndef DATA_UTIL
#define DATA_UTIL

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    size_t width;
    size_t height;
    size_t length;

    int8_t * data;

} scan_data;

scan_data load_scan_data(const char *filename, size_t width, size_t height, size_t length);
scan_data downsample(scan_data *source, size_t skip);

#endif