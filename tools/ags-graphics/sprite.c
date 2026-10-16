/* Build sprite records, shared animations, and frame banks from their TSV
 * lists. */
#include "graphics.h"
#include "../common/assembly.h"

typedef struct {
  char *name, *files, *frames, *animations;
  size_t animationLimit;
  uint8_t bytes[12];
  Buffer *scripts;
  size_t scriptCount;
  Strings animationList, frameList, empty;
  bool hasAnimations, hasFrames;
} Sprite;
typedef struct {
  Sprite *sprites;
  size_t count;
  const char *input;
  Strings pictures;
} Bank;

static Buffer siblingFile(const char *input, const char *name) {
  char *path = sibling(input, name);
  Buffer b = readFile(path);
  free(path);
  return b;
}

static Sprite *find(Bank *b, const char *name) {
  for (size_t i = 0; i < b->count; i++)
    if (!strcmp(b->sprites[i].name, name))
      return b->sprites + i;
  fail("sprite %s has no record", name);
}

static bool owns(const Sprite *s, const char *list) {
  return list && !strcmp(s->name, list);
}

static unsigned integer(const char *s, bool sign, unsigned max) {
  bool negative = *s == '-';
  if (negative)
    s++;
  uint64_t v = decimalOrHex(s, sign ? (negative ? 128 : 127) : max);
  if (negative && !sign)
    fail("negative sprite field");
  return negative ? (unsigned)-v : (unsigned)v;
}

static void metadata(Sprite *s, const Buffer *data) {
  Strings lines = split((char *)data->data, '\n');
  for (size_t i = 0; i < lines.len; i++) {
    char *line = trim(lines.items[i]);
    if (!*line)
      continue;
    char *tab = strchr(line, '\t');
    if (!tab)
      fail("sprite metadata needs key and values");
    *tab++ = 0;
    Strings values = split(tab, ' ');
    if (!strcmp(line, "anim")) {
      Buffer script = {0};
      for (size_t j = 0; j < values.len; j++)
        byte(&script, number(values.items[j], 16, 255));
      s->scripts = resize(s->scripts, s->scriptCount + 1, sizeof(Buffer));
      s->scripts[s->scriptCount++] = script;
    } else if (!strcmp(line, "animations")) {
      freeStrings(&s->animationList);
      s->animationList = values;
      s->hasAnimations = true;
      values = (Strings){0};
    } else if (!strcmp(line, "list")) {
      freeStrings(&s->frameList);
      s->frameList = values;
      s->hasFrames = true;
      values = (Strings){0};
    } else if (!strcmp(line, "empty")) {
      freeStrings(&s->empty);
      s->empty = values;
      values = (Strings){0};
    } else
      fail("unknown sprite metadata key: %s", line);
    freeStrings(&values);
  }
  freeStrings(&lines);
}

static Bank readBank(const char *input) {
  Bank b = {0};
  b.input = input;
  Buffer data = readFile(input);
  Strings lines = split((char *)data.data, '\n');
  Strings header = split(lines.items[0], '\t');
  if (header.len != 12)
    fail("sprite table requires twelve columns");
  freeStrings(&header);
  for (size_t i = 1; i < lines.len; i++) {
    if (!*trim(lines.items[i]))
      continue;
    Strings f = split(lines.items[i], '\t');
    if (f.len != 12)
      fail("sprite row requires twelve fields");
    Sprite s = {0};
    s.animationLimit = SIZE_MAX;
    char *alias = strchr(f.items[0], '=');
    if (alias)
      *alias++ = 0;
    s.name = copyString(f.items[0]);
    s.files = copyString(alias ? alias : f.items[0]);
    s.bytes[0] = integer(f.items[1], false, 255);
    s.bytes[1] = integer(f.items[2], false, 255);
    write16(s.bytes + 2, integer(f.items[3], false, 65535));
    s.bytes[4] = integer(f.items[4], false, 255);
    s.bytes[6] = integer(f.items[5], true, 0);
    s.bytes[7] = integer(f.items[6], true, 0);
    for (unsigned j = 7; j <= 9; j++)
      s.bytes[j + 1] = integer(f.items[j], false, 255);
    if (strcmp(f.items[10], "-"))
      s.frames = copyString(f.items[10]);
    char *colon = strchr(f.items[11], ':');
    if (colon) {
      *colon++ = 0;
      s.animationLimit = number(colon, 10, SIZE_MAX);
    }
    if (strcmp(f.items[11], "-"))
      s.animations = copyString(f.items[11]);
    char *name = formatString("%s.tsv", s.files), *path = sibling(input, name);
    free(name);
    if (owns(&s, s.animations) || (owns(&s, s.frames) && exists(path))) {
      Buffer meta = readFile(path);
      metadata(&s, &meta);
      free(meta.data);
    }
    free(path);
    b.sprites = resize(b.sprites, b.count + 1, sizeof(Sprite));
    b.sprites[b.count++] = s;
    freeStrings(&f);
  }
  char *path = sibling(input, "pictures.tsv");
  if (exists(path)) {
    Buffer pictures = readFile(path);
    b.pictures = split((char *)pictures.data, '\n');
    free(pictures.data);
    if (!b.pictures.len || strcmp(b.pictures.items[0], "sprite\tpicture"))
      fail("invalid pictures table header");
  }
  free(path);
  freeStrings(&lines);
  free(data.data);
  return b;
}

static char *spriteLabel(const char *name, const char *item) {
  return formatString("Sprite_%s_%s", name, item);
}

static void label(Assembly *a, const char *name, const char *item) {
  char *s = spriteLabel(name, item);
  labelAt(a, s, a->bytes.len, false);
  free(s);
}

static void pointer(Assembly *a, const char *name, const char *item) {
  char *s = spriteLabel(name, item);
  addPointer(a, s, 0);
  free(s);
}

static void records(Bank *bank, Assembly *a) {
  for (size_t i = 0; i < bank->count; i++) {
    Sprite *s = bank->sprites + i;
    append(&a->bytes, s->bytes, 5);
    size_t count = 0;
    if (s->animations) {
      Sprite *owner = find(bank, s->animations);
      count =
          owner->hasAnimations ? owner->animationList.len : owner->scriptCount;
      if (s->animationLimit != SIZE_MAX) {
        if (s->animationLimit > count)
          fail("sprite animation count exceeds owner");
        count = s->animationLimit;
      }
    }
    if (count > 255)
      fail("too many sprite animations");
    byte(&a->bytes, count);
    append(&a->bytes, s->bytes + 6, 6);
    if (s->frames)
      pointer(a, s->frames, "Frames");
    else
      word(&a->bytes, 0);
    if (s->animations)
      pointer(a, s->animations, "Animations");
    else
      word(&a->bytes, 0);
  }
}

static void animationList(Bank *bank, Sprite *s, Assembly *a) {
  label(a, s->name, "Animations");
  size_t count = s->hasAnimations ? s->animationList.len : s->scriptCount;
  for (size_t i = 0; i < count; i++) {
    char *value = s->hasAnimations ? copyString(s->animationList.items[i])
                                   : formatString("%zu", i);
    char *colon = strchr(value, ':');
    const char *owner = s->name, *index = value;
    if (colon) {
      *colon = 0;
      owner = value;
      index = colon + 1;
    }
    size_t n = number(index, 10, SIZE_MAX);
    if (n >= find(bank, owner)->scriptCount)
      fail("sprite has no animation %zu", n);
    char *item = formatString("Animation%zu", n);
    pointer(a, owner, item);
    free(item);
    free(value);
  }
}

static void scripts(Sprite *s, Assembly *a) {
  for (size_t i = 0; i < s->scriptCount; i++) {
    char *item = formatString("Animation%zu", i);
    label(a, s->name, item);
    free(item);
    append(&a->bytes, s->scripts[i].data, s->scripts[i].len);
  }
}

static void frames(Bank *bank, Sprite *s, Assembly *list, Assembly *data) {
  char *name = formatString("%s.png", s->files);
  for (size_t i = 1; i < bank->pictures.len; i++) {
    Strings f = split(bank->pictures.items[i], '\t');
    if (f.len == 2 && !strcmp(f.items[0], s->files)) {
      free(name);
      name = formatString("common/%s.png", f.items[1]);
    }
    freeStrings(&f);
  }
  Buffer png = siblingFile(bank->input, name);
  free(name);
  Image image = readImage(&png);
  free(png.data);
  size_t width = s->bytes[0], height = s->bytes[1];
  if (!width || !height || image.width != width || image.height % height)
    fail("sprite frames have wrong dimensions");
  size_t count = image.height / height, size = width * height;
  label(list, s->name, "Frames");
  for (size_t i = 0; i < (s->hasFrames ? s->frameList.len : count); i++) {
    const char *value = s->hasFrames ? s->frameList.items[i] : NULL;
    if (value && !strcmp(value, "-")) {
      word(&list->bytes, 0);
      continue;
    }
    size_t n = value ? number(value, 10, SIZE_MAX) : i;
    if (n >= count)
      fail("frame list exceeds sprite sheet");
    char *item = formatString("Frame%zu", n);
    pointer(list, s->name, item);
    free(item);
  }
  Buffer arena = {0};
  for (size_t i = 0; i < count; i++) {
    char *item = formatString("Frame%zu", i);
    label(data, s->name, item);
    free(item);
    Buffer frame = {image.pixels.data + i * size, size, size}, stream = {0};
    bool empty = false;
    for (size_t j = 0; j < s->empty.len; j++)
      if (number(s->empty.items[j], 10, SIZE_MAX) == i)
        empty = true;
    if (empty) {
      if (s->bytes[10] != 3)
        fail("only an arena frame can be empty");
      for (size_t p = 0; p < size; p++)
        if (frame.data[p])
          fail("empty arena frame is inked");
    }
    switch (s->bytes[10]) {
    case 0:
      stream = compressZeros(&frame);
      break;
    case 1:
      stream = compressLz(&frame, &frame, "tagged", packer);
      break;
    case 3: {
      Buffer decoded = empty ? (Buffer){0} : compressZeros(&frame);
      stream = compressArena(&decoded, &arena);
      free(decoded.data);
      break;
    }
    default:
      fail("sprite codec has no frames");
    }
    append(&arena, stream.data, stream.len);
    append(&data->bytes, stream.data, stream.len);
    free(stream.data);
  }
  free(arena.data);
  freeImage(&image);
}

Buffer spriteSource(const char *input, bool blocks) {
  Bank bank = readBank(input);
  Assembly data = {0};
  records(&bank, &data);
  Buffer out = {0};
  if (!blocks) {
    for (size_t i = 0; i < bank.count; i++)
      if (owns(bank.sprites + i, bank.sprites[i].animations))
        animationList(&bank, bank.sprites + i, &data);
    Assembly images = {0};
    for (size_t i = 0; i < bank.count; i++)
      if (owns(bank.sprites + i, bank.sprites[i].frames)) {
        Assembly list = {0}, image = {0};
        frames(&bank, bank.sprites + i, &list, &image);
        appendAssembly(&data, &list);
        appendAssembly(&images, &image);
        freeAssembly(&list);
        freeAssembly(&image);
      }
    for (size_t i = 0; i < bank.count; i++)
      if (owns(bank.sprites + i, bank.sprites[i].animations))
        scripts(bank.sprites + i, &data);
    appendAssembly(&data, &images);
    freeAssembly(&images);
    out = assemblySource(&data);
  } else {
    Buffer layout = siblingFile(input, "layout.tsv");
    Strings lines = split((char *)layout.data, '\n'), order = {0};
    for (size_t i = 0; i < lines.len; i++) {
      char *line = trim(lines.items[i]);
      if (!*line)
        continue;
      if (!strncmp(line, "section\t", 8)) {
        Buffer source = assemblySource(&data);
        append(&out, source.data, source.len);
        free(source.data);
        freeAssembly(&data);
        print(&out, "\t.section .rodata.%s,\"a\"\n", line + 8);
      } else {
        if (strchr(line, '\t'))
          fail("invalid sprite layout row");
        Sprite *s = find(&bank, line);
        if (owns(s, s->animations)) {
          scripts(s, &data);
          alignAssembly(&data, 4, 0);
          animationList(&bank, s, &data);
        }
        addString(&order, line);
      }
    }
    for (size_t i = 0; i < order.len; i++) {
      Sprite *s = find(&bank, order.items[i]);
      if (!owns(s, s->frames))
        continue;
      Assembly list = {0}, image = {0};
      frames(&bank, s, &list, &image);
      appendAssembly(&data, &image);
      alignAssembly(&data, 4, 0);
      appendAssembly(&data, &list);
      word(&data.bytes, 0);
      freeAssembly(&list);
      freeAssembly(&image);
    }
    Buffer source = assemblySource(&data);
    append(&out, source.data, source.len);
    free(source.data);
    free(layout.data);
    freeStrings(&lines);
    freeStrings(&order);
  }
  freeAssembly(&data);
  for (size_t i = 0; i < bank.count; i++) {
    Sprite *s = bank.sprites + i;
    free(s->name);
    free(s->files);
    free(s->frames);
    free(s->animations);
    for (size_t j = 0; j < s->scriptCount; j++)
      free(s->scripts[j].data);
    free(s->scripts);
    freeStrings(&s->animationList);
    freeStrings(&s->frameList);
    freeStrings(&s->empty);
  }
  free(bank.sprites);
  freeStrings(&bank.pictures);
  return out;
}
