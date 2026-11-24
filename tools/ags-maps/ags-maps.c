/*
 * ags-maps.c
 *
 * Write the assembly for every map of the field from the tables in its folder.
 *
 *   ags-maps games/GAME/src/map games/GAME/src/map/maps.s
 *
 * maps.tsv, beside the maps' folders, lists the folders in the order the Game
 * Pak holds the maps, with a name for a map when the folder's is not used
 * (snow_cave is Field_SnowCave). Each folder holds the map's own tables:
 *
 *   header.tsv  one row: the twelve numbers of the map's header.
 *   layers.tsv  three rows: the eight numbers of each layer.
 *   blocks.tsv  one row for each block the header points at, in order: the
 *               block's name, its packed file (in this folder, or as
 *               FOLDER/FILE in another map's) and an alignment to zero-fill
 *               to after it. The block "none" is a pointer of zero; a block
 *               with no file is the map's treasure spots.
 *   spots.tsv   one row a treasure spot: column, row and subject. Only where
 *               the map has any.
 *   placed.tsv  what follows the map, where anything does: a global name
 *               (empty when it continues the one above), a packed file as a
 *               path from the maps' folder, the bytes the original packer
 *               left after it as hex digits, an alignment to fill to and the
 *               byte to fill with when it is not zero.
 *
 * The last column of each is "edition": empty, or the one language edition
 * the row is for (ja, en, de, es, fr or it).
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CELLS 16

struct Table
{
	char *(*rows)[MAX_CELLS];
	size_t count;
};

static char game[4];
static FILE *out;

static void
fail(const char *what, const char *detail)
{
	fprintf(stderr, "ags-maps: %s: %s\n", what, detail);
	exit(1);
}

/*
 * Read a table whole; every cell a row lacks is an empty string. A table
 * that may be left out is empty when its file is not there.
 */
static struct Table
load(const char *folder, const char *name, int needed)
{
	struct Table table = {0};
	char path[1024], *text, *line, *next;
	size_t room = 0;
	long size;
	FILE *in;

	snprintf(path, sizeof path, "%s/%s.tsv", folder, name);
	in = fopen(path, "rb");
	if (!in && !needed)
		return table;
	if (!in || fseek(in, 0, SEEK_END) || (size = ftell(in)) < 0)
		fail(path, "cannot read");
	rewind(in);
	text = malloc((size_t)size + 1);
	if (!text || fread(text, 1, (size_t)size, in) != (size_t)size)
		fail(path, "cannot read");
	text[size] = 0;
	fclose(in);
	line = strchr(text, '\n');
	for (line = line ? line + 1 : text + size; *line; line = next)
	{
		int cell = 0;

		next = strchr(line, '\n');
		if (next)
			*next++ = 0;
		else
			next = line + strlen(line);
		if (!*line)
			continue;
		if (table.count == room)
		{
			room = room ? room * 2 : 16;
			table.rows = realloc(table.rows, room * sizeof *table.rows);
			if (!table.rows)
				fail(path, "out of memory");
		}
		for (int empty = 0; empty < MAX_CELLS; empty++)
			table.rows[table.count][empty] = "";
		table.rows[table.count][0] = line;
		for (char *at = line; *at; at++)
			if (*at == '\t' && cell < MAX_CELLS - 1)
			{
				*at = 0;
				table.rows[table.count][++cell] = at + 1;
			}
		table.count++;
	}
	return table;
}

static void
begin(const char *edition)
{
	if (edition[0])
		fprintf(out, "\t.ifdef %s_EDITION_%c%c\n", game, toupper((unsigned char)edition[0]), toupper((unsigned char)edition[1]));
}

static void
end(const char *edition)
{
	if (edition[0])
		fprintf(out, "\t.endif\n");
}

static void
bytes(char **cells, int count)
{
	fprintf(out, "\t.byte %s", cells[0]);
	for (int cell = 1; cell < count; cell++)
		fprintf(out, ", %s", cells[cell]);
	fprintf(out, "\n");
}

int
main(int argc, char **argv)
{
	struct Table maps;
	const char *folder, *at;

	if (argc != 3)
		fail("usage", "ags-maps FOLDER FOLDER/maps.s");
	folder = argv[1];
	at = strstr(folder, "games/");
	if (!at || strlen(at) < 9)
		fail(folder, "the folder must be under games/<game>/");
	for (int letter = 0; letter < 3; letter++)
		game[letter] = (char)toupper((unsigned char)at[6 + letter]);
	maps = load(folder, "maps", 1);
	out = fopen(argv[2], "w");
	if (!out)
		fail(argv[2], "cannot write");
	fprintf(out, "@ Written by ags-maps from the tables in each map's folder: do not edit.\n\t.section .rodata\n");
	for (size_t map = 0; map < maps.count; map++)
	{
		const char *name = maps.rows[map][0];
		char own[1024], field[256];
		struct Table header, layers, blocks, spots, placed;

		snprintf(own, sizeof own, "%s/%s", folder, name);
		header = load(own, "header", 1);
		layers = load(own, "layers", 1);
		blocks = load(own, "blocks", 1);
		spots = load(own, "spots", 0);
		placed = load(own, "placed", 0);
		if (maps.rows[map][1][0])
			snprintf(field, sizeof field, "%s", maps.rows[map][1]);
		else
		{
			char *to = field + snprintf(field, sizeof field, "Field_");
			int upper = 1;
			for (const char *from = name; *from; from++)
				if (*from == '_')
					upper = 1;
				else
				{
					*to++ = upper ? (char)toupper((unsigned char)*from) : *from;
					upper = 0;
				}
			*to = 0;
		}
		for (size_t row = 0; row < header.count; row++)
		{
			char **cells = header.rows[row];
			begin(cells[12]);
			fprintf(out, "\t.global %s\n%s:\n", field, field);
			bytes(cells, 12);
			end(cells[12]);
		}
		for (size_t row = 0; row < layers.count; row++)
		{
			char **cells = layers.rows[row];
			begin(cells[8]);
			bytes(cells, 8);
			end(cells[8]);
		}
		for (size_t row = 0; row < blocks.count; row++)
		{
			char **cells = blocks.rows[row];
			begin(cells[3]);
			if (!strcmp(cells[0], "none"))
				fprintf(out, "\t.4byte 0\n");
			else
				fprintf(out, "\t.4byte .L%s_%s - %s\n", field, cells[0], field);
			end(cells[3]);
		}
		for (size_t row = 0; row < blocks.count; row++)
		{
			char **cells = blocks.rows[row];
			if (!strcmp(cells[0], "none"))
				continue;
			begin(cells[3]);
			fprintf(out, ".L%s_%s:\n", field, cells[0]);
			if (cells[1][0])
				fprintf(out, "\t.incbin \"%s/%s\"\n", strchr(cells[1], '/') ? folder : own, cells[1]);
			else
			{
				for (size_t spot = 0; spot < spots.count; spot++)
				{
					char **one = spots.rows[spot];
					if (cells[3][0] && one[3][0] && strcmp(cells[3], one[3]))
						continue;
					if (!cells[3][0])
						begin(one[3]);
					bytes(one, 3);
					if (!cells[3][0])
						end(one[3]);
				}
				fprintf(out, "\t.byte 0xff, 0xff, 0xff\n");
			}
			if (cells[2][0])
				fprintf(out, "\t.balign %s, 0\n", cells[2]);
			end(cells[3]);
		}
		for (size_t row = 0; row < placed.count; row++)
		{
			char **cells = placed.rows[row];
			begin(cells[5]);
			if (cells[0][0])
				fprintf(out, "\t.global %s\n%s:\n", cells[0], cells[0]);
			fprintf(out, "\t.incbin \"%s/%s\"\n", folder, cells[1]);
			for (const char *digit = cells[2]; digit[0] && digit[1]; digit += 2)
				fprintf(out, "\t.byte 0x%c%c\n", digit[0], digit[1]);
			if (cells[3][0])
				fprintf(out, "\t.balign %s, 0x%s\n", cells[3], cells[4][0] ? cells[4] : "0");
			end(cells[5]);
		}
	}
	if (fclose(out))
		fail(argv[2], "cannot write");
	return 0;
}
