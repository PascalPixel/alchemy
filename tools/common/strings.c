/*
 * strings.c
 *
 * Owned string lists and checked numeric input.
 * Lists copy their entries and release them together. Tokenizers copy ranges
 * directly, preserving empty delimited fields while discarding whitespace
 * between words. Numeric parsing rejects trailing or out-of-range input.
 */
#include "common.h"
#include <ctype.h>
#include <errno.h>

char *
copyString(const char *text)
{
	size_t size = strlen(text) + 1;
	char *result = allocate(size, 1);
	memcpy(result, text, size);
	return result;
}

char *
trim(char *text)
{
	while (isspace((unsigned char) *text))
		text++;
	size_t len = strlen(text);
	while (len && isspace((unsigned char) text[len - 1]))
		text[--len] = 0;
	return text;
}

/* Lists own their strings. Add a range directly so tokenizers copy only once. */
static void
addRange(Strings *strings, const char *start, size_t length)
{
	if (strings->len == strings->cap)
	{
		strings->cap = strings->cap ? strings->cap * 2 : 16;
		strings->items = resize(strings->items, strings->cap, sizeof(char *));
	}
	char *text = allocate(length + 1, 1);
	memcpy(text, start, length);
	strings->items[strings->len++] = text;
}

void
addString(Strings *strings, const char *text)
{
	addRange(strings, text, strlen(text));
}

void
addUniqueString(Strings *strings, const char *text)
{
	if (!hasString(strings, text))
		addString(strings, text);
}

bool
hasString(const Strings *strings, const char *text)
{
	for (size_t i = 0; i < strings->len; i++)
		if (!strcmp(strings->items[i], text))
			return true;
	return false;
}

static int
compareStrings(const void *a, const void *b)
{
	return strcmp(*(char *const *) a, *(char *const *) b);
}

void
sortStrings(Strings *strings)
{
	if (strings->len > 1)
		qsort(strings->items, strings->len, sizeof(char *), compareStrings);
}

/*
 * Unlike words(), preserve empty fields, including the one after a trailing
 * separator. TSV column positions depend on those fields.
 */
Strings
split(const char *text, char separator)
{
	Strings result = {0};
	const char *start = text;
	for (;; text++)
		if (*text == separator || !*text)
		{
			addRange(&result, start, text - start);
			if (!*text)
				break;
			start = text + 1;
		}
	return result;
}

void
freeStrings(Strings *strings)
{
	for (size_t i = 0; i < strings->len; i++)
		free(strings->items[i]);
	free(strings->items);
	*strings = (Strings){0};
}

/*
 * Split on runs of whitespace; blank input produces no words.
 */
Strings
words(const char *text)
{
	Strings out = {0};
	while (*text)
	{
		while (isspace((unsigned char) *text))
			text++;
		const char *start = text;
		while (*text && !isspace((unsigned char) *text))
			text++;
		if (text != start)
			addRange(&out, start, text - start);
	}
	return out;
}

uint64_t
decimalOrHex(const char *text, uint64_t maximum)
{
	return !strncmp(text, "0x", 2) ? number(text + 2, 16, maximum)
								   : number(text, 10, maximum);
}

/*
 * Require one complete nonnegative number in the requested base.
 */
uint64_t
number(const char *text, int base, uint64_t maximum)
{
	if (!*text || *text == '-' || isspace((unsigned char) *text))
		fail("invalid number: %s", text);
	errno = 0;
	char *end;
	unsigned long long value = strtoull(text, &end, base);
	if (errno || *end || value > maximum)
		fail("number out of range: %s", text);
	return value;
}
