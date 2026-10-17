/*
 * symbols.c
 *
 * Translate Unicode and inline message commands into an edition's alphabet.
 * Japanese text uses the editable character map and canonical decomposition.
 * Other editions use their single-byte character mapping. Literal text and
 * control arguments are emitted into the same halfword symbol stream.
 */
#include "text.h"
#include <ctype.h>
#include <utf8proc.h>

enum TextControl
{
	TEXT_LINE_BREAK = 3,
	TEXT_SET_COLOR = 8
};
/* One-based color indices proven by TBS's standard sprite and tile palettes.
 * Windows remap these logical colors to their own palette pens. */
enum TextColor
{
	TEXT_COLOR_RED = 3,
	TEXT_COLOR_YELLOW = 5,
	TEXT_COLOR_BLUE = 9,
	TEXT_COLOR_WHITE = 16
};

static const char *commands[32] = {NULL,
								   "page_break",
								   "end_wait",
								   "line_break",
								   "pause_60",
								   "pause_20_skippable",
								   "pause_120_skippable",
								   "reset_text_style",
								   "text_color",
								   "text_effect",
								   NULL,
								   NULL,
								   "control_12",
								   NULL,
								   NULL,
								   "control_15",
								   "protagonist_name",
								   "actor_name",
								   "argument_actor_name",
								   "argument_class_name",
								   "argument_item_name",
								   "argument_ability_name",
								   "argument_signed_number",
								   "argument_location_name",
								   "em_dash",
								   "plural_suffix",
								   "button_icon",
								   "possessive_suffix",
								   "control_28",
								   "article_class",
								   "end_now",
								   NULL};
static bool
takesArgument(unsigned n)
{
	return n == TEXT_SET_COLOR || n == 9 || n == 12 || n == 15 ||
		   (n >= 17 && n <= 20) || n == 26 || n == 28 || n == 29;
}

static unsigned
colorArgument(const char *arg)
{
	static const struct
	{
		const char *name;
		unsigned value;
	} colors[] = {{"red", TEXT_COLOR_RED},
				  {"yellow", TEXT_COLOR_YELLOW},
				  {"blue", TEXT_COLOR_BLUE},
				  {"white", TEXT_COLOR_WHITE}};
	for (size_t i = 0; i < sizeof(colors) / sizeof(*colors); i++)
		if (!strcmp(arg, colors[i].name))
			return colors[i].value;
	if (isalpha((unsigned char) *arg))
		fail("unknown text color: %s", arg);
	return number(arg, 10, 65535);
}

static const uint16_t cp1252[32] = {
	0x20ac, 0x81,	0x201a, 0x192,	0x201e, 0x2026, 0x2020, 0x2021,
	0x2c6,	0x2030, 0x160,	0x2039, 0x152,	0x8d,	0x17d,	0x8f,
	0x90,	0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
	0x2dc,	0x2122, 0x161,	0x203a, 0x153,	0x9d,	0x17e,	0x178};
/*
 * Unspecified or BOX entries reserve a slot but cannot match ordinary text.
 */
static Buffer
charmap(const char *path)
{
	Buffer data = readFile(path), result = {0};
	Strings lines = split((char *) data.data, '\n');
	for (size_t i = 0; i < lines.len; i++)
	{
		const char *s = lines.items[i];
		if (!*s || *s == '#')
			continue;
		Strings fields = split(s, '\t');
		utf8proc_int32_t code = 0xfffd;
		if (fields.len > 1)
		{
			const char *value = fields.items[1];
			if (!strcmp(value, "SPACE"))
				code = ' ';
			else if (strcmp(value, "BOX") &&
					 utf8proc_iterate((const uint8_t *) value, -1, &code) < 0)
				fail("invalid UTF-8 in charmap");
		}
		word(&result, code);
		freeStrings(&fields);
	}
	freeStrings(&lines);
	free(data.data);
	return result;
}

/*
 * Normalize only editions with a character map. Other editions map Unicode
 * directly to the single-byte alphabet, including the CP1252 extension.
 */
static void
literal(Buffer *symbols, Buffer *text, const Buffer *map)
{
	reserve(text, 0);
	uint8_t *normalized = NULL;
	const uint8_t *cursor = text->data;
	if (map->len)
	{
		normalized = utf8proc_NFD(cursor);
		if (!normalized)
			fail("invalid UTF-8 message");
		cursor = normalized;
	}
	while (*cursor)
	{
		utf8proc_int32_t codepoint;
		utf8proc_ssize_t length = utf8proc_iterate(cursor, -1, &codepoint);
		if (length < 0)
			fail("invalid UTF-8 message");
		cursor += length;
		unsigned value = UINT32_MAX;
		if (map->len)
		{
			for (size_t i = 0; i < map->len / 4; i++)
				if (codepoint != 0xfffd &&
					(uint32_t) codepoint == read32(map->data + 4 * i))
				{
					value = (unsigned) i + 32;
					break;
				}
		}
		else
		{
			if (codepoint < 128 || (codepoint >= 160 && codepoint <= 255))
				value = codepoint;
			else
				for (unsigned i = 0; i < 32; i++)
					if (cp1252[i] == codepoint)
						value = 128 + i;
		}
		if (value == UINT32_MAX)
			fail("character U+%04X is not in this edition's font",
				 (unsigned) codepoint);
		half(symbols, value);
	}
	free(normalized);
	text->len = 0;
}

/*
 * Flush literal text before controls. Doubled braces stay literal, and a PO
 * newline emits the same symbol as the explicit line-break command.
 */
static void
symbols(Message *message, const Buffer *map, size_t alphabet)
{
	if (message->null)
		return;
	Buffer text = {0};
	for (const char *s = message->text; *s;)
	{
		if (*s == '\n')
		{
			/* A PO newline starts the next display line. */
			literal(&message->symbols, &text, map);
			half(&message->symbols, TEXT_LINE_BREAK);
			s++;
			continue;
		}
		if ((*s == '{' && s[1] == '{') || (*s == '}' && s[1] == '}'))
		{
			byte(&text, *s);
			s += 2;
			continue;
		}
		if (*s != '{')
		{
			byte(&text, *s++);
			continue;
		}
		literal(&message->symbols, &text, map);
		const char *end = strchr(++s, '}');
		if (!end)
			fail("unclosed message markup");
		char *command = formatString("%.*s", (int) (end - s), s),
			 *arg = strchr(command, ':');
		if (arg)
			*arg++ = 0;
		if (!strcmp(command, "glyph") || !strcmp(command, "byte") ||
			!strcmp(command, "control"))
		{
			if (!arg)
				fail("message markup needs a value");
			half(&message->symbols,
				 number(arg, !strcmp(command, "control") ? 10 : 16, 65535));
		}
		else
		{
			unsigned opcode = 0;
			for (unsigned i = 1; i < 32; i++)
				if (commands[i] && !strcmp(command, commands[i]))
					opcode = i;
			if (!opcode || (arg && !takesArgument(opcode)))
				fail("invalid message command: %s", command);
			half(&message->symbols, opcode);
			if (arg)
				half(&message->symbols, opcode == TEXT_SET_COLOR
											? colorArgument(arg)
											: number(arg, 10, 65535));
		}
		free(command);
		s = end + 1;
	}
	literal(&message->symbols, &text, map);
	free(text.data);
	for (size_t i = 0; i < message->symbols.len; i += 2)
		if (read16(message->symbols.data + i) >= alphabet)
			fail("message symbol is outside the edition alphabet");
}

void
encodeCatalog(Catalog *catalog, const char *mapPath)
{
	Buffer map = {0};

	if (strstr(catalog->target, "-ja"))
	{
		if (!mapPath)
			mapPath = !strcmp(catalog->target, "tbs-ja")
						  ? "games/common/src/graphics/font/charmap_ja.tsv"
						  : "games/common/src/graphics/font/charmap_tla_ja.tsv";
		map = charmap(mapPath);
	}
	for (size_t i = 0; i < catalog->count; i++)
		symbols(catalog->messages + i, &map, catalog->alphabet);
	free(map.data);
}
