/*
 * catalog.c
 *
 * Read and validate editable PO catalogues.
 * The empty PO entry supplies archive metadata. Other entries own their name,
 * numeric key, text, and encoded symbols. Keys must cover one contiguous range;
 * the input order remains available for generated declarations.
 */
#include "text.h"
#include <ctype.h>

static char *
unquote(const char *text)
{
	size_t len = strlen(text);
	if (len < 2 || *text != '"' || text[len - 1] != '"')
		fail("PO string is not quoted");
	Buffer out = {0};
	for (size_t i = 1; i + 1 < len; i++)
	{
		char c = text[i];
		if (c == '\\')
		{
			if (++i + 1 >= len)
				fail("incomplete PO escape");
			switch (text[i])
			{
			case '\\':
				c = '\\';
				break;
			case '"':
				c = '"';
				break;
			case 'n':
				c = '\n';
				break;
			case 'r':
				c = '\r';
				break;
			case 't':
				c = '\t';
				break;
			default:
				fail("unsupported PO escape: %c", text[i]);
			}
		}
		byte(&out, c);
	}
	reserve(&out, 0);
	return (char *) out.data;
}

/*
 * The unnamed empty-id entry is PO metadata. Ordinary entries transfer their
 * strings to the catalogue and are reset before parsing the next entry.
 */
static void
finish(Catalog *catalog, Message *message, bool *format)
{
	if (!message->id)
	{
		free(message->name);
		free(message->text);
		*message = (Message){0};
		return;
	}
	if (!message->text)
		fail("PO entry has no msgstr");
	if (!*message->id && !message->name)
	{
		Strings lines = split(message->text, '\n');
		for (size_t i = 0; i < lines.len; i++)
		{
			char *colon = strchr(lines.items[i], ':');
			if (!colon)
				continue;
			*colon = 0;
			char *key = trim(lines.items[i]), *value = trim(colon + 1);
			if (!strcmp(key, "X-Alchemy-Format"))
			{
				if (decimalOrHex(value, 1) != 1)
					fail("unsupported PO format");
				*format = true;
			}
			else if (!strcmp(key, "X-Alchemy-Target"))
			{
				free(catalog->target);
				catalog->target = copyString(value);
			}
			else if (!strcmp(key, "X-Alchemy-Symbol-Count"))
				catalog->alphabet = decimalOrHex(value, 4096);
			else if (!strncmp(key, "X-Alchemy-", 10))
				fail("%s is not an editable encoder option", key);
		}
		freeStrings(&lines);
		free(message->name);
		free(message->id);
		free(message->text);
	}
	else
	{
		catalog->messages =
			resize(catalog->messages, catalog->count + 1, sizeof(Message));
		catalog->messages[catalog->count++] = *message;
	}
	*message = (Message){0};
}

/*
 * Unique keys in the range 0..count-1 imply complete coverage. Message names
 * are also unique, except for the neutral unnamed-message context.
 */
static void
validateCatalog(const Catalog *catalog, bool format)
{
	if (!format || !catalog->alphabet || !catalog->target || !catalog->count ||
		catalog->count > 65536)
		fail("missing or invalid PO archive headers");
	Strings target = split(catalog->target, '-');
	if (target.len != 2 ||
		(strcmp(target.items[0], "tbs") && strcmp(target.items[0], "tla")) ||
		(strcmp(target.items[1], "ja") && strcmp(target.items[1], "en") &&
		 strcmp(target.items[1], "de") && strcmp(target.items[1], "es") &&
		 strcmp(target.items[1], "fr") && strcmp(target.items[1], "it")))
		fail("unknown PO target: %s", catalog->target);
	freeStrings(&target);
	bool *seen = allocate(catalog->count, sizeof(bool));
	Strings names = {0};
	addString(&names, "MsgEnd");
	for (size_t i = 0; i < catalog->count; i++)
	{
		Message *m = catalog->messages + i;
		size_t id = number(m->id, 10, catalog->count - 1);
		if (seen[id])
			fail("duplicate PO message key: %s", m->id);
		seen[id] = true;
		if (m->null && *m->text)
			fail("null message %s is not empty", m->id);
		if (!m->name || strcmp(m->name, "message"))
		{
			if (!m->name || strncmp(m->name, "Msg", 3) ||
				!isupper((unsigned char) m->name[3]))
				fail("PO message %s needs a Msg name or message context",
					 m->id);
			for (const char *p = m->name + 3; *p; p++)
				if (!isalnum((unsigned char) *p))
					fail("invalid message name: %s", m->name);
			if (hasString(&names, m->name))
				fail("%s names two messages", m->name);
			addString(&names, m->name);
		}
	}
	free(seen);
	freeStrings(&names);
}

/*
 * Continuation lines extend the active field. A blank line closes an entry,
 * including the implicit blank line supplied at EOF.
 */
Catalog
readCatalog(const char *path)
{
	Catalog catalog = {0};
	Message message = {0};
	bool format = false;
	char **active = NULL;
	Buffer data = readFile(path);
	Strings lines = split((char *) data.data, '\n');
	for (size_t i = 0; i <= lines.len; i++)
	{
		char *line = i == lines.len ? "" : trim(lines.items[i]);
		if (!*line)
		{
			finish(&catalog, &message, &format);
			active = NULL;
		}
		else if (!strncmp(line, "#,", 2))
		{
			Strings flags = split(line + 2, ',');
			for (size_t j = 0; j < flags.len; j++)
			{
				char *flag = trim(flags.items[j]);
				if (!strcmp(flag, "alchemy-null"))
					message.null = true;
				else if (!strcmp(flag, "alchemy-end-wait") ||
						 !strcmp(flag, "alchemy-end-now"))
					fail("obsolete PO flag %s: use inline {end_wait} or "
						 "{end_now}",
						 flag);
			}
			freeStrings(&flags);
		}
		else if (*line == '#')
			continue;
		else if (!strncmp(line, "msgctxt ", 8))
		{
			free(message.name);
			message.name = unquote(line + 8);
			active = &message.name;
		}
		else if (!strncmp(line, "msgid ", 6))
		{
			free(message.id);
			message.id = unquote(line + 6);
			active = &message.id;
		}
		else if (!strncmp(line, "msgstr ", 7))
		{
			free(message.text);
			message.text = unquote(line + 7);
			active = &message.text;
		}
		else if (*line == '"')
		{
			if (!active || !*active)
				fail("orphan PO continuation");
			char *tail = unquote(line),
				 *joined = formatString("%s%s", *active, tail);
			free(*active);
			free(tail);
			*active = joined;
		}
		else
			fail("unsupported PO line: %s", line);
	}
	freeStrings(&lines);
	free(data.data);
	validateCatalog(&catalog, format);
	return catalog;
}

void
freeCatalog(Catalog *catalog)
{
	for (size_t i = 0; i < catalog->count; i++)
	{
		Message *message = catalog->messages + i;
		free(message->name);
		free(message->id);
		free(message->text);
		free(message->symbols.data);
	}
	free(catalog->messages);
	free(catalog->target);
	*catalog = (Catalog){0};
}
