/* Collect C and assembly includes, including assets not generated yet, for
 * Make. */
#include "../common/common.h"

typedef struct {
  char *path;
  Strings files;
} Entry;
static Entry *entries;
static size_t entryCount;
static Strings folders, generated;
static const char *build;

static void unique(Strings *s, const char *name) {
  if (!hasString(s, name))
    addString(s, name);
}

static char *resolve(const char *from, const char *name, bool c) {
  if (!strncmp(name, "games/", 6) || !strncmp(name, "recon/", 6))
    return copyString(name);
  if (c) {
    char *path = beside(from, name);
    if (exists(path))
      return path;
    free(path);
  }
  for (size_t i = 0; i < folders.len; i++) {
    char *path = formatString("%s/%s", folders.items[i], name);
    if (exists(path))
      return path;
    free(path);
  }
  for (size_t i = 0; i < generated.len; i++)
    if (!strncmp(name, generated.items[i], strlen(generated.items[i])))
      return formatString("%s/%s", build, name);
  return NULL;
}

static bool readable(const char *path) {
  const char *e = extension(path);
  return !strcmp(e, "c") || !strcmp(e, "h") || !strcmp(e, "s") ||
         !strcmp(e, "S") || !strcmp(e, "inc");
}

static size_t scan(const char *path) {
  for (size_t i = 0; i < entryCount; i++)
    if (!strcmp(entries[i].path, path))
      return i;
  /* Insert before recursing: include guards commonly form cycles. */
  size_t id = entryCount++;
  entries = resize(entries, entryCount, sizeof(Entry));
  entries[id] = (Entry){copyString(path), {0}};
  if (!exists(path))
    return id;
  Buffer data = readFile(path);
  Strings lines = split((char *)data.data, '\n');
  Strings files = {0};
  for (size_t i = 0; i < lines.len; i++) {
    char *line = trim(lines.items[i]);
    bool c = false;
    if (*line == '#') {
      line = trim(line + 1);
      if (strncmp(line, "include", 7))
        continue;
      line += 7;
      c = true;
    } else if (!strncmp(line, ".include", 8))
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
    char *resolved = resolve(path, line, c);
    if (!resolved)
      continue;
    unique(&files, resolved);
    if (readable(resolved) && exists(resolved)) {
      size_t child = scan(resolved);
      for (size_t j = 0; j < entries[child].files.len; j++)
        unique(&files, entries[child].files.items[j]);
    }
    free(resolved);
  }
  sortStrings(&files);
  entries[id].files = files;
  freeStrings(&lines);
  free(data.data);
  return id;
}

int main(int argc, char **argv) {
  const char *usage =
      "usage: ags-deps OUT.mk BUILD OBJDIR [-I DIR]... [-G NAME]... -- "
      "SOURCE...";
  startTool(argc, argv, usage);
  if (argc < 5)
    fail("%s", usage);
  build = argv[2];
  int at = 4;
  for (; at < argc && strcmp(argv[at], "--"); at++) {
    char *option = argv[at];
    Strings *list;
    if (!strncmp(option, "-I", 2))
      list = &folders;
    else if (!strncmp(option, "-G", 2))
      list = &generated;
    else
      fail("unknown option %s", option);
    const char *value = option + 2;
    if (!*value) {
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
  for (at++; at < argc; at++) {
    char *stem = copyString(argv[at]), *dot = strrchr(stem, '.');
    if (dot)
      *dot = 0;
    print(&out, "%s/%s.o:", argv[3], stem);
    free(stem);
    size_t id = scan(argv[at]);
    for (size_t i = 0; i < entries[id].files.len; i++) {
      print(&out, " %s", entries[id].files.items[i]);
      unique(&every, entries[id].files.items[i]);
    }
    byte(&out, '\n');
  }
  sortStrings(&every);
  for (size_t i = 0; i < every.len; i++)
    if (strncmp(every.items[i], build, strlen(build)))
      print(&out, "%s:\n", every.items[i]);
  writeFile(argv[1], &out);
  for (size_t i = 0; i < entryCount; i++) {
    free(entries[i].path);
    freeStrings(&entries[i].files);
  }
  free(entries);
  freeStrings(&every);
  freeStrings(&folders);
  freeStrings(&generated);
  free(out.data);
  return 0;
}
