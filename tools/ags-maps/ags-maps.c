/*
 * ags-maps.c
 *
 * Write the assembly for every map of the field from five tables.
 *
 *   ags-maps games/GAME/src/map games/GAME/src/map/maps.s
 *
 * The tables sit in the folder with the maps, each row starting with the
 * map's folder name, in the order the Game Pak holds the maps:
 *
 *   maps.tsv    one row a map: a name for it when the folder's is not used
 *               (snow_cave is Field_SnowCave), then the twelve numbers of its
 *               header.
 *   layers.tsv  three rows a map: the eight numbers of each layer.
 *   blocks.tsv  one row for each block the header points at, in order: the
 *               block's name, its packed file (in this map's folder or
 *               another's) and an alignment to zero-fill to after it. The
 *               block "none" is a pointer of zero; a block with no file is
 *               the map's treasure spots.
 *   spots.tsv   one row a treasure spot: column, row and subject.
 *   placed.tsv  what follows the map: a global name (empty when it
 *               continues the one above), a packed file, the bytes the
 *               original packer left after it as hex digits, an alignment to
 *               fill to and the byte to fill with when it is not zero.
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
	size_t count, at;
};

static char game[4];
static FILE *out;

static void
fail(const char *what, const char *detail)
{
	fprintf(stderr, "ags-maps: %s: %s\n", what, detail);
	exit(1);
}

/* Read a table whole; every cell a row lacks is an empty string. */
static struct Table
load(const char *folder, const char *name)
{
	struct Table table = {0};
	char path[1024], *text, *line, *next;
	size_t room = 0;
	long size;
	FILE *in;

	snprintf(path, sizeof path, "%s/%s.tsv", folder, name);
	in = fopen(path, "rb");
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
			room = room ? room * 2 : 1024;
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

/* The table's next row when it belongs to this map, else nothing. */
static char **
row(struct Table *table, const char *map)
{
	if (table->at == table->count || strcmp(table->rows[table->at][0], map))
		return 0;
	return table->rows[table->at++];
}

static void
open(const char *edition)
{
	if (edition[0])
		fprintf(out, "\t.ifdef %s_EDITION_%c%c\n", game, toupper((unsigned char)edition[0]), toupper((unsigned char)edition[1]));
}

static void
close(const char *edition)
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
	struct Table maps, layers, blocks, spots, placed;
	const char *folder, *at;

	if (argc != 3)
		fail("usage", "ags-maps FOLDER FOLDER/maps.s");
	folder = argv[1];
	at = strstr(folder, "games/");
	if (!at || strlen(at) < 9)
		fail(folder, "the folder must be under games/<game>/");
	for (int letter = 0; letter < 3; letter++)
		game[letter] = (char)toupper((unsigned char)at[6 + letter]);
	maps = load(folder, "maps");
	layers = load(folder, "layers");
	blocks = load(folder, "blocks");
	spots = load(folder, "spots");
	placed = load(folder, "placed");
	out = fopen(argv[2], "w");
	if (!out)
		fail(argv[2], "cannot write");
	fprintf(out, "@ Written by ags-maps from the tables beside it: do not edit.\n\t.section .rodata\n");
	while (maps.at < maps.count)
	{
		const char *map = maps.rows[maps.at][0];
		char field[256], **cells;
		size_t first, last, first_spot;

		if (maps.rows[maps.at][1][0])
			snprintf(field, sizeof field, "%s", maps.rows[maps.at][1]);
		else
		{
			char *to = field + snprintf(field, sizeof field, "Field_");
			int upper = 1;
			for (const char *from = map; *from; from++)
				if (*from == '_')
					upper = 1;
				else
				{
					*to++ = upper ? (char)toupper((unsigned char)*from) : *from;
					upper = 0;
				}
			*to = 0;
		}
		while ((cells = row(&maps, map)))
		{
			open(cells[14]);
			fprintf(out, "\t.global %s\n%s:\n", field, field);
			bytes(cells + 2, 12);
			close(cells[14]);
		}
		while ((cells = row(&layers, map)))
		{
			open(cells[9]);
			bytes(cells + 1, 8);
			close(cells[9]);
		}
		first = blocks.at;
		while ((cells = row(&blocks, map)))
		{
			open(cells[4]);
			if (!strcmp(cells[1], "none"))
				fprintf(out, "\t.4byte 0\n");
			else
				fprintf(out, "\t.4byte .L%s_%s - %s\n", field, cells[1], field);
			close(cells[4]);
		}
		last = blocks.at;
		first_spot = spots.at;
		while (row(&spots, map))
			;
		for (size_t block = first; block < last; block++)
		{
			cells = blocks.rows[block];
			if (!strcmp(cells[1], "none"))
				continue;
			open(cells[4]);
			fprintf(out, ".L%s_%s:\n", field, cells[1]);
			if (cells[2][0])
				fprintf(out, "\t.incbin \"%s/%s\"\n", folder, cells[2]);
			else
			{
				for (size_t spot = first_spot; spot < spots.at; spot++)
				{
					char **one = spots.rows[spot];
					if (cells[4][0] && one[4][0] && strcmp(cells[4], one[4]))
						continue;
					if (!cells[4][0])
						open(one[4]);
					bytes(one + 1, 3);
					if (!cells[4][0])
						close(one[4]);
				}
				fprintf(out, "\t.byte 0xff, 0xff, 0xff\n");
			}
			if (cells[3][0])
				fprintf(out, "\t.balign %s, 0\n", cells[3]);
			close(cells[4]);
		}
		while ((cells = row(&placed, map)))
		{
			open(cells[6]);
			if (cells[1][0])
				fprintf(out, "\t.global %s\n%s:\n", cells[1], cells[1]);
			fprintf(out, "\t.incbin \"%s/%s\"\n", folder, cells[2]);
			for (const char *digit = cells[3]; digit[0] && digit[1]; digit += 2)
				fprintf(out, "\t.byte 0x%c%c\n", digit[0], digit[1]);
			if (cells[4][0])
				fprintf(out, "\t.balign %s, 0x%s\n", cells[4], cells[5][0] ? cells[5] : "0");
			close(cells[6]);
		}
	}
	if (layers.at < layers.count || blocks.at < blocks.count || spots.at < spots.count || placed.at < placed.count)
		fail(folder, "a table has rows for a map that maps.tsv does not list here");
	if (fclose(out))
		fail(argv[2], "cannot write");
	return 0;
}
