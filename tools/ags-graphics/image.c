/*
 * image.c
 *
 * Read and write indexed PNG images without changing palette indices.
 * The Image owns unpacked pixels, RGB palette entries, and optional alpha
 * entries. Reject true-color input and partial transparency so editing cannot
 * silently change the meaning of a pixel or palette pen.
 */
#include "graphics.h"
#include <png.h>
#include <ctype.h>

typedef struct
{
	const Buffer *buffer;
	size_t at;
} PngInput;
static void
pngError(png_structp png, png_const_charp error)
{
	(void) png;
	fail("PNG: %s", error);
}

static void
pngWarning(png_structp png, png_const_charp error)
{
	(void) png;
	fprintf(stderr, "PNG: %s\n", error);
}

static void
pngRead(png_structp png, png_bytep bytes, png_size_t count)
{
	PngInput *in = png_get_io_ptr(png);
	if (count > in->buffer->len - in->at)
		fail("truncated PNG");
	memcpy(bytes, in->buffer->data + in->at, count);
	in->at += count;
}

static void
pngWrite(png_structp png, png_bytep bytes, png_size_t count)
{
	append(png_get_io_ptr(png), bytes, count);
}

static void
pngFlush(png_structp png)
{
	(void) png;
}
/*
 * Unpack sub-byte pixels while preserving their palette indices. Transparency
 * may hide a pen completely, but must not require blending.
 */
Image
readImage(const Buffer *data)
{
	PngInput in = {data, 0};
	Image image = {0};
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
											 pngError, pngWarning);
	if (!png)
		fail("cannot create PNG reader");
	png_infop info = png_create_info_struct(png);
	if (!info)
		fail("cannot create PNG metadata");
	png_set_read_fn(png, &in, pngRead);
	png_read_info(png, info);
	image.width = png_get_image_width(png, info);
	image.height = png_get_image_height(png, info);
	image.depth = png_get_bit_depth(png, info);
	if (!image.width || !image.height ||
		png_get_color_type(png, info) != PNG_COLOR_TYPE_PALETTE ||
		image.depth > 8)
		fail("PNG must use an indexed 1/2/4/8-bit palette");
	png_colorp palette;
	int colors;
	if (!png_get_PLTE(png, info, &palette, &colors) || colors < 1 ||
		colors > 256 || colors > (1 << image.depth))
		fail("invalid PNG palette");
	for (int i = 0; i < colors; i++)
	{
		byte(&image.palette, palette[i].red);
		byte(&image.palette, palette[i].green);
		byte(&image.palette, palette[i].blue);
	}
	png_bytep alpha;
	int alphaCount;
	png_color_16p transparent;
	if (png_get_tRNS(png, info, &alpha, &alphaCount, &transparent))
	{
		if (alphaCount > colors)
			fail("transparency exceeds palette");
		for (int i = 0; i < alphaCount; i++)
		{
			if (alpha[i] != 0 && alpha[i] != 255)
				fail("transparency must contain only binary alpha");
			byte(&image.alpha, alpha[i]);
		}
	}
	png_set_packing(png);
	png_set_interlace_handling(png);
	png_read_update_info(png, info);
	if (image.height > SIZE_MAX / image.width)
		fail("PNG dimensions overflow");
	size_t n = image.width * image.height;
	reserve(&image.pixels, n);
	image.pixels.len = n;
	png_bytep *rows = allocate(image.height, sizeof(png_bytep));
	for (size_t y = 0; y < image.height; y++)
		rows[y] = image.pixels.data + y * image.width;
	png_read_image(png, rows);
	png_read_end(png, info);
	free(rows);
	png_destroy_read_struct(&png, &info, NULL);
	for (size_t i = 0; i < n; i++)
		if (image.pixels.data[i] >= colors)
			fail("pixel references a missing palette entry");
	return image;
}

/*
 * Rows borrow the Image's unpacked pixels; libpng performs the packing.
 */
Buffer
writeImage(const Image *image)
{
	Buffer out = {0};
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,
											  pngError, pngWarning);
	if (!png)
		fail("cannot create PNG writer");
	png_infop info = png_create_info_struct(png);
	if (!info)
		fail("cannot create PNG metadata");
	png_set_write_fn(png, &out, pngWrite, pngFlush);
	png_set_IHDR(png, info, (png_uint_32) image->width,
				 (png_uint_32) image->height, image->depth,
				 PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_color palette[256];
	size_t colors = image->palette.len / 3;
	for (size_t i = 0; i < colors; i++)
		palette[i] = (png_color){image->palette.data[3 * i],
								 image->palette.data[3 * i + 1],
								 image->palette.data[3 * i + 2]};
	png_set_PLTE(png, info, palette, (int) colors);
	if (image->alpha.len)
		png_set_tRNS(png, info, image->alpha.data, (int) image->alpha.len,
					 NULL);
	png_write_info(png, info);
	png_set_packing(png);
	for (size_t y = 0; y < image->height; y++)
		png_write_row(png, image->pixels.data + y * image->width);
	png_write_end(png, info);
	png_destroy_write_struct(&png, &info);
	return out;
}

void
freeImage(Image *image)
{
	free(image->pixels.data);
	free(image->palette.data);
	free(image->alpha.data);
	*image = (Image){0};
}

Image
readImageFile(const char *path)
{
	Buffer data = readFile(path);
	Image image = readImage(&data);
	free(data.data);
	return image;
}
