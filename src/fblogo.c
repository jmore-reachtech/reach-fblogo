#include "fblogo.h"

struct fblogo_info *fblogo_fb_new()
{
    struct fb_var_screeninfo    fb_var;
    struct fb_fix_screeninfo    fb_fix;
    long int                    screensize  = 0;
    char                        fbdev[9]    = "/dev/fb0";
    struct fblogo_info          *fbinfo     = NULL;
    
    if ((fbinfo = malloc(sizeof(struct fblogo_info))) == NULL) {
      fprintf(stderr, "Error no memory \n");
      return NULL;
    }
    
    memset(fbinfo, 0, sizeof(struct fblogo_info));
    
    if ((fbinfo->fd = open(fbdev, O_RDWR)) < 0){
        fprintf(stderr, "Error opening %s\n", fbdev);
        return NULL;
    }
    
    if (ioctl(fbinfo->fd, FBIOGET_VSCREENINFO, &fb_var) == -1) {
        perror ("Error getting variable framebuffer info");
        return NULL;
    }
    
    if (ioctl(fbinfo->fd, FBIOGET_FSCREENINFO, &fb_fix) == -1) {
        perror ("Error getting fixed framebuffer info");
        return NULL;
    }
    
    fbinfo->fb_width = fb_var.xres;
    fbinfo->fb_height = fb_var.yres;
    fbinfo->fb_depth = fb_var.bits_per_pixel;
    
    screensize = ((fbinfo->fb_width * fbinfo->fb_height * fbinfo->fb_depth) / 8);
    fprintf(stdout, "screensize is %d \n", screensize);
    
    fbinfo->base = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbinfo->fd, 0);
    if (fbinfo->base == MAP_FAILED) {
        perror ("mmap failed \n");
        return NULL;
    }
  
    return fbinfo;
}

void fblogo_fb_destroy(struct fblogo_info *info)
{
    long int                    screensize  = 0;
    
    if (info->base != NULL) {
        screensize = ((info->fb_width * info->fb_height * info->fb_depth) / 8);
        munmap(info->base, screensize);
    }
    
    if (info->data != NULL) {
        free(info->data);
    }
}

int fblogo_prepare_image(FILE *file, struct fblogo_info *info)
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
    info->img_offset = bfh.offset;
    info->img_size = (bfh.filesz - bfh.offset);
    
    fseek(file,18,SEEK_SET);
    fread(&bih.width,4,1,file);
    info->img_width = bih.width;

    fseek(file,22,SEEK_SET);
    fread(&bih.height,4,1,file);
    info->img_height = bih.height;

    fseek(file,26,SEEK_SET);
    fread(&bih.nplanes,2,1,file);

    fseek(file,28,SEEK_SET);
    fread(&bih.depth,2,1,file);
    info->img_depth = bih.depth;

    fseek(file,30,SEEK_SET);
    fread(&bih.compress_type,4,1,file);

    info->data = malloc(info->img_size);
    info->img_rowsize = ((info->img_depth * info->img_width) / 32) * 4;

    info->data = malloc(info->img_height * info->img_rowsize);
    fseek(file, bfh.offset, SEEK_SET);
    ret = fread(info->data, 1, (info->img_height * info->img_rowsize), file);
    if (ret < 1) {
        fprintf(stderr, "image buffer may be wrong \n");
    }

    /* use first pixel for background */
    uint8_t *p = info->data + info->img_size - info->img_rowsize;
    info->img_red = *(p+2);
    info->img_green = *(p+1);
    info->img_blue = *(p);

    return 0;
}

int fblogo_draw_image(struct fblogo_info *info)
{
    uint32_t x= 0, y = 0;
    char *fbp = info->base;

    /* only 24 bit images supported */
    if (info->img_depth != 24) {
        fprintf(stderr, "Only 24 bit images supported \n");
        return -1;
    }

    /* bitmap is stored from bottom to top, left to right, grab the last row */
    uint8_t *prow = info->data + info->img_size - info->img_rowsize;

    for (y = 0; y < info->img_height; y++) {
        uint8_t *p = prow;
        for (x = 0; x < info->img_width; x++) {
            *(fbp) = *(p);
            *(fbp+1) = *(p+1);
            *(fbp+2) = *(p+2);
            p += (info->img_depth / 8);
            fbp += 4;
        }
        prow -= info->img_rowsize;
    }

    return 0;
}

int fblogo_draw_color(struct fblogo_info *info, char r, char g, char b)
{
    uint32_t x= 0, y = 0;
    char *fbp = info->base;

    for (y = 0; y < info->img_height; y++) {
        for (x = 0; x < info->img_width; x++) {

            /* color only */
            *(fbp) = b;     // Blue
            *(fbp+1) = g;   // Green
            *(fbp+2) = r;   // Red
            *(fbp+3) = 0x0; // Transparency

            /* increment pointers */
            fbp += info->img_depth/8;
        }
        fbp += 3200 - info->img_rowsize;
    }
    
    return 0;
}
