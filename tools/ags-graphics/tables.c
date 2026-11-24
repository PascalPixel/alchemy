/*
 * tables.c
 *
 * Encode records, planes, animation scripts, and block grids.
 * These are distinct TSV formats: column names give record widths, while
 * planes and blocks keep spatial positions.
 */
#include <ctype.h>
#include "resource.h"

/*
 * A table's columns are known by their names: each writes a number of its
 * own width, and a column this file does not know is a label for people,
 * such as a name, and writes nothing. A final short record is allowed; once
 * encountered, no later row may follow.
 */
/*
 * The type of a column that is known by its name alone, or nothing.
 */
static const char *
typeOf(const char *name)
{
	static const char *const BYTES[] = {"shape", "height0", "height1",
										"height2", "id", "flags",
										"upper_left", "upper_right",
										"lower_left", "lower_right"};
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
	/* A numbered column, word2 or attr3, has the type of its letters. */
	size_t letters = strcspn(name, "0123456789");
	if (!name[letters] || name[letters + strspn(name + letters, "0123456789")])
		return NULL;
	if (letters == 4 && !strncmp(name, "word", 4))
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
			const char *type = typeOf(columns.items[i]);
			if (!type)
				continue;
			bool sign = *type == 's';
			unsigned bits = number(type + 1, 10, 32);
			char *s = trim(fields.items[i]);
			bool neg = *s == '-';
			if (neg)
				s++;
			uint64_t magnitude = number(s, 10, UINT32_MAX),
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
 * A map's timed changes, one command a row; the game reads halfwords.
 *
 * Tileset animations, under the columns animation, command, value, count,
 * destination and frames. The rows of one animation, 0 to 15, stand
 * together:
 *
 *   picture   copy count tiles from tile value to tile destination, then
 *             wait frames
 *   jump      continue at the animation's command value, counted from 0
 *   stop      stay here for good
 *
 * NAME.paused.tsv beside the table, one column, lists the animations that
 * start paused.
 *
 * A blend script, under the columns command, value and frames:
 *
 *   control   set the blend register to value
 *   level     set the blend level to value, then wait frames
 *   jump      continue at command value, counted from 0
 *
 * The marker that starts each animation and the one that ends the script are
 * written here. The game jumps to a pair of halfwords, so a jump's target
 * starts on one.
 */
Buffer
script(const char *input, const Buffer *data)
{
	Strings lines = linesOf(data), paused = {0};
	if (!lines.len)
		fail("a script names its columns");
	bool blend = !strcmp(lines.items[0], "command\tvalue\tframes");
	if (!blend &&
		strcmp(lines.items[0],
			   "animation\tcommand\tvalue\tcount\tdestination\tframes"))
		fail("a script's columns are a tileset animation's or a blend's");
	char *path = formatString("%.*s.paused.tsv", (int) (strlen(input) - 4),
							  input);
	if (!blend && exists(path))
	{
		Buffer text = readFile(path);
		paused = linesOf(&text);
		free(text.data);
	}
	free(path);
	Buffer out = {0};
	size_t starts[256], count = 0, first = blend ? 0 : 1;
	char *animation = NULL;
	for (size_t i = 1; i < lines.len; i++)
	{
		Strings f = split(lines.items[i], '\t');
		for (size_t j = 0; j < f.len; j++)
			memmove(f.items[j], trim(f.items[j]), strlen(trim(f.items[j])) + 1);
		if (f.len <= first)
			fail("a script row needs its command");
		if (!blend && (!animation || strcmp(animation, f.items[0])))
		{
			half(&out, 0xfd00 | number(f.items[0], 10, 15) |
						   (hasString(&paused, f.items[0]) ? 0x80 : 0));
			free(animation);
			animation = copyString(f.items[0]);
			count = 0;
		}
		const char *command = f.items[first];
		size_t values = f.len - first - 1;
		if (count == 256)
			fail("too many script commands");
		starts[count++] = out.len;
		if (!strcmp(command, "jump") && values == 1)
		{
			size_t to = number(f.items[first + 1], 10, count - 1),
				   bytes = starts[to] - starts[0];
			if (bytes % 4 || bytes / 4 > 254)
				fail("a jump's target does not start on a pair of halfwords");
			half(&out, 0xfe00 | (unsigned) (bytes / 4));
		}
		else if (!strcmp(command, "stop") && values == 0)
			half(&out, 0xfeff);
		else if ((!blend && !strcmp(command, "picture") && values == 4) ||
				 (blend && !strcmp(command, "control") && values == 1) ||
				 (blend && !strcmp(command, "level") && values == 2))
			for (size_t j = first + 1; j < f.len; j++)
				half(&out, number(f.items[j], 10,
								  j == first + 1 ? 0xfcff : 0xffff));
		else
			fail("unknown script command: %s", lines.items[i]);
		freeStrings(&f);
	}
	half(&out, 0xffff);
	free(animation);
	freeStrings(&paused);
	freeStrings(&lines);
	return out;
}

/*
 * A grid of 16x16 blocks under a header line numbering its columns, one
 * number a cell. A block whose 256 cells are all empty is missing: its
 * offset stays zero, while every other block is packed on its own. A row may
 * stop at its last number.
 */
Buffer
blocks(const Buffer *input)
{
	Strings lines = split((char *) input->data, '\n');
	if (lines.len && !*lines.items[lines.len - 1])
		free(lines.items[--lines.len]);
	if (lines.len < 17 || (lines.len - 1) % 16)
		fail("block grid needs its header and whole 16x16 blocks");
	Strings header = split(lines.items[0], '\t');
	size_t width = header.len, high = lines.len - 1;
	if (width % 16)
		fail("block grid needs whole 16x16 blocks");
	Strings *rows = allocate(high, sizeof(Strings));
	for (size_t y = 0; y < high; y++)
	{
		rows[y] = split(lines.items[y + 1], '\t');
		if (rows[y].len > width)
			fail("a block grid row is wider than its header");
	}
	size_t count = width / 16 * (high / 16);
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
				const char *s = x + dx < rows[y + dy].len
									? trim(rows[y + dy].items[x + dx])
									: "";
				if (!*s)
					missing++;
				else
					word(&values, number(s, 10, UINT32_MAX));
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
	for (size_t i = 0; i < high; i++)
		freeStrings(rows + i);
	free(rows);
	freeStrings(&header);
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
 * A background's tile map as the screen shows it: NAME.tsv, a header line
 * numbering its columns and a row for every row of the screen, one tile
 * number for each 8x8 square. Two tables of the same shape may sit beside
 * it: NAME.flip.tsv, where a square is h, v, hv or empty, and
 * NAME.palette.tsv, the palette of each square; without them nothing is
 * flipped and the palette is 0. A row of flips may stop at its last flip.
 * Each square becomes the hardware's halfword; the width, when asked for,
 * is the number of squares in a row.
 */
Buffer
tilemap(const char *input, size_t *width)
{
	static const char *const KINDS[] = {"", ".flip", ".palette"};
	Strings lines[3] = {{0}, {0}, {0}};
	for (size_t k = 0; k < 3; k++)
	{
		char *path = formatString("%.*s%s.tsv", (int) (strlen(input) - 4),
								  input, KINDS[k]);
		if (exists(path))
		{
			Buffer text = readFile(path);
			lines[k] = split((char *) text.data, '\n');
			free(text.data);
		}
		free(path);
	}
	while (lines[0].len && !*trim(lines[0].items[lines[0].len - 1]))
		free(lines[0].items[--lines[0].len]);
	if (lines[0].len < 2)
		fail("a tile map needs its header and a row");
	Strings header = split(lines[0].items[0], '\t');
	size_t wide = header.len;
	Buffer out = {0};
	for (size_t y = 1; y < lines[0].len; y++)
	{
		Strings row[3];
		for (size_t k = 0; k < 3; k++)
			row[k] = split(y < lines[k].len ? lines[k].items[y] : "", '\t');
		if (row[0].len != wide || (lines[2].len && row[2].len != wide))
			fail("a tile map row is not as wide as its header");
		for (size_t x = 0; x < wide; x++)
		{
			unsigned value = number(trim(row[0].items[x]), 10, 1023);
			for (const char *c = x < row[1].len ? trim(row[1].items[x]) : "";
				 *c; c++)
			{
				if (*c != 'h' && *c != 'v')
					fail("a flip is h, v or hv");
				value |= *c == 'h' ? 1024 : 2048;
			}
			if (lines[2].len)
				value |= number(trim(row[2].items[x]), 10, 15) << 12;
			half(&out, value);
		}
		for (size_t k = 0; k < 3; k++)
			freeStrings(row + k);
	}
	if (width)
		*width = wide;
	freeStrings(&header);
	for (size_t k = 0; k < 3; k++)
		freeStrings(lines + k);
	return out;
}

/*
 * The world map's collision tiles as the game reads them: a grid of values
 * of 4 bits under a header line naming its columns, a tile being 4 by 4 of
 * them. Tiles run left to right, then down. Each row of a tile packs into
 * two bytes, the left value in the low nibble.
 */
Buffer
collision(const Buffer *input)
{
	Strings lines = linesOf(input);
	if (lines.len < 5 || (lines.len - 1) % 4)
		fail("collision tiles need a header and whole rows of four");
	size_t wide = 0;
	Strings *rows = allocate(lines.len, sizeof(Strings));
	for (size_t y = 0; y < lines.len; y++)
	{
		rows[y] = split(lines.items[y], '\t');
		if (!y)
			wide = rows[y].len;
		if (!wide || wide % 4 || rows[y].len != wide)
			fail("collision rows differ, or are not whole tiles");
	}
	Buffer out = {0};
	for (size_t band = 1; band < lines.len; band += 4)
		for (size_t x = 0; x < wide; x += 4)
			for (size_t y = band; y < band + 4; y++)
			{
				unsigned values[4];
				for (unsigned i = 0; i < 4; i++)
					values[i] = number(trim(rows[y].items[x + i]), 10, 15);
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
