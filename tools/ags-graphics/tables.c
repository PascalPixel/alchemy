/*
 * tables.c
 *
 * Encode typed records, sparse planes, animation scripts, and block grids.
 * These are distinct TSV formats: column types describe record widths, while
 * planes and blocks retain spatial positions even when cells are omitted.
 */
#include <ctype.h>
#include "resource.h"

/*
 * A strN column holds a quoted string of at most N bytes, written in full and
 * padded with zeros: "Vermin", "" or "\xff", with \\, \" and \xNN escapes.
 */
static void
text(Buffer *out, const char *s, unsigned size)
{
	size_t len = strlen(s), start = out->len;
	if (len < 2 || s[0] != '"' || s[len - 1] != '"')
		fail("table string is not quoted");
	for (size_t i = 1; i < len - 1; i++)
	{
		unsigned c = (unsigned char) s[i];
		if (c == '\\')
		{
			char e = i + 1 < len - 1 ? s[++i] : 0;
			if (e == 'x' && i + 2 < len - 1)
			{
				char digits[3] = {s[i + 1], s[i + 2], 0};
				c = (unsigned) number(digits, 16, 255);
				i += 2;
			}
			else if (e == '\\' || e == '"')
				c = (unsigned char) e;
			else
				fail("unknown table string escape");
		}
		else if (c == '"')
			fail("table string has an unescaped quote");
		if (out->len - start >= size)
			fail("table string is longer than its column");
		byte(out, c);
	}
	while (out->len - start < size)
		byte(out, 0);
}

/*
 * A final short record is allowed; once encountered, no later row may follow.
 * A column named without a type is a label for people, such as a name, and
 * writes nothing.
 */
/*
 * The type of a column that is known by its name alone, or nothing.
 */
static char *
typeOf(const char *name)
{
	static const char *const BYTES[] = {"shape", "height0", "height1",
										"height2", "id", "flags"};
	static const char *const SIGNED[] = {"left", "top", "right", "bottom"};
	static const char *const HALVES[] = {"top_left", "top_right", "bottom_left",
										 "bottom_right", "x", "z"};
	for (size_t i = 0; i < sizeof BYTES / sizeof *BYTES; i++)
		if (!strcmp(name, BYTES[i]))
			return "u8";
	for (size_t i = 0; i < sizeof SIGNED / sizeof *SIGNED; i++)
		if (!strcmp(name, SIGNED[i]))
			return "s8";
	for (size_t i = 0; i < sizeof HALVES / sizeof *HALVES; i++)
		if (!strcmp(name, HALVES[i]))
			return "u16";
	if (!strcmp(name, "y"))
		return "u16";
	if (!strcmp(name, "dx") || !strcmp(name, "dy"))
		return "s16";
	/* A numbered column, x00 or attr3, has the type of its letters. */
	size_t letters = strcspn(name, "0123456789");
	if (!name[letters] || name[letters + strspn(name + letters, "0123456789")])
		return NULL;
	if (letters == 1 && (*name == 'x' || *name == 't'))
		return "u16";
	if (letters == 1 && *name == 'w')
		return "u32";
	if ((letters == 1 && *name == 'b') || !strncmp(name, "attr", letters) ||
		!strncmp(name, "cell", letters))
		return "u8";
	return NULL;
}

Buffer
table(const Buffer *input)
{
	Strings lines = linesOf(input);
	if (!lines.len)
		fail("table names no columns");
	Strings columns = split(lines.items[0], '\t');
	Buffer out = {0};
	bool shortRow = false;
	for (size_t row = 1; row < lines.len; row++)
	{
		Strings fields = split(lines.items[row], '\t');
		if (shortRow || fields.len > columns.len)
			fail("table record does not fit its columns");
		shortRow = fields.len < columns.len;
		for (size_t i = 0; i < fields.len; i++)
		{
			char *type = strrchr(columns.items[i], ':');
			size_t named = strlen(columns.items[i]);
			/* A corner's flip (h, v or hv) and palette join the tile before. */
			if (named > 5 && !strcmp(columns.items[i] + named - 5, "_flip"))
			{
				if (out.len < 2)
					fail("a flip needs the tile before it");
				for (const char *c = trim(fields.items[i]); *c; c++)
					{
					if (*c != 'h' && *c != 'v')
						fail("a flip is h, v or hv");
					out.data[out.len - 1] |= *c == 'h' ? 4 : 8;
				}
				continue;
			}
			if (named > 8 && !strcmp(columns.items[i] + named - 8, "_palette"))
			{
				if (out.len < 2)
					fail("a palette needs the tile before it");
				out.data[out.len - 1] |=
					number(trim(fields.items[i]), 10, 15) << 4;
				continue;
			}
			type = type ? type + 1 : typeOf(columns.items[i]);
			if (!type)
				continue;
			if (!strncmp(type, "str", 3))
			{
				text(&out, trim(fields.items[i]),
					 number(type + 3, 10, 65535));
				continue;
			}
			bool sign = *type == 's';
			if (!sign && *type != 'u')
				fail("unknown table column type");
			unsigned bits = number(type + 1, 10, 32);
			if (bits != 8 && bits != 16 && bits != 32)
				fail("unknown table column width");
			char *s = trim(fields.items[i]);
			bool neg = *s == '-';
			if (neg)
				s++;
			uint64_t magnitude = decimalOrHex(s, UINT32_MAX),
					 limit = sign ? (UINT64_C(1) << (bits - 1)) - !neg
								  : (UINT64_C(1) << bits) - 1;
			if ((neg && !sign) || magnitude > limit)
				fail("table value does not fit its column");
			uint32_t value = neg ? (uint32_t) -magnitude : (uint32_t) magnitude;
			for (unsigned b = 0; b < bits; b += 8)
				byte(&out, value >> b);
		}
		freeStrings(&fields);
	}
	freeStrings(&columns);
	freeStrings(&lines);
	return out;
}

Buffer
plane(const Buffer *input)
{
	/* A header line naming the columns, then a row of bytes for each row. */
	Strings lines = linesOf(input);
	if (lines.len < 2)
		fail("a plane needs its header and a row");
	Strings header = split(lines.items[0], '\t');
	Buffer out = {0};
	for (size_t y = 1; y < lines.len; y++)
	{
		Strings row = split(lines.items[y], '\t');
		if (row.len != header.len)
			fail("a plane row is not as wide as its header");
		for (size_t x = 0; x < row.len; x++)
			byte(&out, number(trim(row.items[x]), 10, 255));
		freeStrings(&row);
	}
	freeStrings(&header);
	freeStrings(&lines);
	return out;
}

/*
 * Commands encode either a prefixed operand or a fixed count of halfwords.
 * `paused n` is `channel n` with the marker's bit 7 set, so n stays below 128.
 */
Buffer
script(const Buffer *input)
{
	Strings lines = linesOf(input);
	Buffer out = {0};
	for (size_t i = 0; i < lines.len; i++)
	{
		Strings f = split(lines.items[i], '\t');
		const char *key = f.items[0];
		size_t n;
		unsigned prefix = 0, operand = 255;
		if (!strcmp(key, "channel"))
		{
			n = 1;
			prefix = 0xfd00;
		}
		else if (!strcmp(key, "paused"))
		{
			n = 1;
			prefix = 0xfd80;
			operand = 127;
		}
		else if (!strcmp(key, "jump"))
		{
			n = 1;
			prefix = 0xfe00;
		}
		else if (!strcmp(key, "stop"))
		{
			n = 0;
			prefix = 0xfeff;
		}
		else if (!strcmp(key, "end"))
		{
			n = 0;
			prefix = 0xffff;
		}
		else if (!strcmp(key, "picture"))
			n = 4;
		else if (!strcmp(key, "control"))
			n = 1;
		else if (!strcmp(key, "level"))
			n = 2;
		else
			fail("unknown script command: %s", key);
		if (f.len != n + 1)
			fail("wrong number of script arguments");
		if (prefix)
			half(&out,
				 prefix | (n ? decimalOrHex(trim(f.items[1]), operand) : 0));
		else
			for (size_t j = 1; j < f.len; j++)
				half(&out, decimalOrHex(trim(f.items[j]), 65535));
		freeStrings(&f);
	}
	freeStrings(&lines);
	return out;
}

/*
 * A missing block omits all 256 cells. Offsets point to independently packed
 * blocks, while fully omitted blocks retain zero offsets.
 */
Buffer
blocks(const Buffer *input)
{
	Strings lines = linesOf(input);
	if (!lines.len || lines.len % 16)
		fail("block grid needs whole 16x16 blocks");
	Strings *rows = allocate(lines.len, sizeof(Strings));
	size_t width = 0;
	for (size_t y = 0; y < lines.len; y++)
	{
		rows[y] = split(lines.items[y], '\t');
		if (!y)
			width = rows[y].len;
		if (!width || width % 16 || rows[y].len != width)
			fail("block grid rows differ");
	}
	size_t count = width / 16 * (lines.len / 16);
	Buffer out = {0};
	for (size_t i = 0; i < count; i++)
		word(&out, 0);
	for (size_t i = 0; i < count; i++)
	{
		size_t x = i % (width / 16) * 16, y = i / (width / 16) * 16;
		Buffer values = {0};
		unsigned missing = 0;
		for (size_t dy = 0; dy < 16; dy++)
			for (size_t dx = 0; dx < 16; dx++)
			{
				char *s = trim(rows[y + dy].items[x + dx]);
				if (!strcmp(s, "-"))
					missing++;
				else
					word(&values, number(s, 16, UINT32_MAX));
			}
		if (missing && missing != 256)
			fail("block is omitted only in part");
		if (!missing)
		{
			write32(out.data + 4 * i, (uint32_t) out.len);
			Buffer packed = compressLz(&values, &values, "plz", packer);
			append(&out, packed.data, packed.len);
			free(packed.data);
		}
		free(values.data);
	}
	for (size_t i = 0; i < lines.len; i++)
		freeStrings(rows + i);
	free(rows);
	freeStrings(&lines);
	return out;
}

/*
 * A map's grid: three tables beside each other with one number a square,
 * grid_picture.tsv, which is the one named, grid_behavior.tsv and
 * grid_shape.tsv, or grid_picture_es.tsv and its two for an edition's own.
 * Each has a header line naming its columns and a row for every row of the
 * map. A picture's top four bits are the layer and priority flags over a
 * 12-bit cell picture; a behavior is a cell behavior and a shape an index
 * into the shape table. The game reads three planes, behaviors (u8), shapes
 * (u8) and cell pictures (u16), one after another.
 */
Buffer
grid(const char *pictures)
{
	static const char *const NAMES[] = {"grid_behavior", "grid_shape",
										"grid_picture"};
	const char *slash = strrchr(pictures, '/'),
			   *base = slash ? slash + 1 : pictures;
	if (strncmp(base, "grid_picture", 12))
		fail("a grid is built from its grid_picture.tsv");
	Buffer out = {0};
	size_t squares = 0;
	for (size_t k = 0; k < 3; k++)
	{
		char path[1024];
		snprintf(path, sizeof path, "%.*s%s%s", (int) (base - pictures),
				 pictures, NAMES[k], base + 12);
		Buffer text = readFile(path);
		Strings lines = linesOf(&text);
		if (lines.len < 2)
			fail("a grid needs its header and a row");
		Strings header = split(lines.items[0], '\t');
		if (!k)
			squares = header.len * (lines.len - 1);
		else if (squares != header.len * (lines.len - 1))
			fail("a map's three grids differ in size");
		for (size_t y = 1; y < lines.len; y++)
		{
			Strings row = split(lines.items[y], '\t');
			if (row.len != header.len)
				fail("a grid row is not as wide as its header");
			for (size_t x = 0; x < row.len; x++)
				if (k < 2)
					byte(&out, number(trim(row.items[x]), 10, 255));
				else
					half(&out, number(trim(row.items[x]), 10, 0xffff));
			freeStrings(&row);
		}
		freeStrings(&header);
		freeStrings(&lines);
		free(text.data);
	}
	return out;
}

/*
 * A background's tile map as the screen shows it: a `palette:N` header, then
 * one text row per row of the screen, one cell per 8x8 square, the tile's
 * number in decimal followed by `h` and `v` where it is flipped. Blank lines
 * may separate screens. Each cell becomes the hardware's halfword; the width,
 * when asked for, is the number of cells in a row.
 */
Buffer
tilemap(const Buffer *input, size_t *width)
{
	Strings lines = linesOf(input);
	if (!lines.len || strncmp(lines.items[0], "palette:", 8))
		fail("tile map needs a palette:N header");
	unsigned palette = number(trim(lines.items[0] + 8), 10, 15);
	size_t wide = 0;
	Buffer out = {0};
	for (size_t y = 1; y < lines.len; y++)
	{
		Strings row = words(lines.items[y]);
		if (y == 1)
			wide = row.len;
		if (row.len != wide)
			fail("tile map rows differ");
		for (size_t x = 0; x < row.len; x++)
		{
			char *cell = row.items[x];
			unsigned flips = 0;
			size_t len = strlen(cell);
			if (len && cell[len - 1] == 'v')
			{
				flips |= 2;
				cell[--len] = 0;
			}
			if (len && cell[len - 1] == 'h')
			{
				flips |= 1;
				cell[--len] = 0;
			}
			half(&out, number(cell, 10, 1023) | flips << 10 | palette << 12);
		}
		freeStrings(&row);
	}
	if (width)
		*width = wide;
	freeStrings(&lines);
	return out;
}

/*
 * The world map's collision tiles as the game reads them: each tile is 4x4
 * values of 4 bits, written as four words of four hex digits, one under the
 * other, each word a row of the tile. Tiles run left to right, then down;
 * blank lines are ignored. Each row packs into two bytes, the left value in
 * the low nibble.
 */
Buffer
collision(const Buffer *input)
{
	Strings lines = linesOf(input);
	if (!lines.len || lines.len % 4)
		fail("collision tiles need whole rows of four");
	Strings *rows = allocate(lines.len, sizeof(Strings));
	for (size_t y = 0; y < lines.len; y++)
	{
		rows[y] = words(lines.items[y]);
		if (!rows[y].len || rows[y].len != rows[0].len)
			fail("collision rows differ");
	}
	Buffer out = {0};
	for (size_t band = 0; band < lines.len; band += 4)
		for (size_t x = 0; x < rows[0].len; x++)
			for (size_t y = band; y < band + 4; y++)
			{
				const char *s = rows[y].items[x];
				if (strlen(s) != 4)
					fail("collision tile row needs four values");
				unsigned values[4];
				for (unsigned i = 0; i < 4; i++)
				{
					char digit[2] = {s[i], 0};
					values[i] = number(digit, 16, 15);
				}
				byte(&out, values[0] | values[1] << 4);
				byte(&out, values[2] | values[3] << 4);
			}
	for (size_t y = 0; y < lines.len; y++)
		freeStrings(rows + y);
	free(rows);
	freeStrings(&lines);
	return out;
}

/*
 * A map's scene: the points the map's events place things at, the entrances
 * and the camera's bounds, three tables beside each other: points.tsv, which
 * is the one named, entrances.tsv and bounds.tsv, or points_ja.tsv and its
 * two for an edition's own. The game reads a header first, the offsets of
 * the entrances, of the map's cell edits and of the bounds, then a zero
 * halfword; each table ends in a byte of 255, and the edits are an empty
 * list here, its end marker alone.
 */
Buffer
scene(const char *points)
{
	static const char *const NAMES[] = {"points", "entrances", "bounds"};
	const char *slash = strrchr(points, '/'), *base = slash ? slash + 1 : points;
	if (strncmp(base, "points", 6))
		fail("a scene is built from its points.tsv");
	Buffer parts[3];
	for (size_t i = 0; i < 3; i++)
	{
		char path[1024];
		snprintf(path, sizeof path, "%.*s%s%s", (int) (base - points), points,
				 NAMES[i], base + 6);
		Buffer text = readFile(path);
		parts[i] = table(&text);
		byte(&parts[i], 0xff);
		free(text.data);
	}
	size_t entrances = 8 + parts[0].len, edits = entrances + parts[1].len,
		   bounds = edits + 2;
	if (bounds > 0xffff)
		fail("a scene is too large for its header");
	Buffer out = {0};
	half(&out, (unsigned) entrances);
	half(&out, (unsigned) edits);
	half(&out, (unsigned) bounds);
	half(&out, 0);
	append(&out, parts[0].data, parts[0].len);
	append(&out, parts[1].data, parts[1].len);
	half(&out, 0xffff);
	append(&out, parts[2].data, parts[2].len);
	for (size_t i = 0; i < 3; i++)
		free(parts[i].data);
	return out;
}
