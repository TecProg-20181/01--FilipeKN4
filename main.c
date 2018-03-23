#include <stdio.h>

typedef struct pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct image {
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;

Image read_image(Image image) {

  // read type of image
  char p3[4];
  scanf("%s", p3);

  // read width height and color of image
  int max_color;
  scanf("%u %u %d", &image.width, &image.height, &max_color);

  // read all pixels of image
  for (unsigned int i = 0; i < image.height; ++i) {
      for (unsigned int j = 0; j < image.width; ++j) {
          scanf("%hu %hu %hu", &image.pixel[i][j].r,
                               &image.pixel[i][j].g,
                               &image.pixel[i][j].b);

      }
  }

  return image;

}

int min(int a, int b) {
  if (a > b)
    return b;
  return a;
}

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

Pixel calculate_average_pixel(Pixel pixel) {
    unsigned short int sum = pixel.r + pixel.g + pixel.b;

    Pixel average = {};
    average.r = sum/3;
    average.g = sum/3;
    average.b = sum/3;

    return average;
}

Image apply_grayscale_filter(Image image) {

    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            Pixel average = calculate_average_pixel(image.pixel[i][j]);
            image.pixel[i][j] = average;
        }
    }

    return image;
}

Pixel sum_blur_filter(Pixel pixel, Pixel sum) {

  sum.r += pixel.r;
  sum.g += pixel.g;
  sum.b += pixel.b;

  return sum;
}

Pixel calculate_average_area(Pixel average, int size) {

  average.r /= size * size;
  average.g /= size * size;
  average.b /= size * size;

  return average;
}

Image apply_blur_filter(Image image) {
    int size = 0;
    scanf("%d", &size);

    for (unsigned short int i = 0; i < image.height; ++i) {
        for (unsigned short int j = 0; j < image.width; ++j) {
            Pixel sum = {0, 0, 0};

            int min_height = min(image.height - 1, i + size/2);
            int min_width = min(image.width - 1, j + size/2);
            for(int x = max(0, i - size/2); x <= min_height; ++x) {
                for(int y = max(0, j - size/2); y <= min_width; ++y) {
                    sum = sum_blur_filter(image.pixel[x][y], sum);
                }
            }
            image.pixel[i][j] = calculate_average_area(sum, size);

        }
    }

    return image;
}

Image rotate_right(Image image) {
    Image rotated_image;

    rotated_image.width = image.height;
    rotated_image.height = image.width;

    for (unsigned int i = 0, y = 0; i < rotated_image.height; ++i, ++y) {
        for (int j = rotated_image.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated_image.pixel[i][j] = image.pixel[x][y];
        }
    }

    return rotated_image;
}

Image apply_rotation_filter(Image image) {
    int rotation_number = 0;
    scanf("%d", &rotation_number);
    rotation_number %= 4;

    for (int j = 0; j < rotation_number; ++j) {
        image = rotate_right(image);
  }

  return image;
}

Pixel sub_max_color(Pixel pixel) {
    pixel.r = 255 - pixel.r;
    pixel.g = 255 - pixel.g;
    pixel.b = 255 - pixel.b;

  return pixel;
}

Image apply_color_invert_filter(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            image.pixel[i][j] = sub_max_color(image.pixel[i][j]);
        }
    }
    return image;
}

Image apply_image_cut_filter(Image image) {
    Image cropped_image;

    int x, y;
    scanf("%d %d", &x, &y);
    int width, height;
    scanf("%d %d", &width, &height);

    cropped_image.width = width;
    cropped_image.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped_image.pixel[i][j] = image.pixel[i + y][j + x];
        }
    }

    return cropped_image;
}

int calculate_min_value(double a, double b, double c, Pixel pixel) {
  int p =  pixel.r * a + pixel.g * b + pixel.b * c;
  int min_value = min(255, p);

  return min_value;
}

Image apply_sepia_filter(Image image) {
  for (unsigned int x = 0; x < image.height; ++x) {
      for (unsigned int j = 0; j < image.width; ++j) {
          Pixel pixel;
          pixel = image.pixel[x][j];

          image.pixel[x][j].r = calculate_min_value(0.393, 0.769, 0.189, pixel);
          image.pixel[x][j].g = calculate_min_value(0.349, 0.686, 0.168, pixel);
          image.pixel[x][j].b = calculate_min_value(0.272, 0.534, 0.131, pixel);
      }
  }

  return image;
}

Image apply_vertical_mirroring_filter(Image image) {
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = image.width, height = image.height;

  if (horizontal == 1) {
      width /= 2;
  } else {
      height /= 2;
  }

  for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
          int x = i, y = j;

          if (horizontal == 1) {
              y = image.width - 1 - j;
          } else {
              x = image.height - 1 - i;
          }

          Pixel aux1;
          aux1 = image.pixel[i][j];
          image.pixel[i][j] = image.pixel[x][y];
          image.pixel[x][y] = aux1;
      }
  }

  return image;
}

void show_image(Image image) {
    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", image.width, image.height);

    // print pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j].r,
                                   image.pixel[i][j].g,
                                   image.pixel[i][j].b);

        }
        printf("\n");
    }
}

int main() {
    Image image;

    image = read_image(image);

    int options;
    scanf("%d", &options);

    for(int i = 0; i < options; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Grayscale filter
                image = apply_grayscale_filter(image);
                break;
            }
            case 2: { // Sepia filter
                image = apply_sepia_filter(image);
                break;
            }
            case 3: { // Blur filter
                image = apply_blur_filter(image);
                break;
            }
            case 4: { // Rotation filter
                image = apply_rotation_filter(image);
                break;
            }
            case 5: { // Vertical mirrorin filter
                image = apply_vertical_mirroring_filter(image);
                break;
            }
            case 6: { // Color invert filter
                image = apply_color_invert_filter(image);
                break;
            }
            case 7: { // Image cut filter
                image = apply_image_cut_filter(image);
                break;
            }
        }

    }
    show_image(image);

    return 0;
}
