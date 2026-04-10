#include <stdio.h>
#include "Image.h"

int getMaxBrightness(int img[HEIGHT][WIDTH]) {
    int max = img[0][0];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (img[i][j] > max) {
                max = img[i][j];
            }
        }
    }

    return max;
}

int main() {
    int result;

    result = getMaxBrightness(image);

    printf("Max brightness = %d\n", result);

    return 0;
}
