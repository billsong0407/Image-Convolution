/* Bill Song - 400172156 - songb10 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pixel {
  unsigned char r, g, b;
} Pixel;

typedef struct image {
  unsigned int width, height, max;
  Pixel **data;
} Image;

typedef struct kernel {
  unsigned int size, scale;
  int **matrix;
} Kernel;

Kernel *readKernel(char *file_name) {
  int i, j;
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Unable to open file\%s\n", file_name);
    return NULL;
  }
  Kernel *kernelFile = malloc(sizeof(Kernel));
  fscanf(file, "%d\n", &kernelFile->size);
  fscanf(file, "%d\n", &kernelFile->scale);

  kernelFile->matrix = malloc(sizeof(Pixel *) * kernelFile->size);
  for (i = 0; i < kernelFile->size; i++)
    kernelFile->matrix[i] = malloc(sizeof(Pixel) * kernelFile->size);
  for (i = 0; i < kernelFile->size; i++) {
    for (j = 0; j < kernelFile->size; j++) {
      fscanf(file, "%d\n", &kernelFile->matrix[i][j]);
    }
  }
  return kernelFile;
}
/* Credit: readPPM source code is from Caleb Mech in T03 */
Image *readPPM(char *file_name) {
  int i, j;
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Unable to open file\%s\n", file_name);
    return NULL;
  }
  char format[3];
  fscanf(file, "%s\n", format);
  if (strcmp(format, "P3"))
    return NULL;
  Image *image = malloc(sizeof(Image));
  if (fscanf(file, "%u %u %u", &image->width, &image->height, &image->max) != 3)
    return NULL;
  image->data = malloc(sizeof(Pixel *) * image->height);
  for (i = 0; i < image->height; i++)
    image->data[i] = malloc(sizeof(Pixel) * image->width);
  for (i = 0; i < image->height; i++) {
    for (j = 0; j < image->width; j++) {
      int pixel_read = fscanf(file, "%hhu %hhu %hhu", &(image->data[i][j].r),
                              &(image->data[i][j].g), &(image->data[i][j].b));
      if (pixel_read != 3)
        return NULL;
    }
  }
  fclose(file);
  return image;
}
/* Credit: writePPM source code is from Caleb Mech in T03 */
int writePPM(char *file_name, Image *image) {
  int i, j;
  FILE *file = fopen(file_name, "w");
  if (!file) {
    fprintf(stderr, "Unable to open file\%s\n", file_name);
    return -1;
  }

  fprintf(file, "P3\n");
  fprintf(file, "%u %u\n", image->width, image->height);
  fprintf(file, "%u\n", image->max);

  for (i = 0; i < image->height; i++) {
    for (j = 0; j < image->width; j++)
      fprintf(file, "%u %u %u ", image->data[i][j].r, image->data[i][j].g,
              image->data[i][j].b);
    fprintf(file, "\n");
  }
  fclose(file);
  return 0;
}

Pixel getPixelValue(int a, int b, int scale, int divisor, Image *image) {
  Pixel new;
  if (a < 0 || a > (image->height) - 1 || b < 0 || b > (image->width) - 1) {
    new.r = 0;
    new.g = 0;
    new.b = 0;
  } else {
    new.r = ((((image->data[a][b].r) - '0') * scale) + '0');
    new.g = ((((image->data[a][b].g)- '0') * scale) + '0');
    new.b = ((((image->data[a][b].b)- '0') * scale) + '0');
  }
  return new;
}

Image *convolution(Image *image, Kernel *kernel) {
  int a = (kernel->size) / 2, redValue, greenValue, blueValue;
  int row, column, i, j, ri = 0, rj = 0;
  Pixel new, sum;
  Image *imageAfter = malloc(sizeof(Image));
  imageAfter->width = image->width;
  imageAfter->height = image->height;
  imageAfter->max = image->max;
  imageAfter->data = malloc(sizeof(Pixel *) * image->height);
  for (i = 0; i < image->height; i++)
    imageAfter->data[i] = malloc(sizeof(Pixel) * image->width);
  for (i = 0; i < image->height; i++) {
    for (j = 0; j < image->width; j++) {
      ri = 0;
      for (row = (i - a); row <= (i + a); row++, ri++) {
        rj = 0;
        for (column = (j - a); column <= (j + a); column++, rj++) {
          new = getPixelValue(row, column, kernel->matrix[ri][rj],
                              kernel->scale, image);
          sum.r = ((sum.r - '0') + (new.r - '0'))+'0';
          sum.g = ((sum.g - '0') + (new.g - '0'))+'0';
          sum.b = ((sum.b - '0') + (new.b - '0'))+'0';
        }
      }
      redValue = sum.r / kernel->scale;
      greenValue = sum.g / kernel->scale;
      blueValue = sum.b / kernel->scale;
      if (redValue < 0)
        redValue = 0 + '0';
      if (redValue > 255)
        redValue = 255 + '0';
      if (greenValue < 0)
        greenValue = 0 + '0';
      if (greenValue > 255)
        greenValue = 255 + '0';
      if (blueValue < 0)
        blueValue = 0 + '0';
      if (blueValue > 255)
        blueValue = 255 + '0';
      imageAfter->data[i][j].r = redValue;
      imageAfter->data[i][j].g = greenValue;
      imageAfter->data[i][j].b = blueValue;
      sum.r = 0 + '0';
      sum.g = 0 + '0';
      sum.b = 0 + '0';
    }
  }
  return imageAfter;
}

int main(int argc, char **argv) {
  Image *imageBefore, *imageAfter;
  Kernel *kernelFile;
  if (argc != 4) {
    printf("Incorrect number of inputs: input.ppm kernel output.ppm\n");
    return -1;
  }
  char *inputFileName = argv[1];
  char *outputFileName = argv[3];
  char *kernelFileName = argv[2];
  imageBefore = readPPM(inputFileName);
  if (!imageBefore)
    return -1;
  kernelFile = readKernel(kernelFileName);
  imageAfter = convolution(imageBefore, kernelFile);
  writePPM(outputFileName, imageAfter);
  return 0;
}