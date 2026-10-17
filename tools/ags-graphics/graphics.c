/*
 * graphics.c
 *
 * Palette conversion and tile traversal shared by encoding and redrawing.
 * PNG pixels are row-major bytes. Tile streams visit each metatile, its 8x8
 * tiles, then each tile's pixel rows. Keep that ordering in one place.
 */
#include "graphics.h"

/*
 * Hardware colors keep five bits per channel. Reject colors that would lose
 * precision rather than silently quantizing an edited palette.
 */
Buffer
gbaPalette(const Image *i)
{
	Buffer out = {0};
	for (size_t n = 0; n < i->alpha.len; n++)
		if (i->alpha.data[n] != 255)
			fail("GBA palettes cannot represent transparent PNG entries");
	for (size_t n = 0; n < i->palette.len; n++)
		if (i->palette.data[n] & 7)
			fail("palette channels must be multiples of eight");
	for (size_t n = 0; n < i->palette.len; n += 3)
		half(&out, (i->palette.data[n] >> 3) |
					   (i->palette.data[n + 1] >> 3) << 5 |
					   (i->palette.data[n + 2] >> 3) << 10);
	return out;
}

/*
 * Explicit sprite dimensions must be one of the twelve hardware shapes.
 */
bool
tileForm(const char *form, unsigned *depth, size_t *width, size_t *height)
{
	if (strncmp(form, "4bpp", 4) && strncmp(form, "8bpp", 4))
		return false;
	*depth = form[0] - '0';
	*width = *height = 8;
	if (!form[4])
		return true;
	unsigned w, h;
	char extra;
	if (sscanf(form + 4, "%ux%u%c", &w, &h, &extra) != 2)
		return false;
	const unsigned shapes[][2] = {{8, 8},  {16, 16}, {32, 32}, {64, 64},
								  {16, 8}, {32, 8},	 {32, 16}, {64, 32},
								  {8, 16}, {8, 32},	 {16, 32}, {32, 64}};
	for (size_t i = 0; i < 12; i++)
		if (shapes[i][0] == w && shapes[i][1] == h)
		{
			*width = w;
			*height = h;
			return true;
		}
	return false;
}

/* Each stream row is eight pixels: metatile, tile, then row within the tile. */
size_t
tileRowOffset(const Image *image, size_t width, size_t height, size_t row)
{
	size_t rowsPerMetatile = width * height / 8;
	size_t metatile = row / rowsPerMetatile;
	size_t tileRow = row % rowsPerMetatile;
	size_t tile = tileRow / 8;
	size_t metatilesAcross = image->width / width;
	size_t tilesAcross = width / 8;
	size_t left = (metatile % metatilesAcross) * width;
	size_t top = (metatile / metatilesAcross) * height;
	size_t x = left + (tile % tilesAcross) * 8;
	size_t y = top + (tile / tilesAcross) * 8 + tileRow % 8;

	return y * image->width + x;
}

Buffer
tiles(const Image *image, unsigned depth, size_t width, size_t height)
{
	if (!width || !height || width % 8 || height % 8 || image->width % width ||
		image->height % height)
		fail("image does not contain whole metatiles");
	Buffer out = {0};

	for (size_t row = 0; row < image->width * image->height / 8; row++)
	{
		size_t offset = tileRowOffset(image, width, height, row);
		const uint8_t *pixels = image->pixels.data + offset;

		for (size_t x = 0; x < 8; x += depth == 4 ? 2 : 1)
		{
			unsigned value = pixels[x];
			if (depth == 4)
				value = (value & 15) | ((pixels[x + 1] & 15) << 4);
			byte(&out, value);
		}
	}
	return out;
}
