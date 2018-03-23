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

Image ler_imagem(Image img) {

  // read type of image
  char p3[4];
  scanf("%s", p3);

  // read width height and color of image
  int max_color;
  scanf("%u %u %d", &img.width, &img.height, &max_color);

  // read all pixels of image
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j].r,
                               &img.pixel[i][j].g,
                               &img.pixel[i][j].b);

      }
  }

  return img;

}

int max(int a, int b) {
  if (a > b)
    return b;
  return a;
}

int media_escala_de_cinza(int argumento1, int argumento2, int argumento3) {
    int soma = argumento1 + argumento2 + argumento3;

    return soma/3;
}

Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            print("%u", img.pixel[i][j].r + img.pixel[i][j].g + img.pixel[i][j].b);
        }
    }*/

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = media_escala_de_cinza(img.pixel[i][j].r,
                                              img.pixel[i][j].g,
                                              img.pixel[i][j].b);

            img.pixel[i][j].r = media;
            img.pixel[i][j].g = media;
            img.pixel[i][j].b = media;
        }
    }

    return img;
}

Image blur(Image img) {
    int T = 0;
    scanf("%d", &T);

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_height = (img.height - 1 > i + T/2) ? i + T/2 : img.height - 1;
            int min_width = (img.width - 1 > j + T/2) ? j + T/2 : img.width - 1;
            for(unsigned int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_height; ++x) {
                for(unsigned int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_width; ++y) {
                    media.r += img.pixel[x][y].r;
                    media.g += img.pixel[x][y].g;
                    media.b += img.pixel[x][y].b;
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            img.pixel[i][j].r = media.r;
            img.pixel[i][j].g = media.g;
            img.pixel[i][j].b = media.b;
        }
    }

    return img;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j].r = img.pixel[x][y].r;
            rotacionada.pixel[i][j].g = img.pixel[x][y].g;
            rotacionada.pixel[i][j].b = img.pixel[x][y].b;
        }
    }

    return rotacionada;
}

Image aplicar_rotacao(Image img) {
    int quantas_vezes = 0;
    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;

    for (int j = 0; j < quantas_vezes; ++j) {
        img = rotacionar90direita(img);
  }

  return img;
}

Image inverter_cores(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].r = 255 - img.pixel[i][j].r;
            img.pixel[i][j].g = 255 - img.pixel[i][j].g;
            img.pixel[i][j].b = 255 - img.pixel[i][j].b;
        }
    }
    return img;
}

Image cortar_imagem(Image img) {
    Image cortada;

    int x, y;
    scanf("%d %d", &x, &y);
    int width, height;
    scanf("%d %d", &width, &height);

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j].r = img.pixel[i + y][j + x].r;
            cortada.pixel[i][j].g = img.pixel[i + y][j + x].g;
            cortada.pixel[i][j].b = img.pixel[i + y][j + x].b;
        }
    }

    return cortada;
}

int calcular_menor_r(double a, double b, double c, Pixel pixel) {
  int p =  pixel.r * a + pixel.g * b + pixel.b * c;
  //int menor_r = (255 >  p) ? p : 255;
  int menor_r = max(255, p);

  return menor_r;
}

Image filtro_sepia(Image img) {
  for (unsigned int x = 0; x < img.height; ++x) {
      for (unsigned int j = 0; j < img.width; ++j) {
          Pixel pixel;
          pixel.r = img.pixel[x][j].r;
          pixel.g = img.pixel[x][j].g;
          pixel.b = img.pixel[x][j].b;

          img.pixel[x][j].r = calcular_menor_r(0.393, 0.769, 0.189, pixel);
          img.pixel[x][j].g = calcular_menor_r(0.349, 0.686, 0.168, pixel);
          img.pixel[x][j].b = calcular_menor_r(0.272, 0.534, 0.131, pixel);
      }
  }

  return img;
}

Image espelhamento_vertical(Image img) {
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = img.width, height = img.height;

  if (horizontal == 1) width /= 2;
  else height /= 2;

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
          int x = i2, y = j;

          if (horizontal == 1) y = img.width - 1 - j;
          else x = img.height - 1 - i2;

          Pixel aux1;
          aux1.r = img.pixel[i2][j].r;
          aux1.g = img.pixel[i2][j].g;
          aux1.b = img.pixel[i2][j].b;

          img.pixel[i2][j].r = img.pixel[x][y].r;
          img.pixel[i2][j].g = img.pixel[x][y].g;
          img.pixel[i2][j].b = img.pixel[x][y].b;

          img.pixel[x][y].r = aux1.r;
          img.pixel[x][y].g = aux1.g;
          img.pixel[x][y].b = aux1.b;
      }
  }

  return img;
}

void mostrar_imagem(Image img) {
    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].r,
                                   img.pixel[i][j].g,
                                   img.pixel[i][j].b);

        }
        printf("\n");
    }
}

int main() {
    Image img;

    img = ler_imagem(img);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = filtro_sepia(img);
                break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = aplicar_rotacao(img);
                break;
            }
            case 5: { // Espelhamento
                img = espelhamento_vertical(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = cortar_imagem(img);
                break;
            }
        }

    }
    mostrar_imagem(img);

    return 0;
}
