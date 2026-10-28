/*
 * tables.c
 *
 * Encode typed records, sparse planes, animation scripts, and block grids.
 * These are distinct TSV formats: column types describe record widths, while
 * planes and blocks retain spatial positions even when cells are omitted.
 */
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
			if (!type)
				continue;
			type++;
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

static const char *
headerField(const Strings *fields, const char *key)
{
	size_t n = strlen(key);
	for (size_t i = 0; i < fields->len; i++)
		if (!strncmp(fields->items[i], key, n) && fields->items[i][n] == ':')
			return fields->items[i] + n + 1;
	fail("plane header needs %s", key);
}

static void
pair(const char *text, char separator, size_t *a, size_t *b)
{
	Strings fields = split(text, separator);
	if (fields.len != 2)
		fail("expected two dimensions");
	*a = number(fields.items[0], 10, SIZE_MAX);
	*b = number(fields.items[1], 10, SIZE_MAX);
	freeStrings(&fields);
}

Buffer
plane(const Buffer *input)
{
	/* A blank row still occupies a row in the map. Only drop the final newline.
   */
	Strings lines = split((char *) input->data, '\n');
	if (lines.len && !*lines.items[lines.len - 1])
		free(lines.items[--lines.len]);
	if (!lines.len)
		fail("plane has no header");
	Strings header = split(lines.items[0], '\t');
	const char *cell = headerField(&header, "cell");
	unsigned size = !strcmp(cell, "u8")	   ? 1
					: !strcmp(cell, "u16") ? 2
					: !strcmp(cell, "u32") ? 4
										   : 0;
	if (!size)
		fail("unknown plane cell type");
	size_t width, height, left, top;
	pair(headerField(&header, "grid"), 'x', &width, &height);
	pair(headerField(&header, "at"), ',', &left, &top);
	uint64_t max = (UINT64_C(1) << (size * 8)) - 1;
	uint32_t fill = number(headerField(&header, "fill"), 16, max);
	if (!width || !height || height > SIZE_MAX / width)
		fail("invalid plane dimensions");
	uint32_t *cells = allocate(width * height, sizeof(uint32_t));
	for (size_t i = 0; i < width * height; i++)
		cells[i] = fill;
	for (size_t y = 1; y < lines.len; y++)
	{
		Strings row = words(lines.items[y]);
		if (top >= height || y - 1 >= height - top || left > width ||
			row.len > width - left)
			fail("plane row leaves the grid");
		for (size_t x = 0; x < row.len; x++)
			cells[(top + y - 1) * width + left + x] =
				number(row.items[x], 16, max);
		freeStrings(&row);
	}
	Buffer out = {0};
	for (size_t i = 0; i < width * height; i++)
		for (unsigned b = 0; b < size; b++)
			byte(&out, cells[i] >> (8 * b));
	free(cells);
	freeStrings(&header);
	freeStrings(&lines);
	return out;
}

/*
 * Commands encode either a prefixed operand or a fixed count of halfwords.
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
		unsigned prefix = 0;
		if (!strcmp(key, "channel"))
		{
			n = 1;
			prefix = 0xfd00;
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
			half(&out, prefix | (n ? decimalOrHex(trim(f.items[1]), 255) : 0));
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
 * A layout's grid: one text cell per map square, `picture[:behavior[:shape]]`
 * in hex, a left-out part 0. The header gives the grid's size, where the
 * written rows start and the cell that fills every square they leave out, in
 * the same order. The game reads three planes, behaviors (u8), shapes (u8) and
 * cell pictures (u16), one after another.
 */
Buffer
grid(const Buffer *input)
{
	Strings lines = split((char *) input->data, '\n');
	if (lines.len && !*lines.items[lines.len - 1])
		free(lines.items[--lines.len]);
	if (!lines.len)
		fail("grid has no header");
	Strings header = split(lines.items[0], '\t');
	size_t width, height, left, top;
	pair(headerField(&header, "grid"), 'x', &width, &height);
	pair(headerField(&header, "at"), ',', &left, &top);
	Strings fill = split(headerField(&header, "fill"), ':');
	if (fill.len != 3)
		fail("grid fill needs picture:behavior:shape");
	uint32_t fills[3] = {number(fill.items[0], 16, 0xffff),
						 number(fill.items[1], 16, 0xff),
						 number(fill.items[2], 16, 0xff)};
	if (!width || !height || height > SIZE_MAX / width)
		fail("invalid grid dimensions");
	uint32_t *cells[3];
	for (unsigned k = 0; k < 3; k++)
	{
		cells[k] = allocate(width * height, sizeof(uint32_t));
		for (size_t i = 0; i < width * height; i++)
			cells[k][i] = fills[k];
	}
	for (size_t y = 1; y < lines.len; y++)
	{
		Strings row = words(lines.items[y]);
		if (top >= height || y - 1 >= height - top || left > width ||
			row.len > width - left)
			fail("grid row leaves the grid");
		for (size_t x = 0; x < row.len; x++)
		{
			Strings parts = split(row.items[x], ':');
			if (!parts.len || parts.len > 3)
				fail("grid cell needs one to three parts");
			size_t at = (top + y - 1) * width + left + x;
			for (unsigned k = 0; k < 3; k++)
				cells[k][at] = k < parts.len
								   ? number(parts.items[k], 16, k ? 0xff : 0xffff)
								   : 0;
			freeStrings(&parts);
		}
		freeStrings(&row);
	}
	Buffer out = {0};
	for (size_t i = 0; i < width * height; i++)
		byte(&out, cells[1][i]);
	for (size_t i = 0; i < width * height; i++)
		byte(&out, cells[2][i]);
	for (size_t i = 0; i < width * height; i++)
		half(&out, cells[0][i]);
	for (unsigned k = 0; k < 3; k++)
		free(cells[k]);
	freeStrings(&fill);
	freeStrings(&header);
	freeStrings(&lines);
	return out;
}

/*
 * A background's tile map as the screen shows it: a `palette:N` header, then
 * one text row per row of the screen, one cell per 8x8 square, the tile's
 * number in hex followed by `h` and `v` where it is flipped. Blank lines may
 * separate screens. Each cell becomes the hardware's halfword; the width, when
 * asked for, is the number of cells in a row.
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
			half(&out, number(cell, 16, 1023) | flips << 10 | palette << 12);
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
