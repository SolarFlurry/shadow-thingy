#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <spng.h>

#include "render/render.h"

const size_t IMAGE_WIDTH = 200;
const size_t IMAGE_HEIGHT = 200;

bool endsWith(const char *str, const char *suffix) {
    if (!str || !suffix) {
        return false;
    }

    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);

    if (lensuffix > lenstr) {
        return false;
    }

    return strcmp(str + (lenstr - lensuffix), suffix) == 0;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Error: Invalid arguments\n");
		return 1;
	}

	FILE* imageFile;

	size_t imageSize = IMAGE_WIDTH * IMAGE_HEIGHT * 4;
	unsigned char* image = malloc(imageSize);

	if (image == NULL) {
		printf("Error: Failed to allocate image\n");
		return 0;
	}

	for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
		image[i * 4 + 0] = 255;
		image[i * 4 + 1] = 255;
		image[i * 4 + 2] = 255;
		image[i * 4 + 3] = 255;
	}
	
	render(image, IMAGE_WIDTH, IMAGE_HEIGHT);
	
	if (endsWith(argv[1], ".png")) {
		imageFile = fopen(argv[1], "wb");
		if (imageFile == NULL) {
			printf("Error: Cannot open file '%s'\n", argv[1]);
			return 1;
		}
		int ret;

		struct spng_ihdr ihdr = {0};
		ihdr.width = IMAGE_WIDTH;
		ihdr.height = IMAGE_HEIGHT;
		ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
		ihdr.bit_depth = 8;

		spng_ctx* ctx = spng_ctx_new(SPNG_CTX_ENCODER);
		ret = spng_set_png_file(ctx, imageFile);
		if (ret) {
			printf("Error: set_png_file\n");
			return 1;
		}
		ret = spng_set_ihdr(ctx, &ihdr);
		if (ret) {
			printf("Error: set_ihdr\n");
			return 1;
		}
		ret = spng_encode_image(ctx, image, imageSize, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
		if (ret) {
			printf("Error: encode_image - %s\n", spng_strerror(ret));
			return 1;
		}
		spng_ctx_free(ctx);
	} else {
		imageFile = fopen(argv[1], "w");
		if (imageFile == NULL) {
			printf("Error: Cannot open file '%s'\n", argv[1]);
			return 1;
		}
		fprintf(imageFile, "P3\n%lu %lu\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
		for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
			fprintf(imageFile, "%d %d %d\n", image[i * 4 + 0], image[i * 4 + 1], image[i * 4  +2]);
		}
	}

	fclose(imageFile);
	free(image);
	return 0;
}