/*
 * ags-list.c
 *
 * Write the assembly for one ordered list of packed pictures.
 *
 *   ags-list FOLDER/list.tsv FOLDER/list.s
 *
 * list.tsv sits in the folder with its pictures. Its first line names two
 * columns: "list", and the form the pictures are packed in (icon4). Each row
 * after it is one picture, in ROM order: the list it belongs to and its file
 * name without ".png". A picture named NAME_ja.png, _de, _es, _fr or _it
 * beside NAME.png replaces it in that edition.
 *
 * A table whose first column is "section" places pictures one after another,
 * each under the global name FOLDER_NAME in mixed case (monster/blue_devil is
 * Monster_BlueDevil). Its columns are: section (empty, or the letter of a
 * .rodata.X section), the picture, "left" (bytes the original packer left
 * after it, as hex digits) and "pad" (an alignment to zero-fill to after it).
 *
 * A table whose first column is "name" is a plain run of placements, one row
 * each: the global name it starts (empty when it continues the one above),
 * the packed file beside the table, "section" (empty, or the letter of a
 * .rodata.X section), "align" (an alignment before it), "left" (bytes the
 * original packer left after it, as hex digits) and "pad" (an alignment to
 * fill to after it), "edition" (empty, or the one language edition
 * the row is for: ja, en, de, es, fr or it) and "fill" (the byte the padding is
 * made of, as hex digits, when it is not zero).
 *
 * A table whose first column is "overlay" is the list of overlays, one row
 * each: the overlay's number in hex, then the bytes the original packer left
 * after it in each edition (ja, en, de, es, fr, it), as hex digits. Each is
 * placed under Resource_OverlayNUMBER and filled to four bytes.
 *
 * Three tables are ones the game's code reads, one row an entry:
 * "song, player" is gSongTable (an empty song is the silent one);
 * "scene, code, area, kind, map, attributes" is gScenes, a named row also
 * giving SceneId_NAME its number; "resource, place" is the resource
 * directory, a named row also giving ResourceId_NAME its number.
 *
 * The rest are tables of records, one row an entry, each with its layout in
 * ROWS below: the first line it is known by, what is written before the rows,
 * for each row and after them (the end marker). In a layout {a} is the row's
 * first cell, {b} its second and so on; {A} is the cell in mixed case; {a|X}
 * is the cell, or X when it is empty; {a?X|Y} is X when the cell is filled
 * (with @ for the cell) and Y when it is empty; {0} is the table's file name
 * in mixed case and {/} its folder. A last column named "edition" holds the
 * one language edition the row is for, or nothing when every edition has it.
 *
 * Each list becomes a global table of pointers ended by -1, with a global
 * LISTEnd label at that -1, followed by its pictures in the same order. A
 * picture named again later is pointed at again and placed once.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_ROWS 4096
#define MAX_TEXT 256

static const char *const LANGUAGES[] = {"ja", "de", "es", "fr", "it"};

static void
fail(const char *what, const char *detail)
{
	fprintf(stderr, "ags-list: %s: %s\n", what, detail);
	exit(1);
}

static int
exists(const char *path)
{
	struct stat info;
	return stat(path, &info) == 0;
}

/* item_long_sword -> ItemLongSword: a label local to the written file. */
static void
label(const char *name, char *out)
{
	int upper = 1;
	for (; *name; name++)
	{
		if (*name == '_')
			upper = 1;
		else
		{
			*out++ = upper ? (char)toupper((unsigned char)*name) : *name;
			upper = 0;
		}
	}
	*out = 0;
}

struct rows
{
	const char *header, *game, *before, *row, *after;
};

#define RODATA "@ Written by ags-list from its table: do not edit.\n\t.section .rodata\n"
#define MUSIC_ROW "\t.2byte {a?SceneId_@|{b}}, ({c|-1} & 0x7fff) | {a?0x8000|0}, {d|-1}, {e}\n"
#define NAME_ROW "\t.2byte {a?SceneId_@|{b}}, ({c|-1} & 0x7fff) | {a?0x8000|0}\n\t.4byte {d}\n"
#define NAME_END "\t.2byte -1, -1\n\t.4byte -1\n\t.2byte 0, 0\n\t.4byte 0\n"

static const struct rows ROWS[] = {
	/* A voice group, Sound_NAMEVoices: one row a voice. */
	{"voice\tkey\tlength\tpan\ttarget\tattack\tdecay\tsustain\trelease", NULL,
	 RODATA "\t.set voice_direct, 0\n\t.set voice_square1, 1\n\t.set voice_square2, 2\n\t.set voice_wave, 3\n"
	        "\t.set voice_noise, 4\n\t.set voice_direct_fixed, 8\n\t.set voice_square1_alt, 9\n"
	        "\t.set voice_square2_alt, 10\n\t.set voice_wave_alt, 11\n\t.set voice_noise_alt, 12\n"
	        "\t.set voice_drums, 128\n\t.balign 4\n\t.global Sound_{0}Voices\nSound_{0}Voices:\n",
	 "\t.byte voice_{a}, {b}, {c}, {d}\n\t.4byte {e}\n\t.byte {f}, {g}, {h}, {i}\n", ""},
	/* gMPlayTable: each music player's work area, its tracks and how many. */
	{"player\ttracks\tcount", NULL, RODATA "\t.balign 4\n\t.global gMPlayTable\ngMPlayTable:\n",
	 "\t.4byte {a}, {b}\n\t.byte {c}, 0\n\t.2byte 0\n",
	 "\t.global gNumMusicPlayers\n\t.equ gNumMusicPlayers, (. - gMPlayTable) / 12\n"
	 "\t.global gMaxLines\n\t.equ gMaxLines, 0\n"},
	/* The samples in the table's folder: a recorded one is NAME.pcm8.bin
	 * under Sound_WaveNAME, a synthesized one is Sound_SynthNAME. */
	{"sample\tsynth\tduty\tspeed\tdepth\tphase", NULL,
	 RODATA "\t.set synth_pulse, 0\n\t.set synth_saw, 1\n\t.set synth_triangle, 2\n\t.balign 4\n",
	 "\t.global Sound_{b?Synth|Wave}{A}\nSound_{b?Synth|Wave}{A}:\n"
	 "{b?\t.4byte 1 << 30, 17140000, 0, 0\n\t.byte 0x80, synth_@\n{c?\t.byte @, {d}, {e}, {f}\n|}\t.balign 4\n"
	 "|\t.incbin \"{/}/{a}.pcm8.bin\"\n}",
	 ""},
	/* gScenes of The Broken Seal; MapIndex_World is the World Map's map. */
	{"scene\tcode\tarea\tkind\tmap", NULL, RODATA "\t.balign 4\n\t.global gScenes\ngScenes:\n",
	 "{a?\t.global SceneId_@\n\t.equiv SceneId_@, (. - gScenes) / 8\n\t.equiv SceneMap_@, {e}\n|}"
	 "\t.2byte {b}\n\t.byte {c}, {d}\n\t.2byte {e}, 0\n",
	 "\t.global MapIndex_World\n\t.equiv MapIndex_World, SceneMap_WorldMap\n"},
	/* gScene_EncounterRules: an empty entrance is any, an empty flag always. */
	{"scene\tentrance\tflag\tshift\tzone", NULL,
	 RODATA "\t.include \"enums/flags.inc\"\n\t.balign 4\n\t.global gScene_EncounterRules\ngScene_EncounterRules:\n",
	 "\t.2byte SceneId_{a}, {b|-1}\n\t.2byte ({c|-1} & 0x7fff) | ({d} << 15), {e}\n", "\t.2byte -1, -1, 0, 0\n"},
	{"scene\tentrance\tflag\tzone0\tzone1\tedition", NULL,
	 RODATA "\t.include \"enums/flags.inc\"\n\t.global gScene_EncounterRules\ngScene_EncounterRules:\n",
	 "\t.2byte SceneId_{a}, {b|-1}, {c|-1}\n\t.byte {d}, {e}\n", "\t.2byte -1, -1, 0, 0\n"},
	/* gScene_MusicRules: a rule is for a scene or for an area. */
	{"scene\tarea\tentrance\tflag\tmusic", "tbs",
	 RODATA "\t.include \"enums/flags.inc\"\n\t.include \"enums/sounds.inc\"\n\t.balign 4\n"
	        "\t.global gScene_MusicRules\ngScene_MusicRules:\n",
	 MUSIC_ROW, "\t.2byte -1, -1, -1, 0\n"},
	{"scene\tarea\tentrance\tflag\tmusic", "tla",
	 RODATA "\t.include \"enums/flags.inc\"\n\t.include \"enums/sounds.inc\"\n\t.include \"enums/area_ids.inc\"\n"
	        "\t.global gScene_MusicRules\ngScene_MusicRules:\n",
	 MUSIC_ROW, "\t.2byte -1, -1, -1, 0\n"},
	/* gArea_NameRules; a row of zeros follows its end. */
	{"scene\tarea\tentrance\tname\tedition", "tbs", RODATA "\t.global gArea_NameRules\ngArea_NameRules:\n", NAME_ROW,
	 NAME_END},
	{"scene\tarea\tentrance\tname\tedition", "tla",
	 RODATA "\t.include \"enums/area_ids.inc\"\n\t.global gArea_NameRules\ngArea_NameRules:\n", NAME_ROW, NAME_END},
	/* gScene_RestartRules: where the party returns to after a defeat. */
	{"scene\tentrance\tsaved\tsaved_entrance", NULL, RODATA "\t.global gScene_RestartRules\ngScene_RestartRules:\n",
	 "\t.2byte SceneId_{a}, {b|-1}, SceneId_{c}, {d}\n", "\t.2byte -1, -1, -1, -1\n"},
	/* gWorldMapPlaces (struct WorldMapPlace). */
	{"place\tmarker\tmap_marker\tsprite\tx\tz\tfacing\tshown_flag\tshown_while\tvisit_scene\tvisit_entrance\tscene\tentrance\tedition",
	 NULL,
	 RODATA "\t.include \"enums/flags.inc\"\n\t.include \"enums/sprite_ids.inc\"\n"
	        "\t.set shown_while_set, 0x1000\n\t.set shown_while_clear, 0\n\t.global gWorldMapPlaces\ngWorldMapPlaces:\n",
	 "\t.byte {a}, {b} | ({c} << 4)\n\t.2byte {d}, {e}, {f}, {g} * 0x10000 / 360, {h?@ + shown_while_{i}|-1}\n"
	 "\t.2byte {j?SceneId_@|-1}, {j?{k|-1}|-2}, {l?SceneId_@|0}, {m|-1}\n",
	 "\t.2byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"},
	/* gSpritePictureCache_Sources: the resource an animation's pictures are in. */
	{"animation\tresource", NULL, RODATA "\t.global gSpritePictureCache_Sources\ngSpritePictureCache_Sources:\n",
	 "\t.2byte ResourceId_{b}, {a}\n", "\t.2byte 0, 0\n"},
	/* Strings the code names, each written as the assembler reads it and
	 * filled to four bytes. Resource_BuildStamp is when an edition was built:
	 * year, month, day, hour, minute and second, two digits each. */
	{"string\ttext\tedition", NULL, RODATA "\t.balign 4\n",
	 "\t.global {a}\n{a}:\n\t.asciz \"{b}\"\n\t.balign 4, 0\n", ""},
};

static const char *row_folder, *row_table;

/* Write a layout, filling its {} from the row's cells. */
static void
expand(FILE *out, const char *at, const char *end, char **cells, const char *self)
{
	while (at < end)
	{
		const char *close = at + 1, *bar = NULL, *cell;
		char text[MAX_TEXT];

		if (*at == '@' && self)
			fputs(self, out);
		if (*at != '{')
		{
			if (*at != '@' || !self)
				fputc(*at, out);
			at++;
			continue;
		}
		for (int depth = 1; depth; close++)
		{
			depth += (*close == '{') - (*close == '}');
			if (depth == 1 && *close == '|' && !bar)
				bar = close;
		}
		close--;
		cell = at[1] == '/' ? row_folder : at[1] == '0' ? row_table : cells[tolower((unsigned char)at[1]) - 'a'];
		if (at[1] == '0' || isupper((unsigned char)at[1]))
		{
			label(cell, text);
			cell = text;
		}
		if (at[2] == '?' && cell[0])
			expand(out, at + 3, bar ? bar : close, cells, cell);
		else if (at[2] == '}' || cell[0])
			fputs(at[2] == '?' ? "" : cell, out);
		if (bar && !cell[0])
			expand(out, bar + 1, close, cells, self);
		at = close + 1;
	}
}

static int
records(const struct rows *rows, FILE *in, char **argv, const char *folder, const char *game)
{
	char line[1024], table[MAX_TEXT], *cells[16];
	const char *base = strrchr(argv[1], '/') + 1;
	int last = 0, edition;
	FILE *out = fopen(argv[2], "w");

	if (!out)
		fail(argv[2], "cannot write");
	snprintf(table, sizeof table, "%.*s", (int)strcspn(base, "."), base);
	row_folder = folder;
	row_table = table;
	for (const char *at = rows->header; *at; at++)
		last += *at == '\t';
	edition = strlen(rows->header) > 8 && !strcmp(rows->header + strlen(rows->header) - 8, "\tedition");
	expand(out, rows->before, rows->before + strlen(rows->before), cells, NULL);
	while (fgets(line, sizeof line, in))
	{
		int cell = 0;

		line[strcspn(line, "\n")] = 0;
		if (!line[0])
			continue;
		for (cell = 0; cell < 16; cell++)
			cells[cell] = "";
		cells[cell = 0] = line;
		for (char *at = line; *at; at++)
			if (*at == '\t' && cell < last)
			{
				*at = 0;
				cells[++cell] = at + 1;
			}
		if (edition && cells[last][0])
			fprintf(out, "\t.ifdef %s_EDITION_%c%c\n", game, toupper((unsigned char)cells[last][0]),
			        toupper((unsigned char)cells[last][1]));
		expand(out, rows->row, rows->row + strlen(rows->row), cells, NULL);
		if (edition && cells[last][0])
			fprintf(out, "\t.endif\n");
	}
	expand(out, rows->after, rows->after + strlen(rows->after), cells, NULL);
	fclose(in);
	if (fclose(out))
		fail(argv[2], "cannot write");
	return 0;
}

int
main(int argc, char **argv)
{
	static char lists[MAX_ROWS][MAX_TEXT], names[MAX_ROWS][MAX_TEXT];
	char folder[1024], form[MAX_TEXT], line[1024], edition[8] = "";
	size_t count = 0;
	FILE *in, *out;
	char *slash, *game;

	if (argc != 3)
		fail("usage", "ags-list FOLDER/list.tsv FOLDER/list.s");
	snprintf(folder, sizeof folder, "%s", argv[1]);
	slash = strrchr(folder, '/');
	if (!slash)
		fail(argv[1], "the table must sit in a folder");
	*slash = 0;
	game = strstr(folder, "games/");
	if (!game || strlen(game) < 9)
		fail(argv[1], "the table must be under games/<game>/");
	edition[0] = (char)toupper((unsigned char)game[6]);
	edition[1] = (char)toupper((unsigned char)game[7]);
	edition[2] = (char)toupper((unsigned char)game[8]);

	in = fopen(argv[1], "r");
	if (!in)
		fail(argv[1], "cannot open");
	if (!fgets(line, sizeof line, in))
		fail(argv[1], "empty");
	line[strcspn(line, "\n")] = 0;
	for (size_t row = 0; row < sizeof ROWS / sizeof *ROWS; row++)
		if (!strcmp(line, ROWS[row].header) && (!ROWS[row].game || !strncmp(game + 6, ROWS[row].game, 3)))
			return records(&ROWS[row], in, argv, folder, edition);
	strcat(line, "\n");
	if (sscanf(line, "section\t%255[^\t\n]", form) == 1)
	{
		char prefix[MAX_TEXT], section[MAX_TEXT] = "\1";
		const char *base = strrchr(folder, '/');

		label(base ? base + 1 : folder, prefix);
		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from list.tsv: do not edit.\n");
		while (fgets(line, sizeof line, in))
		{
			char *cells[4] = {line, "", "", ""}, text[MAX_TEXT];
			int cell = 0;

			line[strcspn(line, "\n")] = 0;
			if (!line[0])
				continue;
			for (char *at = line; *at; at++)
				if (*at == '\t' && cell < 3)
				{
					*at = 0;
					cells[++cell] = at + 1;
				}
			if (strcmp(section, cells[0]))
			{
				snprintf(section, sizeof section, "%s", cells[0]);
				fprintf(out, "\t.section .rodata%s%s\n", section[0] ? "." : "", section);
			}
			label(cells[1], text);
			fprintf(out, "\t.global %s_%s\n%s_%s:\n\t.incbin \"%s/%s.%s\"\n", prefix, text, prefix, text, folder, cells[1], form);
			for (const char *digit = cells[2]; digit[0] && digit[1]; digit += 2)
				fprintf(out, "\t.byte 0x%c%c\n", digit[0], digit[1]);
			if (cells[3][0])
				fprintf(out, "\t.balign %s, 0\n", cells[3]);
		}
		fclose(in);
		if (fclose(out))
			fail(argv[2], "cannot write");
		return 0;
	}
	if (!strcmp(line, "song\tplayer\n"))
	{
		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from its table: do not edit.\n\t.section .rodata\n\t.balign 4\n\t.global gSongTable\ngSongTable:\n");
		while (fgets(line, sizeof line, in))
		{
			char *cells[2] = {line, ""};
			int cell = 0;

			line[strcspn(line, "\n")] = 0;
			if (!line[0])
				continue;
			for (char *at = line; *at; at++)
				if (*at == '\t' && cell < 1)
				{
					*at = 0;
					cells[++cell] = at + 1;
				}
			fprintf(out, "\t.4byte %s\n\t.2byte %s, %s\n", cells[0][0] ? cells[0] : ".LEmptySong", cells[1], cells[1]);
		}
		fprintf(out, ".LEmptySong:\n\t.byte 0, 0, 0, 0\n");
		fclose(in);
		if (fclose(out))
			fail(argv[2], "cannot write");
		return 0;
	}
	if (!strncmp(line, "scene\tcode\t", 11))
	{
		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from its table: do not edit.\n\t.include \"enums/area_ids.inc\"\n\t.include \"enums/system/main_routine_state.inc\"\n\t.section .rodata\n\t.balign 4\n\t.global gScenes\ngScenes:\n");
		while (fgets(line, sizeof line, in))
		{
			char *cells[6] = {line, "", "", "", "", ""};
			int cell = 0;

			line[strcspn(line, "\n")] = 0;
			if (!line[0])
				continue;
			for (char *at = line; *at; at++)
				if (*at == '\t' && cell < 5)
				{
					*at = 0;
					cells[++cell] = at + 1;
				}
			if (cells[0][0])
				fprintf(out, "\t.global SceneId_%s\n\t.equiv SceneId_%s, (. - gScenes) / 8\n", cells[0], cells[0]);
			fprintf(out, "\t.2byte %s\n\t.byte %s, %s\n\t.2byte %s, %s\n", cells[1], cells[2], cells[3], cells[4], cells[5]);
		}
		fclose(in);
		if (fclose(out))
			fail(argv[2], "cannot write");
		return 0;
	}
	if (!strcmp(line, "resource\tplace\n"))
	{
		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from its table: do not edit.\n\t.section .rodata\n\t.balign 4\n\t.global Resource_DirectoryTable\nResource_DirectoryTable:\n");
		while (fgets(line, sizeof line, in))
		{
			char *cells[2] = {line, ""};
			int cell = 0;

			line[strcspn(line, "\n")] = 0;
			if (!line[0])
				continue;
			for (char *at = line; *at; at++)
				if (*at == '\t' && cell < 1)
				{
					*at = 0;
					cells[++cell] = at + 1;
				}
			if (cells[0][0])
				fprintf(out, "\t.global ResourceId_%s\n\t.equiv ResourceId_%s, (. - Resource_DirectoryTable) / 4\n", cells[0], cells[0]);
			fprintf(out, "\t.4byte %s\n", cells[1]);
		}
		fclose(in);
		if (fclose(out))
			fail(argv[2], "cannot write");
		return 0;
	}
	if (!strncmp(line, "overlay\t", 8))
	{
		static const char *const EDITIONS[] = {"JA", "EN", "DE", "ES", "FR", "IT"};

		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from its table: do not edit.\n\t.section .overlays,\"a\"\n");
		while (fgets(line, sizeof line, in))
		{
			char *cells[7] = {line, "", "", "", "", "", ""}, upper[MAX_TEXT];
			int cell = 0;

			line[strcspn(line, "\n")] = 0;
			if (!line[0])
				continue;
			for (char *at = line; *at; at++)
				if (*at == '\t' && cell < 6)
				{
					*at = 0;
					cells[++cell] = at + 1;
				}
			for (cell = 0; cells[0][cell]; cell++)
				upper[cell] = (char)toupper((unsigned char)cells[0][cell]);
			upper[cell] = 0;
			fprintf(out, "\t.global Resource_Overlay%s\nResource_Overlay%s:\n\t.incbin \"overlays/resource_%s.lz\"\n", upper, upper, cells[0]);
			for (cell = 1; cell < 7; cell++)
			{
				if (!cells[cell][0])
					continue;
				fprintf(out, "\t.ifdef %s_EDITION_%s\n", edition, EDITIONS[cell - 1]);
				for (const char *digit = cells[cell]; digit[0] && digit[1]; digit += 2)
					fprintf(out, "\t.byte 0x%c%c\n", digit[0], digit[1]);
				fprintf(out, "\t.endif\n");
			}
			fprintf(out, "\t.balign 4, 0\n");
		}
		fclose(in);
		if (fclose(out))
			fail(argv[2], "cannot write");
		return 0;
	}
	if (!strncmp(line, "name\tfile\t", 10))
	{
		char section[MAX_TEXT] = "\1";

		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from its table: do not edit.\n");
		while (fgets(line, sizeof line, in))
		{
			char *cells[8] = {line, "", "", "", "", "", "", ""};
			int cell = 0;

			line[strcspn(line, "\n")] = 0;
			if (!line[0])
				continue;
			for (char *at = line; *at; at++)
				if (*at == '\t' && cell < 7)
				{
					*at = 0;
					cells[++cell] = at + 1;
				}
			if (strcmp(section, cells[2]))
			{
				snprintf(section, sizeof section, "%s", cells[2]);
				fprintf(out, "\t.section .rodata%s%s\n", section[0] ? "." : "", section);
			}
			if (cells[6][0])
				fprintf(out, "\t.ifdef %s_EDITION_%c%c\n", edition, toupper((unsigned char)cells[6][0]), toupper((unsigned char)cells[6][1]));
			if (cells[3][0])
				fprintf(out, "\t.balign %s\n", cells[3]);
			if (cells[0][0])
				fprintf(out, "\t.global %s\n%s:\n", cells[0], cells[0]);
			fprintf(out, "\t.incbin \"%s/%s\"\n", folder, cells[1]);
			for (const char *digit = cells[4]; digit[0] && digit[1]; digit += 2)
				fprintf(out, "\t.byte 0x%c%c\n", digit[0], digit[1]);
			if (cells[5][0])
				fprintf(out, "\t.balign %s, 0x%s\n", cells[5], cells[7][0] ? cells[7] : "0");
			if (cells[6][0])
				fprintf(out, "\t.endif\n");
		}
		fclose(in);
		if (fclose(out))
			fail(argv[2], "cannot write");
		return 0;
	}
	if (sscanf(line, "list\t%255s", form) != 1)
		fail(argv[1], "the first line must be: list<TAB>FORM or section<TAB>FORM<TAB>left<TAB>pad");
	while (fgets(line, sizeof line, in))
	{
		if (line[0] == '\n')
			continue;
		if (count == MAX_ROWS)
			fail(argv[1], "too many rows");
		if (sscanf(line, "%255[^\t]\t%255s", lists[count], names[count]) != 2)
			fail(argv[1], line);
		count++;
	}
	fclose(in);

	out = fopen(argv[2], "w");
	if (!out)
		fail(argv[2], "cannot write");
	fprintf(out, "@ Written by ags-list from list.tsv: do not edit.\n\t.section .rodata\n");
	for (size_t first = 0; first < count;)
	{
		size_t end = first;
		char text[MAX_TEXT];

		while (end < count && !strcmp(lists[end], lists[first]))
			end++;
		fprintf(out, "\t.balign 4\n\t.global %s\n%s:\n", lists[first], lists[first]);
		for (size_t row = first; row < end; row++)
		{
			label(names[row], text);
			fprintf(out, "\t.4byte .L%s\n", text);
		}
		fprintf(out, "\t.global %sEnd\n%sEnd:\n\t.4byte -1\n", lists[first], lists[first]);
		for (size_t row = first; row < end; row++)
		{
			char path[2048];
			int open = 0;
			size_t earlier = 0;

			while (earlier < row && strcmp(names[earlier], names[row]))
				earlier++;
			if (earlier < row)
				continue;

			snprintf(path, sizeof path, "%s/%s.png", folder, names[row]);
			if (!exists(path))
				fail(path, "no such picture");
			label(names[row], text);
			fprintf(out, ".L%s:\n", text);
			for (size_t language = 0; language < sizeof LANGUAGES / sizeof *LANGUAGES; language++)
			{
				snprintf(path, sizeof path, "%s/%s_%s.png", folder, names[row], LANGUAGES[language]);
				if (!exists(path))
					continue;
				fprintf(out, "\t.ifdef %s_EDITION_%c%c\n\t.incbin \"%s/%s_%s.%s\"\n\t.else\n", edition,
				        toupper((unsigned char)LANGUAGES[language][0]), toupper((unsigned char)LANGUAGES[language][1]),
				        folder, names[row], LANGUAGES[language], form);
				open++;
			}
			fprintf(out, "\t.incbin \"%s/%s.%s\"\n", folder, names[row], form);
			while (open--)
				fprintf(out, "\t.endif\n");
		}
		fprintf(out, "\t.balign 4, 0\n");
		first = end;
	}
	if (fclose(out))
		fail(argv[2], "cannot write");
	return 0;
}
