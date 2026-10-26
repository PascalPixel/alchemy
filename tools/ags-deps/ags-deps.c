/*
 * ags-deps.c
 *
 * Collect source and asset dependencies for Make.
 * Cache each file's direct includes, then traverse that graph separately for
 * each object. Generated includes can be named before they exist; they are
 * object prerequisites but must not cause the scanner to rebuild itself.
 */
#include "../common/common.h"

typedef struct
{
	char *path;
	Strings files;
	size_t *children;
} IncludeFile;
typedef struct
{
	IncludeFile *entries;
	size_t entryCount;
	Strings folders, generated;
	const char *build;
} Dependencies;

/*
 * C includes first search beside their source. Assembly includes use the
 * explicit search path, then known generated prefixes.
 */
static char *
resolve(const Dependencies *deps, const char *from, const char *name, bool c)
{
	if (!strncmp(name, "games/", 6) || !strncmp(name, "recon/", 6))
		return copyString(name);
	if (c)
	{
		char *path = beside(from, name);
		if (exists(path))
			return path;
		free(path);
	}
	for (size_t i = 0; i < deps->folders.len; i++)
	{
		char *path = formatString("%s/%s", deps->folders.items[i], name);
		if (exists(path))
			return path;
		free(path);
	}
	for (size_t i = 0; i < deps->generated.len; i++)
		if (!strncmp(name, deps->generated.items[i],
					 strlen(deps->generated.items[i])))
			return formatString("%s/%s", deps->build, name);
	return NULL;
}

static bool
readable(const char *path)
{
	const char *e = extension(path);
	return !strcmp(e, "c") || !strcmp(e, "h") || !strcmp(e, "s") ||
		   !strcmp(e, "S") || !strcmp(e, "inc");
}

/*
 * Cache direct edges before following them. Entry indices remain valid when
 * recursion grows the array; pointers to entries would not.
 */
static size_t
scan(Dependencies *deps, const char *path)
{
	for (size_t i = 0; i < deps->entryCount; i++)
		if (!strcmp(deps->entries[i].path, path))
			return i;
	/* Insert before recursing: include guards commonly form cycles. */
	size_t id = deps->entryCount++;
	deps->entries =
		resize(deps->entries, deps->entryCount, sizeof(IncludeFile));
	deps->entries[id] = (IncludeFile){.path = copyString(path)};
	if (!exists(path))
		return id;
	Buffer data = readFile(path);
	Strings lines = split((char *) data.data, '\n');
	Strings files = {0};
	for (size_t i = 0; i < lines.len; i++)
	{
		char *line = trim(lines.items[i]);
		bool c = false;
		if (*line == '#')
		{
			line = trim(line + 1);
			if (strncmp(line, "include", 7))
				continue;
			line += 7;
			c = true;
		}
		else if (!strncmp(line, ".include", 8))
			line += 8;
		else if (!strncmp(line, ".incbin", 7))
			line += 7;
		else
			continue;
		line = trim(line);
		char close = *line == '"' ? '"' : *line == '<' ? '>' : 0;
		if (!close)
			continue;
		line++;
		char *end = strchr(line, close);
		if (end)
			*end = 0;
		char *resolved = resolve(deps, path, line, c);
		if (!resolved)
			continue;
		addUniqueString(&files, resolved);
		free(resolved);
	}
	sortStrings(&files);
	deps->entries[id].files = files;
	deps->entries[id].children = allocate(files.len, sizeof(size_t));
	freeStrings(&lines);
	free(data.data);
	/* Store direct includes before recursion; a cycle can now revisit this file. */
	for (size_t i = 0; i < deps->entries[id].files.len; i++)
	{
		const char *child = deps->entries[id].files.items[i];
		size_t childId = SIZE_MAX;
		if (readable(child) && exists(child))
			childId = scan(deps, child);
		deps->entries[id].children[i] = childId;
	}
	return id;
}

/* Traversal has its own visited set; cached files contain only direct includes. */
static void
collectIncludes(const Dependencies *deps, size_t id, bool *visited,
				Strings *files)
{
	if (visited[id])
		return;
	visited[id] = true;
	const Strings *includes = &deps->entries[id].files;
	for (size_t i = 0; i < includes->len; i++)
	{
		const char *path = includes->items[i];
		addUniqueString(files, path);
		size_t child = deps->entries[id].children[i];
		if (child != SIZE_MAX)
			collectIncludes(deps, child, visited, files);
	}
}

static Strings
dependenciesOf(Dependencies *deps, const char *path)
{
	size_t root = scan(deps, path);
	bool *visited = allocate(deps->entryCount, sizeof(bool));
	Strings files = {0};
	collectIncludes(deps, root, visited, &files);
	sortStrings(&files);
	free(visited);
	return files;
}

int
main(int argc, char **argv)
{
	const char *usage =
		"usage: ags-deps OUT.mk BUILD OBJDIR [-I DIR]... [-G NAME]... -- "
		"SOURCE...";
	startTool(argc, argv, usage);
	if (argc < 5)
		fail("%s", usage);
	Dependencies deps = {0};
	deps.build = argv[2];
	int at = 4;
	for (; at < argc && strcmp(argv[at], "--"); at++)
	{
		char *option = argv[at];
		Strings *list;
		if (!strncmp(option, "-I", 2))
			list = &deps.folders;
		else if (!strncmp(option, "-G", 2))
			list = &deps.generated;
		else
			fail("unknown option %s", option);
		const char *value = option + 2;
		if (!*value)
		{
			if (++at == argc)
				fail("missing option value");
			value = argv[at];
		}
		addString(list, value);
	}
	if (at == argc)
		fail("missing -- before sources");
	Buffer out = {0};
	print(&out, "# Written by ags-deps: do not edit.\n");
	Strings every = {0};
	for (at++; at < argc; at++)
	{
		char *stem = copyString(argv[at]), *dot = strrchr(stem, '.');
		if (dot)
			*dot = 0;
		print(&out, "%s/%s.o:", argv[3], stem);
		free(stem);
		Strings files = dependenciesOf(&deps, argv[at]);
		for (size_t i = 0; i < files.len; i++)
		{
			print(&out, " %s", files.items[i]);
			addUniqueString(&every, files.items[i]);
		}
		byte(&out, '\n');
		freeStrings(&files);
	}
	/* An include added to any scanned source changes the dependency graph.
   * Generated includes are build products, not inputs to the scanner. */
	print(&out, "%s:", argv[1]);
	for (size_t i = 0; i < deps.entryCount; i++)
	{
		const char *path = deps.entries[i].path;
		size_t length = strlen(deps.build);
		bool inBuild = !strncmp(path, deps.build, length) &&
					   (path[length] == '/' || path[length] == 0);
		if (!inBuild && exists(path))
			print(&out, " %s", path);
	}
	byte(&out, '\n');
	sortStrings(&every);
	/* The sources every object reads, for the Makefile to find encoders' own
	 * dependency files beside the assets among them. */
	print(&out, "DEPENDENCIES :=");
	for (size_t i = 0; i < every.len; i++)
		if (strncmp(every.items[i], deps.build, strlen(deps.build)))
			print(&out, " %s", every.items[i]);
	byte(&out, '\n');
	for (size_t i = 0; i < every.len; i++)
		if (strncmp(every.items[i], deps.build, strlen(deps.build)))
			print(&out, "%s:\n", every.items[i]);
	writeFile(argv[1], &out);
	for (size_t i = 0; i < deps.entryCount; i++)
	{
		free(deps.entries[i].path);
		freeStrings(&deps.entries[i].files);
		free(deps.entries[i].children);
	}
	free(deps.entries);
	freeStrings(&every);
	freeStrings(&deps.folders);
	freeStrings(&deps.generated);
	free(out.data);
	return 0;
}
