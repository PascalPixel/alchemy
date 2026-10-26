/*
 * resource.c
 *
 * Assemble resource recipes and apply their selected compression.
 * Tables, pictures, mapped parts, and sprite banks own their format details.
 * This module resolves sibling assets, combines parts, and validates any
 * documented literal edits made after packing.
 */
#include "resource.h"
#include <ctype.h>

/*
 * Lowercase resource names match the asset tree, relative to the recipe's own
 * folder (`../MAP_000/GRID_METATILE` reaches a neighbouring map's file).
 */
char *
sibling(const char *input, const char *name)
{
	char *lower = copyString(name);
	for (char *p = lower; *p; p++)
		*p = tolower((unsigned char) *p);
	char *path = beside(input, lower);
	free(lower);
	return path;
}

Buffer
readSibling(const char *input, const char *name)
{
	char *path = sibling(input, name);
	Buffer b = readFile(path);
	free(path);
	return b;
}

/*
 * Discard blank records for list-like formats. Spatial planes deliberately
 * bypass this helper because an empty row still has a position.
 */
Strings
linesOf(const Buffer *input)
{
	Strings all = split((char *) input->data, '\n'), lines = {0};
	for (size_t i = 0; i < all.len; i++)
		if (*trim(all.items[i]))
			addString(&lines, all.items[i]);
	freeStrings(&all);
	return lines;
}

static bool
dataForm(const char *form)
{
	return !strncmp(form, "delta", 5) || !strcmp(form, "table") ||
		   !strcmp(form, "plane") || !strcmp(form, "grid") || !strcmp(form, "parts") ||
		   !strcmp(form, "icons4") || !strcmp(form, "blocks") ||
		   !strcmp(form, "script") || !strcmp(form, "sprites") ||
		   !strcmp(form, "spriteblocks");
}

/*
 * Parts may compose ordinary pictures and tables. Containers and fonts have
 * their own metadata and cannot be nested as plain parts.
 */
static Buffer
parts(const char *input, const Buffer *data, unsigned depth)
{
	Strings lines = linesOf(data);
	Buffer out = {0};
	for (size_t i = 0; i < lines.len; i++)
	{
		Strings f = split(lines.items[i], '\t');
		for (size_t j = 0; j < f.len; j++)
		{
			char *v = copyString(trim(f.items[j]));
			free(f.items[j]);
			f.items[j] = v;
		}
		if (f.len < 2)
			fail("part needs a name and form");
		const char *form = f.items[1];
		if ((dataForm(form) && strcmp(form, "table") &&
			 strcmp(form, "plane")) ||
			!strcmp(form, "font"))
			fail("part must be an image or table");
		char *name = formatString("%s.%s", f.items[0],
								  dataForm(form) ? "tsv" : "png"),
			 *path = sibling(input, name);
		free(name);
		Buffer part = f.len == 2 ? buildResource(path, form, NULL, depth + 1)
								 : mappedPart(input, path, form, &f);
		append(&out, part.data, part.len);
		free(part.data);
		free(path);
		freeStrings(&f);
	}
	freeStrings(&lines);
	return out;
}

/*
 * A run of frames, NAME_00.png, NAME_01.png and on: each frame's pixels in
 * turn, for an animation whose frames differ in size and so share no sheet.
 */
static Buffer
frameRun(const char *input)
{
	size_t stem = strlen(input);
	if (stem < 7 || strcmp(input + stem - 7, "_00.png"))
		fail("a frame run starts at NAME_00.png");
	stem -= 7;
	Buffer out = {0};
	for (unsigned i = 0; i < 100; i++)
	{
		char *path = formatString("%.*s_%02u.png", (int) stem, input, i);
		if (!exists(path))
		{
			free(path);
			break;
		}
		Image image = readImageFile(path);
		append(&out, image.pixels.data, image.pixels.len);
		freeImage(&image);
		free(path);
	}
	return out;
}

/*
 * Absent slots retain zero offsets; present slots include their own palette.
 */
static Buffer
iconBank(const char *input, const Buffer *data)
{
	Strings lines = linesOf(data), slots = {0};
	size_t align = 4;
	for (size_t i = 0; i < lines.len; i++)
	{
		if (!strncmp(lines.items[i], "align\t", 6))
		{
			align = number(trim(lines.items[i] + 6), 16, 65536);
			if (!align || align & (align - 1))
				fail("icon alignment is not a power of two");
		}
		else
			addString(&slots, lines.items[i]);
	}
	Buffer out = {0};
	for (size_t i = 0; i < slots.len; i++)
		half(&out, 0);
	for (size_t i = 0; i < slots.len; i++)
		if (strcmp(slots.items[i], "-"))
		{
			if (out.len > 65535)
				fail("icon bank is too large");
			write16(out.data + 2 * i, (unsigned) out.len);
			char *name = formatString("%s.png", slots.items[i]);
			Buffer png = readSibling(input, name);
			free(name);
			Image image = readImage(&png);
			free(png.data);
			if (image.width != 32 || image.height != 32 ||
				image.palette.len != 48)
				fail("icon must be 32x32 in sixteen colours");
			Buffer palette = gbaPalette(&image),
				   packed = compressIcon(&image.pixels);
			append(&out, palette.data, palette.len);
			append(&out, packed.data, packed.len);
			alignBuffer(&out, align);
			free(palette.data);
			free(packed.data);
			freeImage(&image);
		}
	freeStrings(&lines);
	freeStrings(&slots);
	return out;
}

/* Preserve documented edits made after packing. The edited asset supplies
 * the shipped bytes; its patch supplies only the packer's neutral input. */
static Buffer
patched(const char *input, const Buffer *data)
{
	const char *base = strrchr(input, '/');
	base = base ? base + 1 : input;
	const char *dot = strchr(base, '.');
	char *name = formatString(
			 "%.*s_patch.tsv",
			 (int) (dot ? dot - base : (ptrdiff_t) strlen(base)), base),
		 *path = sibling(input, name);
	free(name);
	Buffer result = {0};
	append(&result, data->data, data->len);
	if (exists(path))
	{
		Buffer patch = readFile(path);
		Strings lines = linesOf(&patch);
		if (lines.len < 2 ||
			strcmp(lines.items[0], "position\tpack\tship\tproof"))
			fail("invalid packing patch header");
		size_t previous = 0;
		for (size_t i = 1; i < lines.len; i++)
		{
			Strings f = split(lines.items[i], '\t');
			if (f.len != 4 || !*trim(f.items[3]))
				fail("packing patch needs four fields and proof");
			size_t at = number(trim(f.items[0]), 10, SIZE_MAX);
			unsigned pack = decimalOrHex(trim(f.items[1]), 255),
					 ship = decimalOrHex(trim(f.items[2]), 255);
			if (at >= data->len || (i > 1 && at <= previous) ||
				ship != data->data[at] || pack == ship)
				fail("packing patch disagrees with asset");
			result.data[at] = pack;
			previous = at;
			freeStrings(&f);
		}
		freeStrings(&lines);
		free(patch.data);
	}
	free(path);
	return result;
}

Buffer
buildResource(const char *input, const char *form, const char *codec,
			  unsigned depth)
{
	if (depth > 32)
		fail("asset nesting is too deep");
	Buffer data = readFile(input), raw = {0};
	if (!strcmp(form, "table"))
		raw = table(&data);
	else if (!strcmp(form, "plane"))
		raw = plane(&data);
	else if (!strcmp(form, "grid"))
		raw = grid(&data);
	else if (!strcmp(form, "script"))
		raw = script(&data);
	else if (!strcmp(form, "blocks"))
		raw = blocks(&data);
	else if (!strcmp(form, "parts"))
		raw = parts(input, &data, depth);
	else if (!strcmp(form, "icons4"))
		raw = iconBank(input, &data);
	else if (!strcmp(form, "bitmaps"))
		raw = frameRun(input);
	else if (!strcmp(form, "sprites") || !strcmp(form, "spriteblocks"))
		raw = spriteSource(input, !strcmp(form, "spriteblocks"));
	else if (!strncmp(form, "delta", 5))
	{
		Buffer t = table(&data);
		raw = deltaMap(&t, number(form + 5, 10, 2));
		free(t.data);
	}
	else
		raw = imageForm(input, form, &data);
	free(data.data);
	if (!codec)
		return raw;
	Buffer out;
	if (!strcmp(codec, "d7"))
		out = compressDelta7(&raw);
	else if (!strcmp(codec, "hlz"))
		out = compressHalfwords(&raw);
	else
	{
		/* A frame run's patch is NAME_patch.tsv, not NAME_00's. */
		char *name = !strcmp(form, "bitmaps")
						 ? formatString("%.*s.png", (int) (strlen(input) - 7), input)
						 : copyString(input);
		Buffer pack = patched(name, &raw);
		free(name);
		out = compressLz(&raw, &pack, codec, packer);
		free(pack.data);
	}
	free(raw.data);
	return out;
}
