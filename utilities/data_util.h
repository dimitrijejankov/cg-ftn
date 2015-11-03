#ifndef DATA_UTIL
#define DATA_UTIL

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef struct
{
    size_t width;
    size_t height;
    size_t length;

    int16_t * data;

} scan_data;

scan_data load_scan_data(const char *filename, size_t width, size_t height, size_t length, size_t bit_rate);
scan_data downsample(scan_data *source, size_t skip);

#endif