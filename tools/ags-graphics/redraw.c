/*
 * redraw.c
 *
 * Rearrange a tile stream into an editable indexed PNG sheet.
 * Input parts contribute tiles in recipe order. Output shape and row width
 * change only their placement; pixel indices and the selected palette survive
 * the conversion.
 */
#include "graphics.h"

/*
 * The first picture supplies the output palette unless one was selected
 * explicitly. Later parts must agree before their tile streams are joined.
 */
static void
appendPicture(Buffer *stream, Image *palette, const char *path,
			  const char *form, unsigned depth, bool explicitPalette)
{
	unsigned d;
	size_t w, h;
	if (!tileForm(form, &d, &w, &h) || d != depth)
		fail("redraw forms must have the same depth");
	Image image = readImageFile(path);
	if (!palette->palette.len)
	{
		palette->palette = image.palette;
		image.palette = (Buffer){0};
		palette->alpha = image.alpha;
		image.alpha = (Buffer){0};
		palette->depth = image.depth;
	}
	else if (!explicitPalette &&
			 (palette->palette.len != image.palette.len ||
			  memcmp(palette->palette.data, image.palette.data,
					 palette->palette.len) ||
			  palette->alpha.len != image.alpha.len ||
			  (palette->alpha.len &&
			   memcmp(palette->alpha.data, image.alpha.data,
					  palette->alpha.len))))
		fail("part palettes differ; specify --palette");
	Buffer pixels = tiles(&image, 8, w, h);
	append(stream, pixels.data, pixels.len);
	free(pixels.data);
	freeImage(&image);
}

Buffer
redraw(const char *input, const char *from, const char *to, size_t wide,
	   const char *palettePath)
{
	unsigned depth;
	size_t w, h;
	if (!to || !tileForm(to, &depth, &w, &h))
		fail("redrawing needs --to with a tile form");
	Buffer stream = {0};
	Image image = {0};
	bool parts = !strcmp(extension(input), "tsv");
	if (palettePath)
	{
		if (!parts)
			fail("--palette is for part lists");
		image = readImageFile(palettePath);
		free(image.pixels.data);
		image.pixels = (Buffer){0};
	}
	if (!parts)
	{
		if (!from)
			fail("redrawing a PNG needs --from");
		appendPicture(&stream, &image, input, from, depth, false);
	}
	else
	{
		if (from)
			fail("a part list names its own forms");
		Buffer data = readFile(input);
		Strings lines = split((char *) data.data, '\n');
		for (size_t i = 0; i < lines.len; i++)
		{
			if (!*trim(lines.items[i]))
				continue;
			Strings f = split(lines.items[i], '\t');
			if (f.len != 2)
				fail("redrawing requires plain tile parts");
			char *name = formatString("%s.png", trim(f.items[0])),
				 *path = sibling(input, name);
			free(name);
			appendPicture(&stream, &image, path, trim(f.items[1]), depth,
						  palettePath != NULL);
			free(path);
			freeStrings(&f);
		}
		freeStrings(&lines);
		free(data.data);
	}
	if (!stream.len || stream.len % (w * h))
		fail("tiles are not whole sprites");
	size_t sprites = stream.len / (w * h);
	if (wide == SIZE_MAX)
		wide = sprites;
	if (!wide || sprites % wide)
		fail("sprites do not fill the requested rows");
	image.width = wide * w;
	image.height = sprites / wide * h;
	reserve(&image.pixels, stream.len);
	image.pixels.len = stream.len;

	for (size_t row = 0; row < stream.len / 8; row++)
	{
		size_t offset = tileRowOffset(&image, w, h, row);
		memcpy(image.pixels.data + offset, stream.data + row * 8, 8);
	}
	for (size_t i = 0; i < stream.len; i++)
		if (stream.data[i] >= image.palette.len / 3 ||
			stream.data[i] >= (1u << image.depth))
			fail("redraw palette cannot represent source pixels");
	Buffer result = writeImage(&image);
	free(stream.data);
	freeImage(&image);
	return result;
}
