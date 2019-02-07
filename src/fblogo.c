#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fblogo.h"

int fblogo_prepare_image(FILE *file, struct bmp_img_info *info)
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    int ret        = 0;

    memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
    memset(&bih, 0, sizeof(BITMAPINFOHEADER));

    fread(&bfh.magic,2,1,file);
    
    fseek(file,2,SEEK_SET);
    fread(&bfh.filesz,4,1,file);

    fseek(file,10,SEEK_SET);
    fread(&bfh.offset,4,1,file);
    info->offset = bfh.offset;
    info->size = (bfh.filesz - bfh.offset);
    
    fseek(file,18,SEEK_SET);
    fread(&bih.width,4,1,file);
    info->width = bih.width;

    fseek(file,22,SEEK_SET);
    fread(&bih.height,4,1,file);
    info->height = bih.height;

    fseek(file,26,SEEK_SET);
    fread(&bih.nplanes,2,1,file);

    fseek(file,28,SEEK_SET);
    fread(&bih.depth,2,1,file);
    info->depth = bih.depth;

    fseek(file,30,SEEK_SET);
    fread(&bih.compress_type,4,1,file);

    info->data = malloc(info->size);
    info->rowsize = ((info->depth * info->width) / 32) * 4;

    info->data = malloc(info->height * info->rowsize);
    fseek(file, bfh.offset, SEEK_SET);
    ret = fread(info->data, 1, (info->height * info->rowsize), file);
    if (ret < 1) {
        fprintf(stderr, "image buffer may be wrong \n");
    }

    /* use first pixel for background */
    uint8_t *p = info->data + info->size - info->rowsize;
    info->red = *(p+2);
    info->green = *(p+1);
    info->blue = *(p);

    return 0;
}

int fblogo_draw_image(char *fb, struct bmp_img_info *info)
{
    uint32_t x= 0, y = 0;
    char *fbp = fb;

    /* only 32 bit images supported */
    if (info->depth != 32) {
        fprintf(stderr, "Only 32 bit images supported \n");
        return -1;
    }

    fprintf(stdout, "Image size %d \n", info->size);
    fprintf(stdout, "Image offset %d \n", info->offset);
    fprintf(stdout, "Image height %d \n", info->height);
    fprintf(stdout, "Image width %d \n", info->width);
    fprintf(stdout, "Image depth %d \n", info->depth);
    fprintf(stdout, "Image scanline %d \n", info->rowsize);

    /* bitmap is stored from bottom to top, left to right, grab the last row */
    uint8_t *prow = info->data + info->size - info->rowsize;

    for (y = 0; y < info->height; y++) {
        uint8_t *p = prow;
        for (x = 0; x < info->width; x++) {
            *(fbp) = *(p);
            *(fbp+1) = *(p+1);
            *(fbp+2) = *(p+2);
            p += (info->depth / 8);
            fbp += 4;
        }
        prow -= info->rowsize;
    }

    return 0;
}

int fblogo_draw_color(char *fb, struct bmp_img_info *info, char r, char g, char b)
{
    uint32_t x= 0, y = 0;
    char *fbp = fb;

    for (y = 0; y < info->height; y++) {
        for (x = 0; x < info->width; x++) {

            /* color only */
            *(fbp) = b;     // Blue
            *(fbp+1) = g;   // Green
            *(fbp+2) = r;   // Red
            *(fbp+3) = 0x0; // Transparency

            /* increment pointers */
            fbp += info->depth/8;
        }
        fbp += 3200 - info->rowsize;
    }
    
    return 0;
}
