/*
 * graphics.h
 *
 * Indexed images, tile layouts, and graphics tool entry points. Image buffers
 * are owned together and released with freeImage.
 */
#ifndef AGS_GRAPHICS_H
#define AGS_GRAPHICS_H
#include "../shared/compress.h"
typedef struct
{
	size_t width, height;
	Buffer pixels, palette, alpha;
	unsigned depth;
} Image;
Image readImage(const Buffer *png);
Image readImageFile(const char *path);
Buffer writeImage(const Image *image);
void freeImage(Image *image);
Buffer gbaPalette(const Image *image);
bool tileForm(const char *form, unsigned *depth, size_t *width, size_t *height);
Buffer tiles(const Image *image, unsigned depth, size_t width, size_t height);
char *sibling(const char *input, const char *name);
size_t tileRowOffset(const Image *image, size_t width, size_t height,
					 size_t row);
Buffer buildResource(const char *input, const char *form, const char *codec,
					 unsigned depth);
Buffer spriteSource(const char *input, bool blocks);
Buffer redraw(const char *input, const char *from, const char *to, size_t wide,
			  const char *palette);
#endif
