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
    printf("type = %c, , bfS = %u, un1 = %hu, un2 = %hu, iDO = %u\n", fh.bfType, fh.bfSize, fh.bfReserved1, fh.bfReserved2, fh.bfOffBits);                                                                         
    printf("w = %d, h = %d\n", ih.biWidth, ih.biHeight);
    return 0;
}