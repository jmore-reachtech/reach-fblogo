#ifndef PSPLASH_BMP_H
#define PSPLASH_BMP_H

#include <stdint.h>

typedef struct {
    uint8_t     magic[2];   /* BM is all we support */
    uint32_t    filesz;     /* size of the file in bytes*/
    uint16_t    reserved1;  /* reserved */
    uint16_t    reserved2;  /* reserved */
    uint32_t    offset;     /* offset to bitmap data */
} BITMAPFILEHEADER;

typedef struct {
    uint32_t header_sz;     /* the size of this header */
    uint32_t width;         /* the bitmap width in pixels */
    uint32_t height;        /* the bitmap height in pixels */
    uint16_t nplanes;       /* the number of color planes being used. */
    uint16_t depth;         /* the number of bits per pixel */
    uint32_t compress_type; /* the compression method being used */
    uint32_t bmp_bytesz;    /* the size of the raw bitmap data */
    uint32_t hres;          /* the horizontal resolution of the image.
                             (pixel per meter) */
    uint32_t vres;          /* the vertical resolution of the image.
                             (pixel per meter) */
    uint32_t ncolors;       /* the number of colors in the color palette,
                             or 0 to default to 2<sup><i>n</i></sup>. */
    uint32_t nimpcolors;    /* the number of important colors used,
                             or 0 when every color is important;
                             generally ignored. */ 
} BITMAPINFOHEADER;

struct bmp_img_info {
    uint32_t offset;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t size;
    uint8_t *data;
    uint32_t rowsize;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

int fblogo_prepare_image(FILE *file, struct bmp_img_info *info);
int fblogo_draw_color(char *fb, struct bmp_img_info *info, char r, char g, char b);
int fblogo_draw_image(char *fb, struct bmp_img_info *info);

#endif
