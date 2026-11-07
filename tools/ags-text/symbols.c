/*
 * symbols.c
 *
 * Translate Unicode and inline message commands into an edition's alphabet.
 * Japanese text uses the editable character map, with voiced kana split into
 * kana and mark.
 * Other editions use their single-byte character mapping. Literal text and
 * control arguments are emitted into the same halfword symbol stream.
 */
#include "text.h"
#include <ctype.h>

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
 * Every kana Unicode decomposes canonically: a base kana and the voicing
 * mark (U+3099) or the semi-voicing mark (U+309A). The Japanese font draws
 * the mark as a symbol of its own, after its kana.
 */
static const uint16_t voicedKana[][3] = {
	{0x304c, 0x304b, 0x3099}, {0x304e, 0x304d, 0x3099}, {0x3050, 0x304f, 0x3099},
	{0x3052, 0x3051, 0x3099}, {0x3054, 0x3053, 0x3099}, {0x3056, 0x3055, 0x3099},
	{0x3058, 0x3057, 0x3099}, {0x305a, 0x3059, 0x3099}, {0x305c, 0x305b, 0x3099},
	{0x305e, 0x305d, 0x3099}, {0x3060, 0x305f, 0x3099}, {0x3062, 0x3061, 0x3099},
	{0x3065, 0x3064, 0x3099}, {0x3067, 0x3066, 0x3099}, {0x3069, 0x3068, 0x3099},
	{0x3070, 0x306f, 0x3099}, {0x3071, 0x306f, 0x309a}, {0x3073, 0x3072, 0x3099},
	{0x3074, 0x3072, 0x309a}, {0x3076, 0x3075, 0x3099}, {0x3077, 0x3075, 0x309a},
	{0x3079, 0x3078, 0x3099}, {0x307a, 0x3078, 0x309a}, {0x307c, 0x307b, 0x3099},
	{0x307d, 0x307b, 0x309a}, {0x3094, 0x3046, 0x3099}, {0x309e, 0x309d, 0x3099},
	{0x30ac, 0x30ab, 0x3099}, {0x30ae, 0x30ad, 0x3099}, {0x30b0, 0x30af, 0x3099},
	{0x30b2, 0x30b1, 0x3099}, {0x30b4, 0x30b3, 0x3099}, {0x30b6, 0x30b5, 0x3099},
	{0x30b8, 0x30b7, 0x3099}, {0x30ba, 0x30b9, 0x3099}, {0x30bc, 0x30bb, 0x3099},
	{0x30be, 0x30bd, 0x3099}, {0x30c0, 0x30bf, 0x3099}, {0x30c2, 0x30c1, 0x3099},
	{0x30c5, 0x30c4, 0x3099}, {0x30c7, 0x30c6, 0x3099}, {0x30c9, 0x30c8, 0x3099},
	{0x30d0, 0x30cf, 0x3099}, {0x30d1, 0x30cf, 0x309a}, {0x30d3, 0x30d2, 0x3099},
	{0x30d4, 0x30d2, 0x309a}, {0x30d6, 0x30d5, 0x3099}, {0x30d7, 0x30d5, 0x309a},
	{0x30d9, 0x30d8, 0x3099}, {0x30da, 0x30d8, 0x309a}, {0x30dc, 0x30db, 0x3099},
	{0x30dd, 0x30db, 0x309a}, {0x30f4, 0x30a6, 0x3099}, {0x30f7, 0x30ef, 0x3099},
	{0x30f8, 0x30f0, 0x3099}, {0x30f9, 0x30f1, 0x3099}, {0x30fa, 0x30f2, 0x3099},
	{0x30fe, 0x30fd, 0x3099}};

/*
 * Reads one code point. Returns its length in bytes, or -1 for bytes that
 * are not UTF-8: a stray or missing continuation byte, an overlong form, a
 * surrogate or a value past U+10FFFF.
 */
static int
decodeUtf8(const uint8_t *s, int32_t *code)
{
	uint32_t c = s[0], minimum;
	int length;
	if (c < 0x80)
	{
		*code = (int32_t) c;
		return 1;
	}
	if ((c & 0xe0) == 0xc0)
		length = 2, c &= 0x1f, minimum = 0x80;
	else if ((c & 0xf0) == 0xe0)
		length = 3, c &= 0x0f, minimum = 0x800;
	else if ((c & 0xf8) == 0xf0)
		length = 4, c &= 0x07, minimum = 0x10000;
	else
		return -1;
	for (int i = 1; i < length; i++)
	{
		if ((s[i] & 0xc0) != 0x80)
			return -1;
		c = c << 6 | (s[i] & 0x3f);
	}
	if (c < minimum || c > 0x10ffff || (c >= 0xd800 && c <= 0xdfff))
		return -1;
	*code = (int32_t) c;
	return length;
}

/* A character map's slot for the code point, or UINT32_MAX for none. */
static unsigned
mapped(const Buffer *map, int32_t codepoint)
{
	for (size_t i = 0; i < map->len / 4; i++)
		if (codepoint != 0xfffd &&
			(uint32_t) codepoint == read32(map->data + 4 * i))
			return (unsigned) i + 32;
	return UINT32_MAX;
}
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
		int32_t code = 0xfffd;
		if (fields.len > 1)
		{
			const char *value = fields.items[1];
			if (!strcmp(value, "SPACE"))
				code = ' ';
			else if (strcmp(value, "BOX") &&
					 decodeUtf8((const uint8_t *) value, &code) < 0)
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
 * Editions with a character map split each voiced kana into its kana and
 * its mark, as Unicode's canonical decomposition does. Other editions map
 * Unicode directly to the single-byte alphabet, including the CP1252
 * extension.
 */
static void
literal(Buffer *symbols, Buffer *text, const Buffer *map)
{
	reserve(text, 0);
	const uint8_t *cursor = text->data;
	while (*cursor)
	{
		int32_t codepoint;
		int length = decodeUtf8(cursor, &codepoint);
		if (length < 0)
			fail("invalid UTF-8 message");
		cursor += length;
		unsigned value = UINT32_MAX;
		if (map->len)
		{
			for (size_t i = 0; i < sizeof(voicedKana) / sizeof(*voicedKana); i++)
				if (voicedKana[i][0] == codepoint)
				{
					unsigned kana = mapped(map, voicedKana[i][1]);
					if (kana == UINT32_MAX)
						fail("character U+%04X is not in this edition's font",
							 (unsigned) voicedKana[i][1]);
					half(symbols, kana);
					codepoint = voicedKana[i][2];
					break;
				}
			value = mapped(map, codepoint);
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
						  ? "games/tbs/src/graphics/font/charmap_ja.tsv"
						  : "games/tla/src/graphics/font/charmap_tla_ja.tsv";
		map = charmap(mapPath);
	}
	for (size_t i = 0; i < catalog->count; i++)
		symbols(catalog->messages + i, &map, catalog->alphabet);
	free(map.data);
}
