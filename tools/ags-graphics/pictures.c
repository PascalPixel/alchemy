/*
 * pictures.c
 *
 * Encode the picture containers used by fonts, menus, and effects.
 * Each family owns its dimensions, picture order, offset table, and compression.
 * The dispatcher reads one PNG; family encoders borrow that Image.
 */
#include "resource.h"

/*
 * Metadata supplies the glyph range and the rows kept from each 16x16 cell.
 * Glyph codes are hex, as the character map writes them; the first row kept,
 * the number of rows and the widths are decimal.
 */
static Buffer
font(const char *input, const Image *image)
{
	char *name = copyString(input), *dot = strrchr(name, '.');
	if (!dot)
		fail("font input has no suffix");
	strcpy(dot, ".tsv");
	Buffer data = readFile(name);
	free(name);
	Strings lines = linesOf(&data);
	size_t first = SIZE_MAX, top = SIZE_MAX, rows = SIZE_MAX, index = 0;
	Buffer out = {0};
	if (image->width < 16 || image->width % 16 || image->height % 16)
		fail("font needs whole 16x16 glyph cells");
	for (size_t i = 0; i < lines.len; i++)
	{
		Strings f = split(lines.items[i], '\t');
		if ((!strcmp(f.items[0], "first") || !strcmp(f.items[0], "top") ||
			 !strcmp(f.items[0], "rows")) &&
			f.len != 2)
			fail("font metadata row needs one value");
		if (!strcmp(f.items[0], "first"))
			first = number(f.items[1], 16, SIZE_MAX);
		else if (!strcmp(f.items[0], "top"))
			top = number(f.items[1], 10, 16);
		else if (!strcmp(f.items[0], "rows"))
			rows = number(f.items[1], 10, 16);
		else
		{
			if (first == SIZE_MAX || top > 16 || rows > 16 || top + rows > 16 ||
				number(f.items[0], 16, SIZE_MAX) != first + index)
				fail("invalid font glyph order or dimensions");
			size_t columns = image->width / 16, x = index % columns * 16,
				   y = index / columns * 16;
			if (y + 16 > image->height)
				fail("glyph lies outside font sheet");
			for (size_t j = 1; j < f.len; j++)
				half(&out, number(trim(f.items[j]), 10, 65535));
			for (size_t dy = 0; dy < rows; dy++)
			{
				unsigned value = 0;
				for (size_t dx = 0; dx < 16; dx++)
					value |=
						(image->pixels
							 .data[(y + top + dy) * image->width + x + dx] != 0)
						<< (15 - dx);
				half(&out, value);
			}
			index++;
		}
		freeStrings(&f);
	}
	freeStrings(&lines);
	free(data.data);
	return out;
}

/*
 * Glyph rows use bit zero for the leftmost pixel.
 */
static Buffer
glyphBitmap(const Image *image)
{
	Buffer out = {0};
	if (image->width != 8)
		fail("glyphs must be eight pixels wide");
	for (size_t y = 0; y < image->height; y++)
	{
		unsigned v = 0;
		for (size_t x = 0; x < 8; x++)
			v |= (image->pixels.data[y * 8 + x] != 0) << x;
		byte(&out, v);
	}

	return out;
}

/*
 * Monochrome tiles use bit seven for the leftmost pixel, unlike glyph rows.
 */
static Buffer
monochromeTiles(const Image *image)
{
	Buffer out = {0};
	if (image->width % 8 || image->height % 8)
		fail("1bpp requires whole 8x8 tiles");
	for (size_t y = 0; y < image->height; y += 8)
		for (size_t x = 0; x < image->width; x += 8)
			for (size_t dy = 0; dy < 8; dy++)
			{
				unsigned v = 0;
				for (size_t dx = 0; dx < 8; dx++)
					v = v << 1 |
						(image->pixels.data[(y + dy) * image->width + x + dx] !=
						 0);
				byte(&out, v);
			}

	return out;
}

/*
 * Offsets stay in picture order although odd pictures are packed before even
 * pictures. An all-255 picture has an offset but contributes no payload.
 */
static Buffer
effectPictures(const Image *image)
{
	Buffer out = {0};
	if (image->height % image->width)
		fail("pictures must be square and stacked");
	size_t count = image->height / image->width,
		   size = image->width * image->width;
	for (size_t i = 0; i <= count; i++)
		word(&out, 0);
	for (unsigned parity = 0; parity < 2; parity++)
		for (size_t i = parity ? 0 : 1; i < count; i += 2)
		{
			write32(out.data + 4 * i, (uint32_t) out.len);
			Buffer picture = {image->pixels.data + i * size, size, size};
			bool blank = true;
			for (size_t p = 0; p < size; p++)
				blank &= picture.data[p] == 255;
			if (!blank)
			{
				Buffer packed = compressZeros(&picture);
				append(&out, packed.data, packed.len);
				free(packed.data);
			}
		}

	return out;
}

/*
 * Each 32x32 icon is compressed independently and padded to 32 bytes.
 */
static Buffer
menuIcons(const Image *image)
{
	Buffer out = {0};
	if (image->width != 32 || image->height % 32)
		fail("icons must be 32x32 and stacked");
	size_t count = image->height / 32;
	for (size_t i = 0; i < count; i++)
		half(&out, 0);
	for (size_t i = 0; i < count; i++)
	{
		if (out.len > 65535)
			fail("icon bank is too large");
		write16(out.data + 2 * i, (unsigned) out.len);
		Image icon = *image;
		icon.height = 32;
		icon.pixels.data += i * 1024;
		icon.pixels.len = 1024;
		Buffer raw = tiles(&icon, 8, 8, 8),
			   packed = compressLz(&raw, &raw, "plz", packer);
		alignBuffer(&packed, 32);
		append(&out, packed.data, packed.len);
		free(raw.data);
		free(packed.data);
	}

	return out;
}

/*
 * The first offset selects the palette; each later offset selects an
 * independently compressed eight-pixel-high strip.
 */
static Buffer
pictureStrips(const Image *image)
{
	Buffer out = {0};
	if (image->width % 8 || image->height % 8)
		fail("strip picture requires whole 8x8 tiles");
	size_t bands = image->height / 8;
	for (size_t i = 0; i <= bands; i++)
		word(&out, 0);
	write32(out.data, (uint32_t) out.len);
	Buffer palette = gbaPalette(image);
	append(&out, palette.data, palette.len);
	free(palette.data);
	for (size_t i = 0; i < bands; i++)
	{
		write32(out.data + 4 * (i + 1), (uint32_t) out.len);
		Image strip = *image;
		strip.height = 8;
		strip.pixels.data += i * 8 * image->width;
		strip.pixels.len = 8 * image->width;
		Buffer raw = tiles(&strip, 8, 8, 8),
			   packed = compressLz(&raw, &raw, "lz", packer);
		append(&out, packed.data, packed.len);
		free(raw.data);
		free(packed.data);
	}

	return out;
}

Buffer
imageForm(const char *input, const char *form, const Buffer *data)
{
	Image image = readImage(data);
	Buffer out = {0};
	unsigned depth;
	size_t mw, mh;
	if (tileForm(form, &depth, &mw, &mh))
		out = tiles(&image, depth, mw, mh);
	else if (!strcmp(form, "gbapal"))
		out = gbaPalette(&image);
	else if (!strcmp(form, "bitmap"))
		append(&out, image.pixels.data, image.pixels.len);
	else if (!strcmp(form, "icon4"))
		out = compressIcon(&image.pixels);
	else if (!strcmp(form, "font"))
		out = font(input, &image);
	else if (!strcmp(form, "glyphs"))
		out = glyphBitmap(&image);
	else if (!strcmp(form, "1bpp"))
		out = monochromeTiles(&image);
	else if (!strcmp(form, "pictures"))
		out = effectPictures(&image);
	else if (!strcmp(form, "icons"))
		out = menuIcons(&image);
	else if (!strcmp(form, "strips"))
		out = pictureStrips(&image);
	else
		fail("unknown image form: %s", form);
	freeImage(&image);
	return out;
}
