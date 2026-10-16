/* Write each edition's object list and linker fragments in source-defined
 * order. */
#include "../common/common.h"
#include <dirent.h>

static const char *defaults = ".text .rodata .data .padding";
typedef struct {
  Strings fields;
} Row;
static Row *rows;
static size_t count;
static const char *language, *build;
static bool links(const Row *r, const char *image) {
  if (strcmp(r->fields.items[0], image))
    return false;
  if (r->fields.len < 5 || !*r->fields.items[4])
    return true;
  Strings langs = split(r->fields.items[4], ' ');
  bool yes = hasString(&langs, language);
  freeStrings(&langs);
  return yes;
}

static const char *sections(const Row *r) {
  return r->fields.len > 3 && *r->fields.items[3] ? r->fields.items[3]
                                                  : defaults;
}

static void statement(Buffer *text, const char *object, const char *sections) {
  if (!strcmp(object, "*"))
    print(text, "    *(%s)\n", sections);
  else
    print(text, "    \"*%s%s.o\"(%s)\n", strchr(object, ':') ? "" : "/", object,
          sections);
}

static Buffer render(const char *image, const char *place) {
  Buffer text = {0};
  bool plain = !strcmp(image, "main") ? !strncmp(place, "rom_", 4)
                                      : !strcmp(place, "text");
  for (size_t i = 0; i < count; i++) {
    Row *r = rows + i;
    if (!strcmp(r->fields.items[0], image) &&
        !strcmp(r->fields.items[1], place))
      plain &=
          !strcmp(sections(r), defaults) && strcmp(r->fields.items[2], "*");
  }
  const char *passes[] = {".text .padding", ".rodata", ".data .overlays"};
  for (unsigned pass = 0; pass < (plain ? 3u : 1u); pass++)
    for (size_t i = 0; i < count; i++)
      if (links(rows + i, image) && !strcmp(rows[i].fields.items[1], place))
        statement(&text, rows[i].fields.items[2],
                  plain ? passes[pass] : sections(rows + i));
  reserve(&text, 0);
  return text;
}

static void save(const char *suffix, Buffer *text) {
  char *path = formatString("%s/%s", build, suffix);
  writeChanged(path, text);
  free(path);
  free(text->data);
  *text = (Buffer){0};
}

static void addObject(Strings *objects, const char *object) {
  if (strchr(object, '/') && !strchr(object, ':') && !strchr(object, '*') &&
      !hasString(objects, object))
    addString(objects, object);
}

static void objectList(Buffer *text, const char *variable,
                       const Strings *objects) {
  print(text, "%s :=", variable);
  for (size_t i = 0; i < objects->len; i++)
    print(text, " \\\n\t$(BUILD)/obj/%s.o", objects->items[i]);
  byte(text, '\n');
}

static Strings named(const char *path) {
  Buffer text = readFile(path);
  Strings objects = {0};
  char *p = (char *)text.data;
  while ((p = strstr(p, "\"*/"))) {
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

int main(int argc, char **argv) {
  const char *usage =
      "usage: ags-objects OBJECTS.tsv OVERLAYS.tsv LANG BUILD\n       "
      "ags-objects --scripts MAIN.ld OVERLAY_DIR BUILD";
  startTool(argc, argv, usage);
  if (argc != 5)
    fail("%s", usage);
  build = argv[4];
  Strings images = {0}, mainObjects = {0};
  Strings *objects = NULL;
  if (!strcmp(argv[1], "--scripts")) {
    DIR *folder = opendir(argv[3]);
    if (!folder)
      fail("%s: cannot open directory", argv[3]);
    struct dirent *entry;
    while ((entry = readdir(folder))) {
      size_t n = strlen(entry->d_name);
      if (n > 12 && !strncmp(entry->d_name, "resource_", 9) &&
          !strcmp(entry->d_name + n - 3, ".ld")) {
        char *id = copyString(entry->d_name + 9);
        id[n - 12] = 0;
        addString(&images, id);
        free(id);
      }
    }
    closedir(folder);
    sortStrings(&images);
    objects = allocate(images.len, sizeof(Strings));
    for (size_t i = 0; i < images.len; i++) {
      char *path = formatString("%s/resource_%s.ld", argv[3], images.items[i]);
      objects[i] = named(path);
      free(path);
    }
    mainObjects = named(argv[2]);
  } else {
    language = argv[3];
    Buffer input = readFile(argv[1]);
    Strings lines = split((char *)input.data, '\n');
    for (size_t i = 1; i < lines.len; i++) {
      if (!*trim(lines.items[i]))
        continue;
      Strings fields = split(lines.items[i], '\t');
      if (fields.len < 3 || fields.len > 5)
        fail("%s:%zu: expected three to five columns", argv[1], i + 1);
      rows = resize(rows, count + 1, sizeof(Row));
      rows[count++].fields = fields;
      if (strcmp(fields.items[0], "main") &&
          !hasString(&images, fields.items[0]))
        addString(&images, fields.items[0]);
    }
    Strings places = {0};
    for (size_t i = 0; i < count; i++)
      if (!strcmp(rows[i].fields.items[0], "main")) {
        if (!hasString(&places, rows[i].fields.items[1]))
          addString(&places, rows[i].fields.items[1]);
        if (links(rows + i, "main"))
          addObject(&mainObjects, rows[i].fields.items[2]);
      }
    for (size_t i = 0; i < places.len; i++) {
      Buffer text = render("main", places.items[i]);
      char *path = formatString("ld/%s.ld", places.items[i]);
      save(path, &text);
      free(path);
    }
    Buffer headers = readFile(argv[2]);
    Strings bss = split((char *)headers.data, '\n');
    objects = allocate(images.len, sizeof(Strings));
    for (size_t i = 0; i < images.len; i++) {
      const char *id = images.items[i];
      bool calls = false;
      for (size_t j = 0; j < count; j++)
        if (links(rows + j, id)) {
          addObject(objects + i, rows[j].fields.items[2]);
          calls |= !strcmp(rows[j].fields.items[2], "libgcc.a:_call_via_rX");
        }
      Buffer text = {0};
      if (calls)
        print(&text, "EXTERN(_call_via_r0)\n");
      char *path = formatString("ld/%s/extern.ld", id);
      save(path, &text);
      free(path);
      const char *parts[] = {"text", "bss", "discard"};
      for (unsigned part = 0; part < 3; part++) {
        text = render(id, parts[part]);
        if (part == 1) {
          const char *header = NULL;
          for (size_t j = 1; j < bss.len; j++)
            if (!strncmp(bss.items[j], id, strlen(id)) &&
                bss.items[j][strlen(id)] == '\t')
              header = bss.items[j] + strlen(id) + 1;
          if (!!header != !!text.len)
            fail("overlay %s: bss header and rows disagree", id);
          if (header) {
            Buffer section = {0};
            print(&section, "    %s :\n    {\n%s    }\n", header, text.data);
            free(text.data);
            text = section;
          }
        }
        path = formatString("ld/%s/%s.ld", id, parts[part]);
        save(path, &text);
        free(path);
      }
    }
    freeStrings(&places);
    freeStrings(&bss);
    free(headers.data);
    freeStrings(&lines);
    free(input.data);
    for (size_t i = 0; i < count; i++)
      freeStrings(&rows[i].fields);
    free(rows);
  }
  Buffer out = {0};
  print(&out, "# Written by ags-objects from the object list: do not edit.\n");
  objectList(&out, "MAIN_OBJS", &mainObjects);
  print(&out, "OVERLAYS :=");
  for (size_t i = 0; i < images.len; i++)
    print(&out, " %s", images.items[i]);
  byte(&out, '\n');
  for (size_t i = 0; i < images.len; i++) {
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
