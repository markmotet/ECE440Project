#include <stdio.h>
#include <stdlib.h>

typedef struct __attribute__((__packed__)) {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct __attribute__((__packed__)) {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;

int main(void) {                                                                                                                                                                                                                             
    BITMAPFILEHEADER fh;                                                                                                                                                                                                                           
    BITMAPINFOHEADER ih;                                                                                                                                                                                                                           
    FILE *img = fopen("terraria.bmp", "rb");
    fread(&fh, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), img);
    fread(&ih, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), img);


    printf("---------- BMP METADATA ----------\n");
    printf("FILE SIZE:   %u bytes\n", fh.bfSize);                                                                         
    printf("DATA OFFSET: %u bytes\n", fh.bfOffBits);                                                                         
    printf("WIDTH:       %d px\n", ih.biWidth);
    printf("HEIGHT:      %d px\n", ih.biHeight);
    printf("----------------------------------\n");
    return 0;
}