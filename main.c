#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(void) {
    int width, height, channels;
    unsigned char *img = stbi_load("image.jpg", &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    // convert image to matrix
    int img_size = width * height * channels;
    unsigned char *img_matrix = malloc(img_size);
    if(img_matrix == NULL) {
        printf("Error allocating memory for image matrix\n");
        exit(1);
    }
    for(int i = 0; i < img_size; i++) {
        img_matrix[i] = img[i];
    }

    int choice;
    printf("Enter 1 for encryption or 2 for decryption: ");
    scanf("%d", &choice);

    int P = 3, Q = 2, n = width;

    // apply 2D Arnold cat map encryption/decryption to each pixel
    double x, y;
    for(unsigned char *p = img_matrix; p != img_matrix + img_size; p += channels) {
        x = *p;
        y = *(p + 1);
        if(choice == 1) {
          //encryption
          *p = (unsigned char)(fmod((1*x + P*y), n));
          *(p + 1) = (unsigned char)(fmod((Q*x + (P*Q+1)*y), n));
        } else if(choice == 2) {
          //decryption
          *p = (unsigned char)(fmod(((P*Q+1)*x - P*y), n));
          *(p + 1) = (unsigned char)(fmod((-Q*x + 1*y), n));
        }
    }

    // save the resulting image
    if(stbi_write_jpg("image_arnold.jpg", width, height, channels, img_matrix, 100)) {
        printf("Successfully saved the image with 2D Arnold cat map applied\n");
    } else {
        printf("Error saving the image\n");
    }

    // free memory
    free(img_matrix);
    stbi_image_free(img);

    return 0;
}
