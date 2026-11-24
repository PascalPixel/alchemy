/*
 * ags-check.c
 *
 * The file standard of this repository, and the check that holds it.
 *
 *   git ls-files games tools | ags-check [--alarms] [LIST]
 *
 * Reads the names of the tracked files, one a line, from LIST or standard
 * input, and looks only at those under games/ and tools/. It prints one line
 * for each file that breaks the standard, "path: what is wrong", and exits
 * with status one when there is any. "make test" runs it.
 *
 * KINDS. A source file is one of: C (.c), a header (.h), assembly (.s) that
 * holds real instructions or data that is honestly shaped by hand, a table
 * (.tsv), a picture (.png), a sample (.wav), a song (.mid), dialogue (.po), a
 * linker script (.ld), a Makefile, and rom.sha1. Nothing else: no prose or
 * notes (.md, .txt), no .json, no scripts (.py, .sh, .js, .ts, .rs), no
 * binary dumps (.bin, .gba), and no assembler include (.inc) but the few in
 * INCLUDES below, each named with its reason.
 *
 * LIMITS. A .c file has at most 2,000 lines, a .h file 500, a .s file 1,500.
 *
 * TABLES. A .tsv file is rows of cells divided by tabs and ended by newlines.
 * Its first line is the names of its columns and nothing else: each name is
 * lower-case letters, digits and underscores, with no type after a colon, and
 * no name is given twice. A grid names its columns by number, 0 1 2 and so
 * on. A cell holds one thing: never a hexadecimal number (0x...), never a list
 * (several things divided by ", ", or several numbers divided by spaces).
 * There is no blank line, no comment line starting with #, and no carriage
 * return, and the file ends with a newline. A row is never longer than the
 * first line; it may be shorter, its empty last cells left off.
 *
 * NAMES. No file or folder has a placeholder for a name: a word and a number
 * that only tells it from its neighbours, like wave_52 or set_119. A name may
 * end in _NUMBER (before a language's _ja, _en, _de, _es, _fr or _it) only as
 * one of a series: the same name beside it in the same folder with the
 * numbers running from 0 or 1 without a gap, all written at one width, as in
 * digit_0 to digit_9 or the frames blast_00 to blast_30.
 *
 * ALARMS. These are things to look at, not errors, and --alarms lists them: a
 * table of more than 1,000 rows, of more than 16 columns (a grid aside) or of
 * more than 64 KB; rows that end in empty cells; cells that hold spaces; and
 * cells that hold a "/", since a file should not name another file's path
 * where that can be avoided.
 *
 * PENDING. What the tree does not yet meet is named in this file, in the
 * PENDING tables and at the line limits, never passed over in silence. Each
 * run counts it in a line starting "ags-check: pending", and --alarms lists
 * the files. A pending entry goes when its files are fixed.
 */
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *const KINDS[] = {"c",   "h",   "s",  "tsv", "png",
                                    "wav", "mid", "po", "ld"};
static const struct {
  const char *kind;
  long lines;
} LIMITS[] = {{"c", 2000}, {"h", 500}, {"s", 1500}};

/* The assembler includes that stay, as paths inside a game's folder. */
static const char *const INCLUDES[] = {
    /* Sets the edition's symbols for every assembly file. */
    "include/edition.inc",
    /* The head each bank's far-call table starts with. */
    "src/gamepak/far_call/table.inc",
    /* The far-call stub macro the tables and overlays share. */
    "src/gamepak/far_call/veneer.inc",
    /* The import list the build cuts down for each overlay. */
    "src/system/overlay/imports.inc",
    /* Glyph widths, placed inside the font's own assembly. */
    "src/graphics/font/glyph_widths.inc",
};

/* PENDING: a column name that is a file form, in monster/list.tsv. */
static const char *const PENDING_COLUMNS[] = {"pictures.lz"};
/* PENDING: numbered names that are no series, by the word they start with. */
static const char *const PENDING_NAMES[] = {
    "item_", "psynergy_", "icon_", "spr_",          "blank_",
    "set_",  "test_",     "map_",  "psynergy_test_"};
/* PENDING: the line limits hold under tools/ and not yet under games/. */
static const char *const LIMITS_HOLD_IN = "tools/";

#define COUNT(list) (sizeof(list) / sizeof(*(list)))

static bool alarms;
static int violations;

static bool among(const char *text, const char *const *list, size_t count) {
  for (size_t i = 0; i < count; i++)
    if (!strcmp(text, list[i]))
      return true;
  return false;
}

static void wrong(const char *path, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("%s: ", path);
  vprintf(format, args);
  putchar('\n');
  va_end(args);
  violations++;
}

/* Things counted rather than failed: what is pending, and the alarms. */
static struct Tally {
  bool pending;
  char what[160];
  int count;
} *tallies;
static size_t tallyCount;

static void tally(bool pending, const char *path, long size, const char *format,
                  ...) {
  char what[160];
  va_list args;
  va_start(args, format);
  vsnprintf(what, sizeof(what), format, args);
  va_end(args);
  if (alarms && path && size)
    printf("%s: %s: %s (%ld)\n", path, pending ? "pending" : "alarm", what,
           size);
  else if (alarms && path)
    printf("%s: %s: %s\n", path, pending ? "pending" : "alarm", what);
  size_t i = 0;
  while (i < tallyCount && strcmp(tallies[i].what, what))
    i++;
  if (i == tallyCount) {
    tallies = realloc(tallies, ++tallyCount * sizeof(*tallies));
    if (!tallies)
      exit(2);
    tallies[i].pending = pending;
    tallies[i].count = 0;
    strcpy(tallies[i].what, what);
  }
  tallies[i].count++;
}

static char *readText(const char *path, size_t *size) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    wrong(path, "cannot be read");
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  *size = (size_t)ftell(file);
  rewind(file);
  char *text = malloc(*size + 1);
  if (!text || fread(text, 1, *size, file) != *size)
    exit(2);
  text[*size] = '\0';
  fclose(file);
  return text;
}

/* Cut text at the next mark, and say where the piece after it starts. */
static char *cut(char *text, char mark) {
  char *end = strchr(text, mark);
  if (!end)
    return NULL;
  *end = '\0';
  return end + 1;
}

static bool holdsHex(const char *cell) {
  for (const char *at = strstr(cell, "0x"); at; at = strstr(at + 1, "0x"))
    if (isxdigit((unsigned char)at[2]) &&
        (at == cell || !(isalnum((unsigned char)at[-1]) || at[-1] == '_')))
      return true;
  return false;
}

static bool holdsList(const char *cell) {
  if (strstr(cell, ", "))
    return true;
  return strchr(cell, ' ') && strpbrk(cell, "0123456789") &&
         cell[strspn(cell, "0123456789- ")] == '\0';
}

static void checkTable(const char *path, const char *name) {
  size_t size;
  char *text = readText(path, &size);
  if (!text)
    return;
  if (!size || text[size - 1] != '\n')
    wrong(path, "does not end with a newline");
  if (memchr(text, '\r', size))
    wrong(path, "holds a carriage return");
  if (memchr(text, '\0', size))
    wrong(path, "holds a zero byte");

  /* The first line: the names of the columns. */
  char *row = cut(text, '\n');
  size_t columns = 1;
  for (const char *at = text; *at; at++)
    columns += *at == '\t';
  char **names = malloc(columns * sizeof(*names));
  if (!names)
    exit(2);
  bool grid = true;
  char *next = text;
  for (size_t i = 0; i < columns; i++) {
    char *column = names[i] = next, number[24];
    next = cut(column, '\t');
    snprintf(number, sizeof(number), "%zu", i);
    grid = grid && !strcmp(column, number);
    if (!*column)
      wrong(path, "column %zu has no name", i + 1);
    else if (column[strspn(column, "abcdefghijklmnopqrstuvwxyz0123456789_")]) {
      if (among(column, PENDING_COLUMNS, COUNT(PENDING_COLUMNS)))
        tally(true, path, 0, "column name that is not a plain word");
      else
        wrong(path, "column name \"%s\" is not a plain word", column);
    }
    for (size_t j = 0; j < i; j++)
      if (*column && !strcmp(column, names[j]))
        wrong(path, "column name \"%s\" is given twice", column);
  }

  long rows = 0;
  for (long line = 2; row && *row; line++, rows++) {
    char *after = cut(row, '\n'), *cell = row;
    if (!*row)
      wrong(path, "line %ld is blank", line);
    else if (*row == '#')
      wrong(path, "line %ld is a comment", line);
    else if (row[strlen(row) - 1] == '\t')
      tally(false, NULL, 0, "row that ends in empty cells");
    for (size_t i = 0; cell; i++) {
      char *following = cut(cell, '\t');
      const char *column = i < columns ? names[i] : "?";
      if (i == columns)
        wrong(path, "line %ld is longer than the first line", line);
      if (holdsHex(cell))
        wrong(path, "line %ld, %s: a hexadecimal number", line, column);
      if (holdsList(cell))
        wrong(path, "line %ld, %s: a list in one cell", line, column);
      else if (strchr(cell, ' '))
        tally(false, NULL, 0, "%s, %s: cell that holds a space", name, column);
      if (strchr(cell, '/'))
        tally(false, NULL, 0, "%s, %s: cell that holds a /", name, column);
      cell = following;
    }
    row = after;
  }
  if (rows > 1000)
    tally(false, path, rows, "table of more than 1,000 rows");
  if (columns > 16 && !grid)
    tally(false, path, (long)columns, "table of more than 16 columns");
  if (size > 64 * 1024)
    tally(false, path, (long)size, "table of more than 64 KB");
  free(names);
  free(text);
}

static void checkLines(const char *path, const char *kind, long limit) {
  size_t size;
  char *text = readText(path, &size);
  if (!text)
    return;
  long lines = 0;
  for (size_t i = 0; i < size; i++)
    lines += text[i] == '\n';
  free(text);
  if (lines <= limit)
    return;
  if (!strncmp(path, LIMITS_HOLD_IN, strlen(LIMITS_HOLD_IN)))
    wrong(path, "%ld lines, and a .%s file has at most %ld", lines, kind,
          limit);
  else
    tally(true, path, lines, "game .%s file of more than %ld lines", kind,
          limit);
}

static void checkKind(const char *path) {
  const char *name = strrchr(path, '/') + 1;
  const char *dot = strrchr(name, '.'), *kind = dot ? dot + 1 : "";
  if (!strcmp(name, "Makefile") || !strcmp(name, "rom.sha1"))
    return;
  if (!strcmp(kind, "tsv"))
    checkTable(path, name);
  for (size_t i = 0; i < COUNT(LIMITS); i++)
    if (!strcmp(kind, LIMITS[i].kind))
      checkLines(path, kind, LIMITS[i].lines);
  if (among(kind, KINDS, COUNT(KINDS)))
    return;
  /* games/GAME/REST: an include is allowed by the path inside its game. */
  const char *game = strchr(path, '/') + 1, *rest = strchr(game, '/');
  if (!strcmp(kind, "inc") && !strncmp(path, "games/", 6) && rest &&
      among(rest + 1, INCLUDES, COUNT(INCLUDES)))
    return;
  if (dot)
    wrong(path, ".%s is not a kind of source file", kind);
  else
    wrong(path, "a file without a kind");
}

/* A file or folder whose name ends in _NUMBER, filed under its series. */
static struct Numbered {
  char *series; /* the path with # for the number; a folder ends in / */
  long number;
  int width;
} *numbered;
static size_t numberedCount;

/* Note path[0..end) when its last part, before its first dot, is numbered. */
static void noteName(const char *path, size_t end, bool folder) {
  static const char *const LANGUAGES[] = {"_ja", "_en", "_de",
                                          "_es", "_fr", "_it"};
  size_t start = end;
  while (start && path[start - 1] != '/')
    start--;
  size_t stem = start, tail;
  while (stem < end && path[stem] != '.')
    stem++;
  tail = stem;
  for (size_t i = 0; i < COUNT(LANGUAGES); i++)
    if (stem - start > 3 && !strncmp(path + stem - 3, LANGUAGES[i], 3))
      tail = stem - 3;
  size_t digits = tail;
  while (digits > start && isdigit((unsigned char)path[digits - 1]))
    digits--;
  if (digits == tail || digits < start + 2 || path[digits - 1] != '_' ||
      !islower((unsigned char)path[digits - 2]))
    return;
  numbered = realloc(numbered, ++numberedCount * sizeof(*numbered));
  char *series = malloc(end + 3);
  if (!numbered || !series)
    exit(2);
  snprintf(series, end + 3, "%.*s#%.*s%s", (int)digits, path, (int)(end - tail),
           path + tail, folder ? "/" : "");
  numbered[numberedCount - 1] = (struct Numbered){
      series, strtol(path + digits, NULL, 10), (int)(tail - digits)};
}

static int byName(const void *a, const void *b) {
  const struct Numbered *one = a, *other = b;
  int order = strcmp(one->series, other->series);
  if (order)
    return order;
  if (one->number != other->number)
    return one->number < other->number ? -1 : 1;
  return one->width - other->width;
}

static void checkNames(void) {
  if (numberedCount)
    qsort(numbered, numberedCount, sizeof(*numbered), byName);
  for (size_t first = 0, end; first < numberedCount; first = end) {
    /* A folder is noted once for each file in it: count it once. */
    long members = 1;
    bool series = numbered[first].number <= 1;
    for (end = first + 1; end < numberedCount &&
                          !strcmp(numbered[end].series, numbered[first].series);
         end++) {
      if (!byName(&numbered[end], &numbered[end - 1]))
        continue;
      members++;
      series = series && numbered[end].number == numbered[end - 1].number + 1 &&
               numbered[end].width == numbered[first].width;
    }
    if (series && members > 1)
      continue;
    const char *path = numbered[first].series, *name = strrchr(path, '#');
    const char *word = name;
    while (word > path && word[-1] != '/')
      word--;
    char start[1024];
    snprintf(start, sizeof(start), "%.*s", (int)(name - word), word);
    bool pending = among(start, PENDING_NAMES, COUNT(PENDING_NAMES));
    for (size_t i = first; i < end; i++) {
      if (i > first && !byName(&numbered[i], &numbered[i - 1]))
        continue;
      char shown[1024];
      snprintf(shown, sizeof(shown), "%.*s%0*ld%s", (int)(name - path), path,
               numbered[i].width, numbered[i].number, name + 1);
      if (pending)
        tally(true, shown, 0, "numbered name that is not a series");
      else
        wrong(shown, "a numbered name that is not one of a series");
    }
  }
}

int main(int argc, char **argv) {
  FILE *list = stdin;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--alarms"))
      alarms = true;
    else if (argv[i][0] == '-' || list != stdin ||
             !(list = fopen(argv[i], "r"))) {
      fprintf(stderr, "usage: git ls-files games tools | "
                      "ags-check [--alarms] [LIST]\n");
      return 2;
    }
  }
  char path[1024];
  long files = 0;
  while (fgets(path, sizeof(path), list)) {
    path[strcspn(path, "\n")] = '\0';
    if (strncmp(path, "games/", 6) && strncmp(path, "tools/", 6))
      continue;
    files++;
    checkKind(path);
    size_t length = strlen(path);
    noteName(path, length, false);
    for (size_t i = 6; i < length; i++)
      if (path[i] == '/')
        noteName(path, i, true);
  }
  checkNames();
  for (size_t i = 0; i < tallyCount; i++)
    if (tallies[i].pending || alarms)
      printf("ags-check: %s: %d x %s\n",
             tallies[i].pending ? "pending" : "alarm", tallies[i].count,
             tallies[i].what);
  printf("ags-check: %ld files, %d against the standard\n", files, violations);
  return violations != 0;
}
