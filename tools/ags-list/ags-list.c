/*
 * ags-list.c
 *
 * Write the assembly that places the files a table lists, in the table's
 * order. The table's first line names its columns and says which of four
 * kinds it is. The tool knows these forms and nothing of what the files hold.
 *
 *   ags-list FOLDER/list.tsv FOLDER/list.s
 *
 * "list" and a form (icon4): lists of packed pictures. Each row is the list a
 * picture belongs to and its file name without ".png", beside the table. A
 * picture named NAME_ja.png, _de, _es, _fr or _it beside NAME.png replaces it
 * in that edition. Each list becomes a global table of pointers ended by -1,
 * with a global LISTEnd label at that -1, followed by its pictures in the same
 * order. A picture named again later is pointed at again and placed once.
 *
 * "section" and a form: pictures one after another, each under the global
 * name FOLDER and NAME in mixed case, run together (monster/blue_devil is
 * MonsterBlueDevil).
 * The columns are: section (empty, or the letter of a .rodata.X section), the
 * picture, "left" (bytes the original packer left after it, as hex digits)
 * and "pad" (an alignment to zero-fill to after it).
 *
 * "name": a plain run of placements, one row each: the global name it starts
 * (empty when it continues the one above), the packed file beside the table,
 * "section" (empty, or the letter of a .rodata.X section), "align" (an
 * alignment before it), "left" (bytes the original packer left after it, as
 * hex digits) and "pad" (an alignment to fill to after it), "edition" (empty,
 * or the one language edition the row is for: ja, en, de, es, fr or it) and
 * "fill" (the byte the padding is made of, as hex digits, when it is not
 * zero).
 *
 * "file": files whose leftovers differ by edition, one row each: the file as
 * the assembler finds it, the global name it is placed under, then the bytes
 * the original packer left after it in each edition (ja, en, de, es, fr, it),
 * as hex digits. Each is filled to four bytes.
 *
 * The table must be under games/GAME/: a row for one edition is assembled
 * when the build defines GAME_EDITION_ and the edition, in capitals.
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
			fprintf(out, "\t.global %s%s\n%s%s:\n\t.incbin \"%s/%s.%s\"\n", prefix, text, prefix, text, folder, cells[1], form);
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
	if (!strncmp(line, "file\tname\t", 10))
	{
		static const char *const EDITIONS[] = {"JA", "EN", "DE", "ES", "FR", "IT"};

		out = fopen(argv[2], "w");
		if (!out)
			fail(argv[2], "cannot write");
		fprintf(out, "@ Written by ags-list from its table: do not edit.\n\t.section .rodata\n");
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
			fprintf(out, "\t.global %s\n%s:\n\t.incbin \"%s\"\n", cells[1], cells[1], cells[0]);
			for (cell = 2; cell < 8; cell++)
			{
				if (!cells[cell][0])
					continue;
				fprintf(out, "\t.ifdef %s_EDITION_%s\n", edition, EDITIONS[cell - 2]);
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
		fail(argv[1], "the first line must start with list, section, name or file");
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
