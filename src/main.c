#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fblogo.h"

int main(int argc, char **argv) 
{
    char                *path = "/data/splash.bmp";
    FILE                *bmp_file = NULL;
    struct fblogo_info  *binfo = NULL;
    
    if (argc == 2) {
        path = argv[1];
    }
    
    fprintf(stdout, "using %s for logo \n",path);
    
    bmp_file = fopen(path, "rb+");
  
    if (bmp_file == NULL) {
        perror("bitmap open");
        exit(-2);
    }
    
    binfo = fblogo_fb_new();
    if (binfo == NULL) {
        fprintf(stderr, "Failed to allocate fblogo_info \n");
        return -1;
    }
    
    fblogo_prepare_image(bmp_file, binfo);
    
    fprintf(stdout, "Image size %d \n", binfo->img_size);
    fprintf(stdout, "Image offset %d \n", binfo->img_offset);
    fprintf(stdout, "Image height %d \n", binfo->img_height);
    fprintf(stdout, "Image width %d \n", binfo->img_width);
    fprintf(stdout, "Image depth %d \n", binfo->img_depth);
    fprintf(stdout, "Image scanline %d \n", binfo->img_rowsize);
    
    fblogo_draw_image(binfo);
    
    fblogo_fb_destroy(binfo);
    
    fclose(bmp_file);
    
    return 0;
}

