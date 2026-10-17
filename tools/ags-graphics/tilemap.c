/*
 * tilemap.c
 *
 * Recover a tile run from an editable drawing and its tilemap.
 * Repeated map entries must agree after undoing flips. Animated drawings append
 * otherwise hidden tiles below the visible map, keeping every tile editable.
 */
#include "resource.h"

/* Shared map entries must draw identical pixels after undoing their flips. */
static void
copyTile(Image *run, const Image *drawing, size_t left, size_t top, size_t tile,
		 bool horizontalFlip, bool verticalFlip, bool repeated)
{
	for (size_t y = 0; y < 8; y++)
		for (size_t x = 0; x < 8; x++)
		{
			size_t sourceX = left + (horizontalFlip ? 7 - x : x);
			size_t sourceY = top + (verticalFlip ? 7 - y : y);
			uint8_t pixel =
				drawing->pixels.data[sourceY * drawing->width + sourceX];

			if (tile == SIZE_MAX)
			{
				if (pixel)
					fail("unmapped cell is inked");
				continue;
			}
			size_t destination = y * run->width + tile * 8 + x;
			if (repeated && run->pixels.data[destination] != pixel)
				fail("tile is drawn two ways");
			run->pixels.data[destination] = pixel;
		}
}

/*
 * Find the visible bounds of the requested tile range, then recover each
 * frame into a complete horizontal run, including any hidden tiles.
 */
Buffer
mappedPart(const char *input, const char *path, const char *form,
		   const Strings *fields)
{
	if (fields->len != 5 && fields->len != 6)
		fail("mapped part needs MAP FIRST COUNT [frames]");
	if (strcmp(form, "4bpp") && strcmp(form, "8bpp"))
		fail("mapped part must be 4bpp or 8bpp");
	bool frames = fields->len == 6;
	if (frames && strcmp(fields->items[5], "frames"))
		fail("invalid mapped part option");
	size_t first = decimalOrHex(fields->items[3], 1023),
		   count = decimalOrHex(fields->items[4], 1024);
	if (!count)
		fail("empty tile range");
	char *name = formatString("%s.tsv", fields->items[2]);
	Buffer rawMap = readSibling(input, name);
	free(name);
	Strings lines = linesOf(&rawMap);
	if (!lines.len)
		fail("empty tilemap");
	Strings columns = split(lines.items[0], '\t');
	size_t wide = columns.len;
	for (size_t i = 0; i < wide; i++)
	{
		char *type = strrchr(columns.items[i], ':');
		if (!type || strcmp(type, ":u16"))
			fail("tilemap requires u16 columns");
	}
	Buffer map = table(&rawMap);
	if (map.len % (2 * wide))
		fail("tilemap has incomplete rows");
	size_t left = wide, right = 0, top = SIZE_MAX, bottom = 0;
	bool *shown = allocate(count, sizeof(bool));
	for (size_t i = 0; i < map.len / 2; i++)
	{
		size_t tile = read16(map.data + 2 * i) & 1023;
		if (tile < first || tile - first >= count)
			continue;
		shown[tile - first] = true;
		if (i % wide < left)
			left = i % wide;
		if (i % wide > right)
			right = i % wide;
		if (i / wide < top)
			top = i / wide;
		if (i / wide > bottom)
			bottom = i / wide;
	}
	if (top == SIZE_MAX)
		fail("tilemap shows no tiles from the run");
	size_t w = right + 1 - left, h = bottom + 1 - top, hidden = 0;
	for (size_t i = 0; i < count; i++)
		hidden += !shown[i];
	size_t rows = h + (frames ? (hidden + w - 1) / w : 0);
	Image image = readImageFile(path);
	if (image.width != w * 8 || image.height % (rows * 8) ||
		(!frames && image.height != rows * 8))
		fail("tilemap drawing dimensions differ");
	Buffer out = {0};
	for (size_t frame = 0; frame < image.height / (rows * 8); frame++)
	{
		Image run = {count * 8, 8, {0}, {0}, {0}, 8};
		reserve(&run.pixels, count * 64);
		run.pixels.len = count * 64;
		memset(run.pixels.data, 0, run.pixels.len);
		bool *seen = allocate(count, sizeof(bool));
		size_t hiddenSlot = 0;
		for (size_t cell = 0; cell < w * rows; cell++)
		{
			size_t tile = SIZE_MAX;
			bool hf = false, vf = false;
			if (cell < w * h)
			{
				unsigned entry = read16(
					map.data + 2 * ((top + cell / w) * wide + left + cell % w));
				if ((entry & 1023) >= first && (entry & 1023) - first < count)
					tile = (entry & 1023) - first;
				hf = entry & 1024;
				vf = entry & 2048;
			}
			else
			{
				while (hiddenSlot < count && shown[hiddenSlot])
					hiddenSlot++;
				if (hiddenSlot < count)
					tile = hiddenSlot++;
			}
			size_t sourceX = cell % w * 8;
			size_t sourceY = (frame * rows + cell / w) * 8;
			copyTile(&run, &image, sourceX, sourceY, tile, hf, vf,
					 tile != SIZE_MAX && seen[tile]);
			if (tile != SIZE_MAX)
				seen[tile] = true;
		}
		Buffer packed = tiles(&run, form[0] - '0', 8, 8);
		append(&out, packed.data, packed.len);
		free(packed.data);
		free(run.pixels.data);
		free(seen);
	}
	free(shown);
	freeImage(&image);
	free(map.data);
	free(rawMap.data);
	freeStrings(&columns);
	freeStrings(&lines);
	return out;
}
