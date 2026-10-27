/*
 * ags-objects.c
 *
 * Write edition object lists and linker fragments in source order.
 * TSV rows describe images, placement, sections, and edition selection.
 * Overlays still written as one linker script each name their objects
 * themselves. Either way the same Make variables result, and the table
 * gives the main image its linker fragments.
 */
#include "../shared/shared.h"
#include <dirent.h>

static const char *defaults = ".text .rodata .data .padding";
typedef struct
{
	char *image, *place, *object, *sections;
	bool selected;
} Row;
static Row *rows;
static size_t count;
static const char *build;
/* Interpret columns once; the renderer works with named, edition-filtered rows. */
static Row
readRow(const Strings *fields, const char *language)
{
	Row row = {copyString(fields->items[0]), copyString(fields->items[1]),
			   copyString(fields->items[2]),
			   copyString(fields->len > 3 && *fields->items[3]
							  ? fields->items[3]
							  : defaults),
			   true};

	if (fields->len > 4 && *fields->items[4])
	{
		Strings editions = split(fields->items[4], ' ');
		row.selected = hasString(&editions, language);
		freeStrings(&editions);
	}
	return row;
}

static bool
links(const Row *row, const char *image)
{
	return row->selected && !strcmp(row->image, image);
}

static void
statement(Buffer *text, const char *object, const char *sections)
{
	if (!strcmp(object, "*"))
		print(text, "    *(%s)\n", sections);
	else
		print(text, "    \"*%s%s.o\"(%s)\n", strchr(object, ':') ? "" : "/",
			  object, sections);
}

/*
 * Ordinary code places text from all objects before their read-only data.
 * A custom section selection or wildcard keeps the table's literal order.
 */
static Buffer
render(const char *image, const char *place)
{
	Buffer text = {0};
	bool plain = !strcmp(image, "main") ? !strncmp(place, "rom_", 4)
										: !strcmp(place, "text");
	for (size_t i = 0; i < count; i++)
	{
		Row *r = rows + i;
		if (!strcmp(r->image, image) && !strcmp(r->place, place))
			plain &= !strcmp(r->sections, defaults) && strcmp(r->object, "*");
	}
	const char *passes[] = {".text .padding", ".rodata", ".data .overlays"};
	for (unsigned pass = 0; pass < (plain ? 3u : 1u); pass++)
		for (size_t i = 0; i < count; i++)
			if (links(rows + i, image) && !strcmp(rows[i].place, place))
				statement(&text, rows[i].object,
						  plain ? passes[pass] : rows[i].sections);
	reserve(&text, 0);
	return text;
}

static void
save(const char *suffix, Buffer *text)
{
	char *path = formatString("%s/%s", build, suffix);
	writeChanged(path, text);
	free(path);
	free(text->data);
	*text = (Buffer){0};
}

static void
addObject(Strings *objects, const char *object)
{
	if (strchr(object, '/') && !strchr(object, ':') && !strchr(object, '*') &&
		!hasString(objects, object))
		addString(objects, object);
}

static void
objectList(Buffer *text, const char *variable, const Strings *objects)
{
	print(text, "%s :=", variable);
	for (size_t i = 0; i < objects->len; i++)
		print(text, " \\\n\t$(BUILD)/obj/%s.o", objects->items[i]);
	byte(text, '\n');
}

/*
 * The script path reads object names only; placement remains owned by the
 * existing linker script.
 */
static Strings
named(const char *path)
{
	Buffer text = readFile(path);
	Strings objects = {0};
	char *p = (char *) text.data;
	while ((p = strstr(p, "\"*/")))
	{
		p += 3;
		char *end = strstr(p, ".o\"");
		if (!end)
			break;
		char save = *end;
		*end = 0;
		addObject(&objects, p);
		*end = save;
		p = end + 3;
	}
	free(text.data);
	return objects;
}

static void
readRows(const char *path, const char *language, Strings *images)
{
	Buffer input = readFile(path);
	Strings lines = split((char *) input.data, '\n');
	for (size_t i = 1; i < lines.len; i++)
	{
		if (!*trim(lines.items[i]))
			continue;
		Strings fields = split(lines.items[i], '\t');
		if (fields.len < 3 || fields.len > 5)
			fail("%s:%zu: expected three to five columns", path, i + 1);
		rows = resize(rows, count + 1, sizeof(Row));
		rows[count++] = readRow(&fields, language);
		if (strcmp(fields.items[0], "main") &&
			!hasString(images, fields.items[0]))
			addString(images, fields.items[0]);
		freeStrings(&fields);
	}
	freeStrings(&lines);
	free(input.data);
}

/*
 * A BSS section exists only when both its header and selected rows exist.
 * Empty extern/discard fragments are still valid generated files.
 */
static void
writeOverlay(const char *id, const Strings *bss, Strings *objects)
{
	bool calls = false;
	for (size_t j = 0; j < count; j++)
		if (links(rows + j, id))
		{
			addObject(objects, rows[j].object);
			calls |= !strcmp(rows[j].object, "libgcc.a:_call_via_rX");
		}
	Buffer text = {0};
	if (calls)
		print(&text, "EXTERN(_call_via_r0)\n");
	char *path = formatString("ld/%s/extern.ld", id);
	save(path, &text);
	free(path);
	const char *parts[] = {"text", "bss", "discard"};
	for (unsigned part = 0; part < 3; part++)
	{
		text = render(id, parts[part]);
		if (part == 1)
		{
			const char *header = NULL;
			for (size_t j = 1; j < bss->len; j++)
				if (!strncmp(bss->items[j], id, strlen(id)) &&
					bss->items[j][strlen(id)] == '\t')
					header = bss->items[j] + strlen(id) + 1;
			if (!!header != !!text.len)
				fail("overlay %s: bss header and rows disagree", id);
			if (header)
			{
				Buffer section = {0};
				print(&section, "    %s :\n    {\n%s    }\n", header,
					  text.data);
				free(text.data);
				text = section;
			}
		}
		path = formatString("ld/%s/%s.ld", id, parts[part]);
		save(path, &text);
		free(path);
	}
}

/*
 * Overlays still written as one linker script each, resource_<id>.ld in one
 * folder, name their objects themselves; the script keeps their placement.
 */
static Strings *
scriptOverlays(const char *folderPath, Strings *images)
{
	DIR *folder = opendir(folderPath);
	struct dirent *entry;
	while ((entry = readdir(folder)))
	{
		size_t n = strlen(entry->d_name);
		if (n > 12 && !strncmp(entry->d_name, "resource_", 9) &&
			!strcmp(entry->d_name + n - 3, ".ld"))
		{
			char *id = copyString(entry->d_name + 9);
			id[n - 12] = 0;
			addString(images, id);
			free(id);
		}
	}
	closedir(folder);
	sortStrings(images);
	Strings *objects = allocate(images->len, sizeof(Strings));
	for (size_t i = 0; i < images->len; i++)
	{
		char *path =
			formatString("%s/resource_%s.ld", folderPath, images->items[i]);
		objects[i] = named(path);
		free(path);
	}
	return objects;
}

int
main(int argc, char **argv)
{
	const char *usage = "usage: ags-objects OBJECTS.tsv OVERLAYS LANG BUILD\n"
						"OVERLAYS is overlays.tsv, or a folder of overlay "
						"scripts resource_<id>.ld";
	startTool(argc, argv, usage);
	if (argc != 5)
		fail("%s", usage);
	build = argv[4];
	const char *language = argv[3];
	Strings images = {0}, mainObjects = {0};
	Strings *objects = NULL;
	readRows(argv[1], language, &images);
	Strings places = {0};
	for (size_t i = 0; i < count; i++)
		if (!strcmp(rows[i].image, "main"))
		{
			if (!hasString(&places, rows[i].place))
				addString(&places, rows[i].place);
			if (links(rows + i, "main"))
				addObject(&mainObjects, rows[i].object);
		}
	for (size_t i = 0; i < places.len; i++)
	{
		Buffer text = render("main", places.items[i]);
		char *path = formatString("ld/%s.ld", places.items[i]);
		save(path, &text);
		free(path);
	}
	DIR *folder = opendir(argv[2]);
	if (folder)
	{
		closedir(folder);
		objects = scriptOverlays(argv[2], &images);
	}
	else
	{
		Buffer headers = readFile(argv[2]);
		Strings bss = split((char *) headers.data, '\n');
		objects = allocate(images.len, sizeof(Strings));
		for (size_t i = 0; i < images.len; i++)
			writeOverlay(images.items[i], &bss, objects + i);
		freeStrings(&bss);
		free(headers.data);
	}
	freeStrings(&places);
	for (size_t i = 0; i < count; i++)
	{
		free(rows[i].image);
		free(rows[i].place);
		free(rows[i].object);
		free(rows[i].sections);
	}
	free(rows);
	Buffer out = {0};
	print(&out,
		  "# Written by ags-objects from the object list: do not edit.\n");
	objectList(&out, "MAIN_OBJS", &mainObjects);
	print(&out, "OVERLAYS :=");
	for (size_t i = 0; i < images.len; i++)
		print(&out, " %s", images.items[i]);
	byte(&out, '\n');
	for (size_t i = 0; i < images.len; i++)
	{
		char *var = formatString("OVERLAY_OBJS_%s", images.items[i]);
		objectList(&out, var, objects + i);
		free(var);
		freeStrings(objects + i);
	}
	save("objects.mk", &out);
	free(objects);
	freeStrings(&images);
	freeStrings(&mainObjects);
	return 0;
}
