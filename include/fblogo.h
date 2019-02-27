#ifndef PSPLASH_BMP_H
#define PSPLASH_BMP_H

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

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

struct fblogo_info {
    int     fd;
    char    *data;
    char    *base;
    int     fb_width;
    int     fb_height;
    int     fb_depth;
    int     img_offset;
    int     img_width;
    int     img_height;
    int     img_depth;
    int     img_size;
    int     img_rowsize;
    int     img_red;
    char    img_green;
    char    img_blue;
    char    img_alpha;
};

int fblogo_prepare_image(FILE *file, struct fblogo_info *info);
int fblogo_draw_color(struct fblogo_info *info, char r, char g, char b);
int fblogo_draw_image(struct fblogo_info *info);
struct fblogo_info *fblogo_fb_new(void);
void fblogo_fb_destroy(struct fblogo_info *info);

#endif
