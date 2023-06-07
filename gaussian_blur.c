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

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;

// Create a Gaussian kernel.
// For simplicity we are going to create a 3x3 kernel.
float kernel[3][3] = {
    {1.0/16, 2.0/16, 1.0/16},
    {2.0/16, 4.0/16, 2.0/16},
    {1.0/16, 2.0/16, 1.0/16}
};

int main(void) {
    BITMAPFILEHEADER fh;                                                                                                                                                                                                                           
    BITMAPINFOHEADER ih;  
    RGB *pixelData;

    FILE *img = fopen("terraria_24bit.bmp", "rb");
    fread(&fh, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), img);
    fread(&ih, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), img);

    // Allocate memory for the pixel data
    pixelData = (RGB *)malloc(sizeof(RGB) * ih.biWidth * ih.biHeight);

    // Read pixel data
    fread(pixelData, sizeof(RGB), ih.biWidth * ih.biHeight, img);
    fclose(img);

    // Allocate memory for the new pixel data
    RGB *newPixelData = (RGB *)malloc(sizeof(RGB) * ih.biWidth * ih.biHeight);

    // Apply the Gaussian kernel to each pixel
    for(int y = 0; y < ih.biHeight; y++) {
        for(int x = 0; x < ih.biWidth; x++) {
            float r = 0, g = 0, b = 0;
            for(int ky = -1; ky <= 1; ky++) {
                for(int kx = -1; kx <= 1; kx++) {
                    int px = x + kx;
                    int py = y + ky;

                    // Ensure the pixel is within the bounds of the image
                    if(px >= 0 && px < ih.biWidth && py >= 0 && py < ih.biHeight) {
                        r += pixelData[py * ih.biWidth + px].r * kernel[ky+1][kx+1];
                        g += pixelData[py * ih.biWidth + px].g * kernel[ky+1][kx+1];
                        b += pixelData[py * ih.biWidth + px].b * kernel[ky+1][kx+1];
                    }
                }
            }

            newPixelData[y * ih.biWidth + x].r = (unsigned char) r;
            newPixelData[y * ih.biWidth + x].g = (unsigned char) g;
            newPixelData[y * ih.biWidth + x].b = (unsigned char) b;
        }
    }

    // Write the blurred image to a new file
    FILE *output = fopen("blurred.bmp", "wb");
    fwrite(&fh, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), output);
    fwrite(&ih, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), output);
    fwrite(newPixelData, sizeof(RGB), ih.biWidth * ih.biHeight, output);
    fclose(output);

    // Free the memory we allocated
    free(pixelData);
    free(newPixelData);

    return 0;
}