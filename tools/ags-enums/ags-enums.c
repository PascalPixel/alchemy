/*
 * ags-enums.c
 *
 * Give assembly a C header's enum constants, so a table in a .s file names an
 * id the way the C does. Each enumerator becomes an .equiv with its value. An
 * initializer is an integer literal or an earlier enumerator; anything else,
 * or a conditional inside an enum, is refused, since only C could evaluate it.
 */
#include "../shared/shared.h"
#include <ctype.h>

typedef struct
{
	Strings names;
	long *values;
} Constants;

static const char *path;
static unsigned line = 1;

static _Noreturn void
refuse(const char *what)
{
	fail("%s:%u: %s", path, line, what);
}

/* Skip blanks, comments and preprocessor lines; a conditional is refused. */
static const char *
skip(const char *at, bool inEnum)
{
	for (;;)
	{
		if (*at == '\n')
			line++;
		if (isspace((unsigned char) *at))
			at++;
		else if (at[0] == '/' && at[1] == '/')
			at += strcspn(at, "\n");
		else if (at[0] == '/' && at[1] == '*')
		{
			const char *end = strstr(at + 2, "*/");
			if (!end)
				refuse("unterminated comment");
			for (; at < end; at++)
				line += *at == '\n';
			at = end + 2;
		}
		else if (*at == '#')
		{
			if (inEnum)
				refuse("a preprocessor line inside an enum");
			at += strcspn(at, "\n");
		}
		else
			return at;
	}
}

static bool
identifierChar(char c)
{
	return isalnum((unsigned char) c) || c == '_';
}

/* The identifier or number at `at`, copied; `at` moves past it. */
static char *
token(const char **at)
{
	const char *start = *at, *end = start;
	while (identifierChar(*end))
		end++;
	if (end == start)
		refuse("expected a name or a number");
	char *text = allocate((size_t) (end - start) + 1, 1);
	memcpy(text, start, (size_t) (end - start));
	*at = end;
	return text;
}

static long
valueOf(const Constants *constants, const char *text, bool negative)
{
	if (isdigit((unsigned char) *text))
	{
		char *end;
		long value = strtol(text, &end, 0);
		while (*end == 'u' || *end == 'U' || *end == 'l' || *end == 'L')
			end++;
		if (*end)
			refuse("not an integer literal");
		return negative ? -value : value;
	}
	for (size_t i = 0; i < constants->names.len; i++)
		if (!strcmp(constants->names.items[i], text))
			return negative ? -constants->values[i] : constants->values[i];
	refuse("an initializer that is not a literal or an earlier enumerator");
}

static void
add(Constants *constants, const char *name, long value)
{
	if (hasString(&constants->names, name))
		refuse("an enumerator defined twice");
	constants->values = resize(constants->values, constants->names.len + 1,
							   sizeof(long));
	constants->values[constants->names.len] = value;
	addString(&constants->names, name);
}

/* One enum body, from after its opening brace to after its closing one. */
static const char *
enumerators(Constants *constants, const char *at)
{
	long next = 0;
	for (;;)
	{
		at = skip(at, true);
		if (*at == '}')
			return at + 1;
		char *name = token(&at);
		long value = next;
		at = skip(at, true);
		if (*at == '=')
		{
			at = skip(at + 1, true);
			bool negative = *at == '-';
			if (negative)
				at = skip(at + 1, true);
			char *text = token(&at);
			value = valueOf(constants, text, negative);
			free(text);
			at = skip(at, true);
		}
		add(constants, name, value);
		free(name);
		next = value + 1;
		if (*at == ',')
			at++;
		else if (*at != '}')
			refuse("expected , or } after an enumerator");
	}
}

int
main(int argc, char **argv)
{
	const char *usage = "usage: ags-enums HEADER.h OUT.inc";
	startTool(argc, argv, usage);
	if (argc != 3)
		fail("%s", usage);
	path = argv[1];
	Buffer header = readFile(path);
	Constants constants = {0};
	const char *at = (const char *) header.data;
	while (*(at = skip(at, false)))
	{
		if (!identifierChar(*at))
		{
			at++;
			continue;
		}
		char *word = token(&at);
		bool isEnum = !strcmp(word, "enum");
		free(word);
		if (!isEnum)
			continue;
		at = skip(at, false);
		if (identifierChar(*at))
			free(token(&at));
		at = skip(at, false);
		if (*at == '{')
			at = enumerators(&constants, at + 1);
	}
	Buffer out = {0};
	print(&out, "@ Written by ags-enums from %s: do not edit.\n", path);
	for (size_t i = 0; i < constants.names.len; i++)
		print(&out, "\t.equiv %s, %ld\n", constants.names.items[i],
			  constants.values[i]);
	writeFile(argv[2], &out);
	freeStrings(&constants.names);
	free(constants.values);
	free(header.data);
	free(out.data);
	return 0;
}
