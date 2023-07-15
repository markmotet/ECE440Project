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


// We are using a Gaussian kernel for image blurring. The kernel originally contained floating point values.
// To improve performance, we use fixed-point arithmetic by converting these floating point values to integers.
// We chose unsigned short as the data type and scaled the original floating point values by a factor of 2^16 (or 65536).
// This allows us to represent the fractional part of the original floating point values in the lower 16 bits of the unsigned short.

unsigned short kernel[3][3] = {
    {4096, 8192, 4096},
    {8192, 16384, 8192},
    {4096, 8192, 4096}
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
            // We are accumulating the result of multiplying the pixel values (unsigned char) with the kernel values (unsigned short).
            // This operation could result in a value that exceeds the maximum value that can be stored in an unsigned char, causing an overflow.
            // To prevent this, we use unsigned int for r, g, and b which can hold larger values.

            unsigned int r = 0, g = 0, b = 0;  // Change to unsigned int
            
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

            // After applying the kernel, we divide the accumulated values by the scale factor (2^16) to get back to the original scale.
            // This is done by right shifting the values by 16 bits.
            // The result is then cast back to unsigned char when storing it in the newPixelData array.


            newPixelData[y * ih.biWidth + x].r = (unsigned char) (r >> 16);
            newPixelData[y * ih.biWidth + x].g = (unsigned char) (g >> 16);
            newPixelData[y * ih.biWidth + x].b = (unsigned char) (b >> 16);
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
