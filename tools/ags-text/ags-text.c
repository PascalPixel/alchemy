/* Encode editable PO messages as the game's context-dependent Huffman archive.
 */
#include "../common/assembly.h"
#include <ctype.h>
#include <utf8proc.h>

typedef struct {
  char *name, *id, *text;
  bool null;
  Buffer symbols;
} Message;
typedef struct {
  Message *messages;
  size_t count, alphabet;
  char *target;
} Catalog;
typedef struct {
  unsigned symbol;
  uint64_t count;
  Buffer path;
} Transition;
typedef struct {
  Transition *transitions;
  size_t count;
  Buffer leaves;
  Bits tree;
} Context;
typedef struct {
  uint64_t count;
  size_t order;
  int left, right, transition;
} Node;

static char *unquote(const char *s) {
  size_t len = strlen(s);
  if (len < 2 || *s != '"' || s[len - 1] != '"')
    fail("PO string is not quoted");
  Buffer out = {0};
  for (size_t i = 1; i + 1 < len; i++) {
    char c = s[i];
    if (c == '\\') {
      if (++i + 1 >= len)
        fail("incomplete PO escape");
      switch (s[i]) {
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
        fail("unsupported PO escape: %c", s[i]);
      }
    }
    byte(&out, c);
  }
  reserve(&out, 0);
  return (char *)out.data;
}

static void finish(Catalog *c, Message *m, bool *format) {
  if (!m->id) {
    free(m->name);
    free(m->text);
    *m = (Message){0};
    return;
  }
  if (!m->text)
    fail("PO entry has no msgstr");
  if (!*m->id && !m->name) {
    Strings lines = split(m->text, '\n');
    for (size_t i = 0; i < lines.len; i++) {
      char *colon = strchr(lines.items[i], ':');
      if (!colon)
        continue;
      *colon = 0;
      char *key = trim(lines.items[i]), *value = trim(colon + 1);
      if (!strcmp(key, "X-Alchemy-Format")) {
        if (decimalOrHex(value, 1) != 1)
          fail("unsupported PO format");
        *format = true;
      } else if (!strcmp(key, "X-Alchemy-Target")) {
        free(c->target);
        c->target = copyString(value);
      } else if (!strcmp(key, "X-Alchemy-Symbol-Count"))
        c->alphabet = decimalOrHex(value, 4096);
      else if (!strncmp(key, "X-Alchemy-", 10))
        fail("%s is not an editable encoder option", key);
    }
    freeStrings(&lines);
    free(m->name);
    free(m->id);
    free(m->text);
  } else {
    c->messages = resize(c->messages, c->count + 1, sizeof(Message));
    c->messages[c->count++] = *m;
  }
  *m = (Message){0};
}

static Catalog readCatalog(const char *path) {
  Catalog c = {0};
  Message m = {0};
  bool format = false;
  char **active = NULL;
  Buffer data = readFile(path);
  Strings lines = split((char *)data.data, '\n');
  for (size_t i = 0; i <= lines.len; i++) {
    char *s = i == lines.len ? "" : trim(lines.items[i]);
    if (!*s) {
      finish(&c, &m, &format);
      active = NULL;
    } else if (!strncmp(s, "#,", 2)) {
      Strings flags = split(s + 2, ',');
      for (size_t j = 0; j < flags.len; j++)
        if (!strcmp(trim(flags.items[j]), "alchemy-null"))
          m.null = true;
      freeStrings(&flags);
    } else if (*s == '#')
      continue;
    else if (!strncmp(s, "msgctxt ", 8)) {
      free(m.name);
      m.name = unquote(s + 8);
      active = &m.name;
    } else if (!strncmp(s, "msgid ", 6)) {
      free(m.id);
      m.id = unquote(s + 6);
      active = &m.id;
    } else if (!strncmp(s, "msgstr ", 7)) {
      free(m.text);
      m.text = unquote(s + 7);
      active = &m.text;
    } else if (*s == '"') {
      if (!active || !*active)
        fail("orphan PO continuation");
      char *tail = unquote(s), *joined = formatString("%s%s", *active, tail);
      free(*active);
      free(tail);
      *active = joined;
    } else
      fail("unsupported PO line: %s", s);
  }
  freeStrings(&lines);
  free(data.data);
  if (!format || !c.alphabet || !c.target || !c.count || c.count > 65536)
    fail("missing or invalid PO archive headers");
  Strings target = split(c.target, '-');
  if (target.len != 2 ||
      (strcmp(target.items[0], "tbs") && strcmp(target.items[0], "tla")) ||
      (strcmp(target.items[1], "ja") && strcmp(target.items[1], "en") &&
       strcmp(target.items[1], "de") && strcmp(target.items[1], "es") &&
       strcmp(target.items[1], "fr") && strcmp(target.items[1], "it")))
    fail("unknown PO target: %s", c.target);
  freeStrings(&target);
  bool *seen = allocate(c.count, sizeof(bool));
  Strings names = {0};
  addString(&names, "MsgEnd");
  for (size_t i = 0; i < c.count; i++) {
    Message *m = c.messages + i;
    size_t id = number(m->id, 10, c.count - 1);
    if (seen[id])
      fail("duplicate PO message key: %s", m->id);
    seen[id] = true;
    if (m->null && *m->text)
      fail("null message %s is not empty", m->id);
    if (!m->name || strcmp(m->name, "message")) {
      if (!m->name || strncmp(m->name, "Msg", 3) ||
          !isupper((unsigned char)m->name[3]))
        fail("PO message %s needs a Msg name or message context", m->id);
      for (const char *p = m->name + 3; *p; p++)
        if (!isalnum((unsigned char)*p))
          fail("invalid message name: %s", m->name);
      if (hasString(&names, m->name))
        fail("%s names two messages", m->name);
      addString(&names, m->name);
    }
  }
  free(seen);
  freeStrings(&names);
  return c;
}

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
static bool takesArgument(unsigned n) {
  return n == 8 || n == 9 || n == 12 || n == 15 || (n >= 17 && n <= 20) ||
         n == 26 || n == 28 || n == 29;
}

static const uint16_t cp1252[32] = {
    0x20ac, 0x81,   0x201a, 0x192,  0x201e, 0x2026, 0x2020, 0x2021,
    0x2c6,  0x2030, 0x160,  0x2039, 0x152,  0x8d,   0x17d,  0x8f,
    0x90,   0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x2dc,  0x2122, 0x161,  0x203a, 0x153,  0x9d,   0x17e,  0x178};
static Buffer charmap(const char *path) {
  Buffer data = readFile(path), result = {0};
  Strings lines = split((char *)data.data, '\n');
  for (size_t i = 0; i < lines.len; i++) {
    const char *s = lines.items[i];
    if (!*s || *s == '#')
      continue;
    Strings fields = split(s, '\t');
    utf8proc_int32_t code = 0xfffd;
    if (fields.len > 1) {
      const char *value = fields.items[1];
      if (!strcmp(value, "SPACE"))
        code = ' ';
      else if (strcmp(value, "BOX") &&
               utf8proc_iterate((const uint8_t *)value, -1, &code) < 0)
        fail("invalid UTF-8 in charmap");
    }
    word(&result, code);
    freeStrings(&fields);
  }
  freeStrings(&lines);
  free(data.data);
  return result;
}

static void literal(Buffer *symbols, Buffer *text, const Buffer *map) {
  reserve(text, 0);
  uint8_t *normalized = NULL;
  const uint8_t *s = text->data;
  if (map->len) {
    normalized = utf8proc_NFD(s);
    if (!normalized)
      fail("invalid UTF-8 message");
    s = normalized;
  }
  while (*s) {
    utf8proc_int32_t cp;
    utf8proc_ssize_t n = utf8proc_iterate(s, -1, &cp);
    if (n < 0)
      fail("invalid UTF-8 message");
    s += n;
    unsigned value = UINT32_MAX;
    if (map->len) {
      for (size_t i = 0; i < map->len / 4; i++)
        if (cp != 0xfffd && (uint32_t)cp == read32(map->data + 4 * i)) {
          value = (unsigned)i + 32;
          break;
        }
    } else {
      if (cp < 128 || (cp >= 160 && cp <= 255))
        value = cp;
      else
        for (unsigned i = 0; i < 32; i++)
          if (cp1252[i] == cp)
            value = 128 + i;
    }
    if (value == UINT32_MAX)
      fail("character U+%04X is not in this edition's font", (unsigned)cp);
    half(symbols, value);
  }
  free(normalized);
  text->len = 0;
}

static void symbols(Message *m, const Buffer *map, size_t alphabet) {
  if (m->null)
    return;
  Buffer text = {0};
  for (const char *s = m->text; *s;) {
    if ((*s == '{' && s[1] == '{') || (*s == '}' && s[1] == '}')) {
      byte(&text, *s);
      s += 2;
      continue;
    }
    if (*s != '{') {
      byte(&text, *s++);
      continue;
    }
    literal(&m->symbols, &text, map);
    const char *end = strchr(++s, '}');
    if (!end)
      fail("unclosed message markup");
    char *command = formatString("%.*s", (int)(end - s), s),
         *arg = strchr(command, ':');
    if (arg)
      *arg++ = 0;
    if (!strcmp(command, "glyph") || !strcmp(command, "byte") ||
        !strcmp(command, "control")) {
      if (!arg)
        fail("message markup needs a value");
      half(&m->symbols,
           number(arg, !strcmp(command, "control") ? 10 : 16, 65535));
    } else {
      unsigned opcode = 0;
      for (unsigned i = 1; i < 32; i++)
        if (commands[i] && !strcmp(command, commands[i]))
          opcode = i;
      if (!opcode || (arg && !takesArgument(opcode)))
        fail("invalid message command: %s", command);
      half(&m->symbols, opcode);
      if (arg)
        half(&m->symbols, number(arg, 10, 65535));
    }
    free(command);
    s = end + 1;
  }
  literal(&m->symbols, &text, map);
  free(text.data);
  for (size_t i = 0; i < m->symbols.len; i += 2)
    if (read16(m->symbols.data + i) >= alphabet)
      fail("message symbol is outside the edition alphabet");
}

static Transition *transition(Context *c, unsigned symbol) {
  for (size_t i = 0; i < c->count; i++)
    if (c->transitions[i].symbol == symbol)
      return c->transitions + i;
  c->transitions = resize(c->transitions, c->count + 1, sizeof(Transition));
  c->transitions[c->count] = (Transition){symbol, 0, {0}};
  return c->transitions + c->count++;
}

static void walk(Context *c, Node *nodes, int index, Buffer *path) {
  Node *n = nodes + index;
  if (n->transition >= 0) {
    Transition *t = c->transitions + n->transition;
    putBits(&c->tree, 1, 1);
    half(&c->leaves, t->symbol);
    append(&t->path, path->data, path->len);
  } else {
    putBits(&c->tree, 0, 1);
    byte(path, 0);
    walk(c, nodes, n->left, path);
    path->data[path->len - 1] = 1;
    walk(c, nodes, n->right, path);
    path->len--;
  }
}

static bool before(const Node *a, const Node *b) {
  return a->count < b->count || (a->count == b->count && a->order < b->order);
}
/* Each preceding symbol has its own Huffman tree. Equal frequencies keep
 * insertion order: changing that tie break changes the whole archive. */
static void derive(Context *c) {
  if (!c->count)
    return;
  Node *nodes = allocate(2 * c->count, sizeof(Node));
  int *queue = allocate(c->count, sizeof(int));
  size_t length = c->count, used = c->count;
  for (size_t i = 0; i < c->count; i++) {
    nodes[i] = (Node){c->transitions[i].count, i, -1, -1, (int)i};
    queue[i] = (int)i;
  }
  while (length > 1) {
    for (size_t i = 1; i < length; i++) {
      int value = queue[i];
      size_t j = i;
      while (j && before(nodes + value, nodes + queue[j - 1])) {
        queue[j] = queue[j - 1];
        j--;
      }
      queue[j] = value;
    }
    int a = queue[0], b = queue[1];
    nodes[used] = (Node){nodes[a].count + nodes[b].count, used, a, b, -1};
    memmove(queue, queue + 2, (length - 2) * sizeof(int));
    queue[length - 2] = (int)used++;
    length--;
  }
  Buffer path = {0};
  walk(c, nodes, queue[0], &path);
  free(path.data);
  free(nodes);
  free(queue);
}
/* Build symbol contexts first, then the message banks and their pointers. */
static Assembly archive(Catalog *catalog, const char *label) {
  size_t alphabet = catalog->alphabet;
  Context *contexts = allocate(alphabet, sizeof(Context));
  Message **messages = allocate(catalog->count, sizeof(Message *));
  for (size_t i = 0; i < catalog->count; i++)
    messages[number(catalog->messages[i].id, 10, catalog->count - 1)] =
        catalog->messages + i;
  for (size_t i = 0; i < catalog->count; i++)
    if (!messages[i]->null) {
      Buffer *symbols = &messages[i]->symbols;
      unsigned previous = 0;
      for (size_t at = 0; at <= symbols->len; at += 2) {
        unsigned symbol = at == symbols->len ? 0 : read16(symbols->data + at);
        transition(contexts + previous, symbol)->count++;
        previous = symbol;
      }
    }
  Assembly a = {0};
  a.alignment = 4;
  Buffer offsets = {0}, bases = {0}, directory = {0};
  size_t groupStart = 0;
  char *models = formatString("%sModels", label);
  labelAt(&a, models, 0, true);
  for (size_t i = 0; i < alphabet; i++) {
    Context *c = contexts + i;
    derive(c);
    if (!(i % 256)) {
      groupStart = a.bytes.len;
      word(&bases, (uint32_t)groupStart);
    }
    if (!c->count) {
      half(&offsets, 0x8000);
      continue;
    }
    Buffer leaves = {0};
    size_t n = c->leaves.len / 2;
    /* The final pair is stored first; an odd final leaf occupies two bytes. */
    for (size_t group = (n + 1) / 2; group; group--) {
      size_t at = (group - 1) * 2;
      unsigned first = read16(c->leaves.data + at * 2);
      if (at + 1 < n) {
        unsigned second = read16(c->leaves.data + (at + 1) * 2);
        byte(&leaves, second);
        byte(&leaves, (first & 15) << 4 | second >> 8);
        byte(&leaves, first >> 4);
      } else {
        byte(&leaves, (first & 15) << 4);
        byte(&leaves, first >> 4);
      }
    }
    size_t offset = a.bytes.len + leaves.len - groupStart;
    if (offset > 65535)
      fail("context model exceeds its offset table");
    half(&offsets, (unsigned)offset);
    append(&a.bytes, leaves.data, leaves.len);
    append(&a.bytes, c->tree.bytes.data, c->tree.bytes.len);
    free(leaves.data);
  }
  alignBuffer(&a.bytes, 2);
  size_t offsetTable = a.bytes.len;
  append(&a.bytes, offsets.data, offsets.len);
  alignBuffer(&a.bytes, 4);
  char *name = formatString("%sContexts", label);
  labelAt(&a, name, a.bytes.len, true);
  free(name);
  for (size_t i = 0; i < bases.len / 4; i++) {
    addPointer(&a, models, read32(bases.data + 4 * i));
    addPointer(&a, models, offsetTable + i * 512);
  }
  for (size_t start = 0; start < catalog->count; start += 256) {
    Buffer payload = {0}, lengths = {0};
    for (size_t i = start; i < start + 256 && i < catalog->count; i++) {
      Bits bits = {0};
      unsigned previous = 0;
      if (!messages[i]->null) {
        Buffer *symbols = &messages[i]->symbols;
        for (size_t at = 0; at <= symbols->len; at += 2) {
          unsigned symbol = at == symbols->len ? 0 : read16(symbols->data + at);
          Transition *t = transition(contexts + previous, symbol);
          for (size_t b = 0; b < t->path.len; b++)
            putBits(&bits, t->path.data[b], 1);
          previous = symbol;
        }
      }
      size_t n = bits.bytes.len;
      if (n > 65535)
        fail("encoded message exceeds its length field");
      while (n >= 255) {
        byte(&lengths, 255);
        n -= 255;
      }
      byte(&lengths, n);
      append(&payload, bits.bytes.data, bits.bytes.len);
      free(bits.bytes.data);
    }
    word(&directory, (uint32_t)a.bytes.len);
    append(&a.bytes, payload.data, payload.len);
    word(&directory, (uint32_t)a.bytes.len);
    append(&a.bytes, lengths.data, lengths.len);
    free(payload.data);
    free(lengths.data);
  }
  alignBuffer(&a.bytes, 4);
  name = formatString("%sBanks", label);
  labelAt(&a, name, a.bytes.len, true);
  free(name);
  for (size_t i = 0; i < directory.len; i += 4)
    addPointer(&a, models, read32(directory.data + i));
  for (size_t i = 0; i < alphabet; i++) {
    Context *c = contexts + i;
    for (size_t j = 0; j < c->count; j++)
      free(c->transitions[j].path.data);
    free(c->transitions);
    free(c->tree.bytes.data);
    free(c->leaves.data);
  }
  free(contexts);
  free(messages);
  free(models);
  free(offsets.data);
  free(bases.data);
  free(directory.data);
  return a;
}

int main(int argc, char **argv) {
  const char *usage =
      "usage: ags-text CATALOG.po OUTPUT.inc|OUTPUT.h [--label NAME] "
      "[--header] [--as-value] [--charmap FILE]";
  startTool(argc, argv, usage);
  if (argc < 3)
    fail("%s", usage);
  bool header = false, asValue = false;
  const char *label = "Text_Message", *mapPath = NULL;
  for (int i = 3; i < argc; i++) {
    if (!strcmp(argv[i], "--header"))
      header = true;
    else if (!strcmp(argv[i], "--as-value"))
      asValue = true;
    else if (!strcmp(argv[i], "--label") && i + 1 < argc)
      label = argv[++i];
    else if (!strcmp(argv[i], "--charmap") && i + 1 < argc)
      mapPath = argv[++i];
    else
      fail("unknown or incomplete option: %s", argv[i]);
  }
  Catalog c = readCatalog(argv[1]);
  Buffer map = {0};
  if (strstr(c.target, "-ja")) {
    if (!mapPath)
      mapPath = !strcmp(c.target, "tbs-ja")
                    ? "games/common/src/graphics/font/charmap_ja.tsv"
                    : "games/common/src/graphics/font/charmap_tla_ja.tsv";
    map = charmap(mapPath);
  }
  for (size_t i = 0; i < c.count; i++)
    symbols(c.messages + i, &map, c.alphabet);
  Buffer out = {0};
  if (header) {
    print(&out, "/* Message numbers from the edition's editable PO catalog. "
                "*/\n#ifndef ALCHEMY_MESSAGE_IDS_H\n#define "
                "ALCHEMY_MESSAGE_IDS_H\n#include \"types.h\"\n#define "
                "MSGNO(name) TEXT_MESSAGE_NUMBER_ ## name\n");
    for (size_t i = 0; i < c.count; i++)
      if (strcmp(c.messages[i].name, "message"))
        print(&out, "#define TEXT_MESSAGE_NUMBER_%s %llu\n", c.messages[i].name,
              (unsigned long long)number(c.messages[i].id, 10, 65535));
    print(&out, "#define TEXT_MESSAGE_NUMBER_MsgEnd %zu\n", c.count);
    if (asValue)
      print(
          &out,
          "/* A message's name is its linker symbol, whose value is the "
          "message's\n * number. C can name a link-time value only as an "
          "address, so each\n * name below stands for that address read as an "
          "s32. A macro is not\n * expanded again inside its own replacement "
          "(C89 3.8.3.4): the name in\n * the cast is the symbol, which the "
          "code still loads from a relocated\n * pool word. */\n");
    for (size_t i = 0; i <= c.count; i++) {
      const char *name = i == c.count ? "MsgEnd" : c.messages[i].name;
      if (!strcmp(name, "message"))
        continue;
      print(&out, "extern u8 %s[];\n", name);
      if (asValue)
        print(&out, "#define %s ((s32)%s)\n", name, name);
    }
    print(&out, "#endif\n");
  } else {
    print(&out, "@ %s's message archive and message numbers, built from %s.\n",
          c.target, argv[1]);
    Assembly a = archive(&c, label);
    Buffer source = assemblySource(&a);
    append(&out, source.data, source.len);
    free(source.data);
    freeAssembly(&a);
    for (size_t i = 0; i < c.count; i++)
      if (strcmp(c.messages[i].name, "message"))
        print(&out, "\t.global %s\n\t.set %s, %llu\n", c.messages[i].name,
              c.messages[i].name,
              (unsigned long long)number(c.messages[i].id, 10, 65535));
    print(&out, "\t.global MsgEnd\n\t.set MsgEnd, %zu\n", c.count);
  }
  writeFile(argv[2], &out);
  free(out.data);
  free(map.data);
  free(c.target);
  for (size_t i = 0; i < c.count; i++) {
    free(c.messages[i].name);
    free(c.messages[i].id);
    free(c.messages[i].text);
    free(c.messages[i].symbols.data);
  }
  free(c.messages);
  return 0;
}
