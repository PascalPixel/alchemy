/* Resource containers: tables, maps, fonts, and banks of pictures. */
#include "graphics.h"

static Buffer readSibling(const char *input, const char *name) {
  char *path = sibling(input, name);
  Buffer b = readFile(path);
  free(path);
  return b;
}

static Strings linesOf(const Buffer *input) {
  Strings all = split((char *)input->data, '\n'), lines = {0};
  for (size_t i = 0; i < all.len; i++)
    if (*trim(all.items[i]))
      addString(&lines, all.items[i]);
  freeStrings(&all);
  return lines;
}

Buffer table(const Buffer *input) {
  Strings lines = linesOf(input);
  if (!lines.len)
    fail("table names no columns");
  Strings columns = split(lines.items[0], '\t');
  Buffer out = {0};
  bool shortRow = false;
  for (size_t row = 1; row < lines.len; row++) {
    Strings fields = split(lines.items[row], '\t');
    if (shortRow || fields.len > columns.len)
      fail("table record does not fit its columns");
    shortRow = fields.len < columns.len;
    for (size_t i = 0; i < fields.len; i++) {
      char *type = strrchr(columns.items[i], ':');
      if (!type)
        fail("table column needs NAME:TYPE");
      type++;
      bool sign = *type == 's';
      if (!sign && *type != 'u')
        fail("unknown table column type");
      unsigned bits = number(type + 1, 10, 32);
      if (bits != 8 && bits != 16 && bits != 32)
        fail("unknown table column width");
      char *s = trim(fields.items[i]);
      bool neg = *s == '-';
      if (neg)
        s++;
      uint64_t magnitude = decimalOrHex(s, UINT32_MAX),
               limit = sign ? (UINT64_C(1) << (bits - 1)) - !neg
                            : (UINT64_C(1) << bits) - 1;
      if ((neg && !sign) || magnitude > limit)
        fail("table value does not fit its column");
      uint32_t value = neg ? (uint32_t)-magnitude : (uint32_t)magnitude;
      for (unsigned b = 0; b < bits; b += 8)
        byte(&out, value >> b);
    }
    freeStrings(&fields);
  }
  freeStrings(&columns);
  freeStrings(&lines);
  return out;
}

static const char *headerField(const Strings *fields, const char *key) {
  size_t n = strlen(key);
  for (size_t i = 0; i < fields->len; i++)
    if (!strncmp(fields->items[i], key, n) && fields->items[i][n] == ':')
      return fields->items[i] + n + 1;
  fail("plane header needs %s", key);
}

static void pair(const char *text, char separator, size_t *a, size_t *b) {
  Strings fields = split(text, separator);
  if (fields.len != 2)
    fail("expected two dimensions");
  *a = number(fields.items[0], 10, SIZE_MAX);
  *b = number(fields.items[1], 10, SIZE_MAX);
  freeStrings(&fields);
}

static Buffer plane(const Buffer *input) {
  /* A blank row still occupies a row in the map. Only drop the final newline.
   */
  Strings lines = split((char *)input->data, '\n');
  if (lines.len && !*lines.items[lines.len - 1])
    free(lines.items[--lines.len]);
  if (!lines.len)
    fail("plane has no header");
  Strings header = split(lines.items[0], '\t');
  const char *cell = headerField(&header, "cell");
  unsigned size = !strcmp(cell, "u8")    ? 1
                  : !strcmp(cell, "u16") ? 2
                  : !strcmp(cell, "u32") ? 4
                                         : 0;
  if (!size)
    fail("unknown plane cell type");
  size_t width, height, left, top;
  pair(headerField(&header, "grid"), 'x', &width, &height);
  pair(headerField(&header, "at"), ',', &left, &top);
  uint64_t max = (UINT64_C(1) << (size * 8)) - 1;
  uint32_t fill = number(headerField(&header, "fill"), 16, max);
  if (!width || !height || height > SIZE_MAX / width)
    fail("invalid plane dimensions");
  uint32_t *cells = allocate(width * height, sizeof(uint32_t));
  for (size_t i = 0; i < width * height; i++)
    cells[i] = fill;
  for (size_t y = 1; y < lines.len; y++) {
    Strings row = words(lines.items[y]);
    if (top >= height || y - 1 >= height - top || left > width ||
        row.len > width - left)
      fail("plane row leaves the grid");
    for (size_t x = 0; x < row.len; x++)
      cells[(top + y - 1) * width + left + x] = number(row.items[x], 16, max);
    freeStrings(&row);
  }
  Buffer out = {0};
  for (size_t i = 0; i < width * height; i++)
    for (unsigned b = 0; b < size; b++)
      byte(&out, cells[i] >> (8 * b));
  free(cells);
  freeStrings(&header);
  freeStrings(&lines);
  return out;
}

static Buffer script(const Buffer *input) {
  Strings lines = linesOf(input);
  Buffer out = {0};
  for (size_t i = 0; i < lines.len; i++) {
    Strings f = split(lines.items[i], '\t');
    const char *key = f.items[0];
    size_t n;
    unsigned prefix = 0;
    if (!strcmp(key, "channel")) {
      n = 1;
      prefix = 0xfd00;
    } else if (!strcmp(key, "jump")) {
      n = 1;
      prefix = 0xfe00;
    } else if (!strcmp(key, "stop")) {
      n = 0;
      prefix = 0xfeff;
    } else if (!strcmp(key, "end")) {
      n = 0;
      prefix = 0xffff;
    } else if (!strcmp(key, "frame"))
      n = 4;
    else if (!strcmp(key, "control"))
      n = 1;
    else if (!strcmp(key, "level"))
      n = 2;
    else
      fail("unknown script command: %s", key);
    if (f.len != n + 1)
      fail("wrong number of script arguments");
    if (prefix)
      half(&out, prefix | (n ? decimalOrHex(trim(f.items[1]), 255) : 0));
    else
      for (size_t j = 1; j < f.len; j++)
        half(&out, decimalOrHex(trim(f.items[j]), 65535));
    freeStrings(&f);
  }
  freeStrings(&lines);
  return out;
}

static Buffer blocks(const Buffer *input) {
  Strings lines = linesOf(input);
  if (!lines.len || lines.len % 16)
    fail("block grid needs whole 16x16 blocks");
  Strings *rows = allocate(lines.len, sizeof(Strings));
  size_t width = 0;
  for (size_t y = 0; y < lines.len; y++) {
    rows[y] = split(lines.items[y], '\t');
    if (!y)
      width = rows[y].len;
    if (!width || width % 16 || rows[y].len != width)
      fail("block grid rows differ");
  }
  size_t count = width / 16 * (lines.len / 16);
  Buffer out = {0};
  for (size_t i = 0; i < count; i++)
    word(&out, 0);
  for (size_t i = 0; i < count; i++) {
    size_t x = i % (width / 16) * 16, y = i / (width / 16) * 16;
    Buffer values = {0};
    unsigned missing = 0;
    for (size_t dy = 0; dy < 16; dy++)
      for (size_t dx = 0; dx < 16; dx++) {
        char *s = trim(rows[y + dy].items[x + dx]);
        if (!strcmp(s, "-"))
          missing++;
        else
          word(&values, number(s, 16, UINT32_MAX));
      }
    if (missing && missing != 256)
      fail("block is omitted only in part");
    if (!missing) {
      write32(out.data + 4 * i, (uint32_t)out.len);
      Buffer packed = compressLz(&values, &values, "plz", packer);
      append(&out, packed.data, packed.len);
      free(packed.data);
    }
    free(values.data);
  }
  for (size_t i = 0; i < lines.len; i++)
    freeStrings(rows + i);
  free(rows);
  freeStrings(&lines);
  return out;
}

static Buffer font(const char *input, const Image *image) {
  char *name = copyString(input), *dot = strrchr(name, '.');
  if (!dot)
    fail("font input has no suffix");
  strcpy(dot, ".tsv");
  Buffer data = readFile(name);
  free(name);
  Strings lines = linesOf(&data);
  size_t first = SIZE_MAX, top = SIZE_MAX, rows = SIZE_MAX, index = 0;
  Buffer out = {0};
  if (image->width < 16 || image->width % 16 || image->height % 16)
    fail("font needs whole 16x16 glyph cells");
  for (size_t i = 0; i < lines.len; i++) {
    Strings f = split(lines.items[i], '\t');
    if ((!strcmp(f.items[0], "first") || !strcmp(f.items[0], "top") ||
         !strcmp(f.items[0], "rows")) &&
        f.len != 2)
      fail("font metadata row needs one value");
    if (!strcmp(f.items[0], "first"))
      first = number(f.items[1], 16, SIZE_MAX);
    else if (!strcmp(f.items[0], "top"))
      top = number(f.items[1], 16, 16);
    else if (!strcmp(f.items[0], "rows"))
      rows = number(f.items[1], 16, 16);
    else {
      if (first == SIZE_MAX || top > 16 || rows > 16 || top + rows > 16 ||
          number(f.items[0], 16, SIZE_MAX) != first + index)
        fail("invalid font glyph order or dimensions");
      size_t columns = image->width / 16, x = index % columns * 16,
             y = index / columns * 16;
      if (y + 16 > image->height)
        fail("glyph lies outside font sheet");
      for (size_t j = 1; j < f.len; j++)
        half(&out, number(trim(f.items[j]), 16, 65535));
      for (size_t dy = 0; dy < rows; dy++) {
        unsigned value = 0;
        for (size_t dx = 0; dx < 16; dx++)
          value |=
              (image->pixels.data[(y + top + dy) * image->width + x + dx] != 0)
              << (15 - dx);
        half(&out, value);
      }
      index++;
    }
    freeStrings(&f);
  }
  freeStrings(&lines);
  free(data.data);
  return out;
}

static Buffer imageForm(const char *input, const char *form,
                        const Buffer *data) {
  Image image = readImage(data);
  Buffer out = {0};
  unsigned depth;
  size_t mw, mh;
  if (tileForm(form, &depth, &mw, &mh))
    out = tiles(&image, depth, mw, mh);
  else if (!strcmp(form, "gbapal"))
    out = gbaPalette(&image);
  else if (!strcmp(form, "bitmap"))
    append(&out, image.pixels.data, image.pixels.len);
  else if (!strcmp(form, "icon4"))
    out = compressIcon(&image.pixels);
  else if (!strcmp(form, "font"))
    out = font(input, &image);
  else if (!strcmp(form, "glyphs")) {
    if (image.width != 8)
      fail("glyphs must be eight pixels wide");
    for (size_t y = 0; y < image.height; y++) {
      unsigned v = 0;
      for (size_t x = 0; x < 8; x++)
        v |= (image.pixels.data[y * 8 + x] != 0) << x;
      byte(&out, v);
    }
  } else if (!strcmp(form, "1bpp")) {
    if (image.width % 8 || image.height % 8)
      fail("1bpp requires whole 8x8 tiles");
    for (size_t y = 0; y < image.height; y += 8)
      for (size_t x = 0; x < image.width; x += 8)
        for (size_t dy = 0; dy < 8; dy++) {
          unsigned v = 0;
          for (size_t dx = 0; dx < 8; dx++)
            v = v << 1 |
                (image.pixels.data[(y + dy) * image.width + x + dx] != 0);
          byte(&out, v);
        }
  } else if (!strcmp(form, "frames")) {
    if (image.height % image.width)
      fail("frames must be square and stacked");
    size_t count = image.height / image.width, size = image.width * image.width;
    for (size_t i = 0; i <= count; i++)
      word(&out, 0);
    for (unsigned parity = 0; parity < 2; parity++)
      for (size_t i = parity ? 0 : 1; i < count; i += 2) {
        write32(out.data + 4 * i, (uint32_t)out.len);
        Buffer frame = {image.pixels.data + i * size, size, size};
        bool blank = true;
        for (size_t p = 0; p < size; p++)
          blank &= frame.data[p] == 255;
        if (!blank) {
          Buffer packed = compressZeros(&frame);
          append(&out, packed.data, packed.len);
          free(packed.data);
        }
      }
  } else if (!strcmp(form, "icons")) {
    if (image.width != 32 || image.height % 32)
      fail("icons must be 32x32 and stacked");
    size_t count = image.height / 32;
    for (size_t i = 0; i < count; i++)
      half(&out, 0);
    for (size_t i = 0; i < count; i++) {
      if (out.len > 65535)
        fail("icon bank is too large");
      write16(out.data + 2 * i, (unsigned)out.len);
      Image frame = image;
      frame.height = 32;
      frame.pixels.data += i * 1024;
      frame.pixels.len = 1024;
      Buffer raw = tiles(&frame, 8, 8, 8),
             packed = compressLz(&raw, &raw, "plz", packer);
      alignBuffer(&packed, 32);
      append(&out, packed.data, packed.len);
      free(raw.data);
      free(packed.data);
    }
  } else if (!strcmp(form, "strips")) {
    if (image.width % 8 || image.height % 8)
      fail("strip picture requires whole 8x8 tiles");
    size_t bands = image.height / 8;
    for (size_t i = 0; i <= bands; i++)
      word(&out, 0);
    write32(out.data, (uint32_t)out.len);
    Buffer palette = gbaPalette(&image);
    append(&out, palette.data, palette.len);
    free(palette.data);
    for (size_t i = 0; i < bands; i++) {
      write32(out.data + 4 * (i + 1), (uint32_t)out.len);
      Image strip = image;
      strip.height = 8;
      strip.pixels.data += i * 8 * image.width;
      strip.pixels.len = 8 * image.width;
      Buffer raw = tiles(&strip, 8, 8, 8),
             packed = compressLz(&raw, &raw, "lz", packer);
      append(&out, packed.data, packed.len);
      free(raw.data);
      free(packed.data);
    }
  } else
    fail("unknown image form: %s", form);
  freeImage(&image);
  return out;
}

static Buffer mappedPart(const char *input, const char *path, const char *form,
                         const Strings *fields) {
  if (fields->len != 5 && fields->len != 6)
    fail("mapped part needs MAP FIRST COUNT [frames]");
  if (strcmp(form, "4bpp") && strcmp(form, "8bpp"))
    fail("mapped part must be 4bpp or 8bpp");
  bool frames = fields->len == 6;
  if (frames && strcmp(fields->items[5], "frames"))
    fail("invalid mapped part option");
  size_t first = decimalOrHex(fields->items[3], 1023),
         count = decimalOrHex(fields->items[4], 1024);
  if (!count)
    fail("empty tile range");
  char *name = formatString("%s.tsv", fields->items[2]);
  Buffer rawMap = readSibling(input, name);
  free(name);
  Strings lines = linesOf(&rawMap);
  if (!lines.len)
    fail("empty tilemap");
  Strings columns = split(lines.items[0], '\t');
  size_t wide = columns.len;
  for (size_t i = 0; i < wide; i++) {
    char *type = strrchr(columns.items[i], ':');
    if (!type || strcmp(type, ":u16"))
      fail("tilemap requires u16 columns");
  }
  Buffer map = table(&rawMap);
  if (map.len % (2 * wide))
    fail("tilemap has incomplete rows");
  size_t left = wide, right = 0, top = SIZE_MAX, bottom = 0;
  bool *shown = allocate(count, sizeof(bool));
  for (size_t i = 0; i < map.len / 2; i++) {
    size_t tile = read16(map.data + 2 * i) & 1023;
    if (tile < first || tile - first >= count)
      continue;
    shown[tile - first] = true;
    if (i % wide < left)
      left = i % wide;
    if (i % wide > right)
      right = i % wide;
    if (i / wide < top)
      top = i / wide;
    if (i / wide > bottom)
      bottom = i / wide;
  }
  if (top == SIZE_MAX)
    fail("tilemap shows no tiles from the run");
  size_t w = right + 1 - left, h = bottom + 1 - top, hidden = 0;
  for (size_t i = 0; i < count; i++)
    hidden += !shown[i];
  size_t rows = h + (frames ? (hidden + w - 1) / w : 0);
  Buffer png = readFile(path);
  Image image = readImage(&png);
  free(png.data);
  if (image.width != w * 8 || image.height % (rows * 8) ||
      (!frames && image.height != rows * 8))
    fail("tilemap drawing dimensions differ");
  Buffer out = {0};
  for (size_t frame = 0; frame < image.height / (rows * 8); frame++) {
    Image run = {count * 8, 8, {0}, {0}, {0}, 8};
    reserve(&run.pixels, count * 64);
    run.pixels.len = count * 64;
    memset(run.pixels.data, 0, run.pixels.len);
    bool *seen = allocate(count, sizeof(bool));
    size_t hiddenSlot = 0;
    for (size_t cell = 0; cell < w * rows; cell++) {
      size_t tile = SIZE_MAX;
      bool hf = false, vf = false;
      if (cell < w * h) {
        unsigned entry =
            read16(map.data + 2 * ((top + cell / w) * wide + left + cell % w));
        if ((entry & 1023) >= first && (entry & 1023) - first < count)
          tile = (entry & 1023) - first;
        hf = entry & 1024;
        vf = entry & 2048;
      } else {
        while (hiddenSlot < count && shown[hiddenSlot])
          hiddenSlot++;
        if (hiddenSlot < count)
          tile = hiddenSlot++;
      }
      for (size_t y = 0; y < 8; y++)
        for (size_t x = 0; x < 8; x++) {
          uint8_t p =
              image.pixels
                  .data[(frame * rows * 8 + cell / w * 8 + (vf ? 7 - y : y)) *
                            image.width +
                        cell % w * 8 + (hf ? 7 - x : x)];
          if (tile == SIZE_MAX) {
            if (p)
              fail("unmapped cell is inked");
          } else {
            size_t at = y * run.width + tile * 8 + x;
            if (seen[tile] && run.pixels.data[at] != p)
              fail("tile is drawn two ways");
            run.pixels.data[at] = p;
          }
        }
      if (tile != SIZE_MAX)
        seen[tile] = true;
    }
    Buffer packed = tiles(&run, form[0] - '0', 8, 8);
    append(&out, packed.data, packed.len);
    free(packed.data);
    free(run.pixels.data);
    free(seen);
  }
  free(shown);
  freeImage(&image);
  free(map.data);
  free(rawMap.data);
  freeStrings(&columns);
  freeStrings(&lines);
  return out;
}

static bool dataForm(const char *form) {
  return !strncmp(form, "delta", 5) || !strcmp(form, "table") ||
         !strcmp(form, "plane") || !strcmp(form, "parts") ||
         !strcmp(form, "icons4") || !strcmp(form, "blocks") ||
         !strcmp(form, "script") || !strcmp(form, "sprites") ||
         !strcmp(form, "spriteblocks");
}

static Buffer parts(const char *input, const Buffer *data, unsigned depth) {
  Strings lines = linesOf(data);
  Buffer out = {0};
  for (size_t i = 0; i < lines.len; i++) {
    Strings f = split(lines.items[i], '\t');
    for (size_t j = 0; j < f.len; j++) {
      char *v = copyString(trim(f.items[j]));
      free(f.items[j]);
      f.items[j] = v;
    }
    if (f.len < 2)
      fail("part needs a name and form");
    const char *form = f.items[1];
    if ((dataForm(form) && strcmp(form, "table") && strcmp(form, "plane")) ||
        !strcmp(form, "font"))
      fail("part must be an image or table");
    char *name =
             formatString("%s.%s", f.items[0], dataForm(form) ? "tsv" : "png"),
         *path = sibling(input, name);
    free(name);
    Buffer part = f.len == 2 ? buildResource(path, form, NULL, depth + 1)
                             : mappedPart(input, path, form, &f);
    append(&out, part.data, part.len);
    free(part.data);
    free(path);
    freeStrings(&f);
  }
  freeStrings(&lines);
  return out;
}

static Buffer iconBank(const char *input, const Buffer *data) {
  Strings lines = linesOf(data), slots = {0};
  size_t align = 4;
  for (size_t i = 0; i < lines.len; i++) {
    if (!strncmp(lines.items[i], "align\t", 6)) {
      align = number(trim(lines.items[i] + 6), 16, 65536);
      if (!align || align & (align - 1))
        fail("icon alignment is not a power of two");
    } else
      addString(&slots, lines.items[i]);
  }
  Buffer out = {0};
  for (size_t i = 0; i < slots.len; i++)
    half(&out, 0);
  for (size_t i = 0; i < slots.len; i++)
    if (strcmp(slots.items[i], "-")) {
      if (out.len > 65535)
        fail("icon bank is too large");
      write16(out.data + 2 * i, (unsigned)out.len);
      char *name = formatString("%s.png", slots.items[i]);
      Buffer png = readSibling(input, name);
      free(name);
      Image image = readImage(&png);
      free(png.data);
      if (image.width != 32 || image.height != 32 || image.palette.len != 48)
        fail("icon must be 32x32 in sixteen colours");
      Buffer palette = gbaPalette(&image), packed = compressIcon(&image.pixels);
      append(&out, palette.data, palette.len);
      append(&out, packed.data, packed.len);
      alignBuffer(&out, align);
      free(palette.data);
      free(packed.data);
      freeImage(&image);
    }
  freeStrings(&lines);
  freeStrings(&slots);
  return out;
}
/* Preserve documented edits made after packing. The edited asset supplies
 * the shipped bytes; its patch supplies only the packer's neutral input. */
static Buffer patched(const char *input, const Buffer *data) {
  const char *base = strrchr(input, '/');
  base = base ? base + 1 : input;
  const char *dot = strchr(base, '.');
  char *name = formatString("%.*s_patch.tsv",
                            (int)(dot ? dot - base : (ptrdiff_t)strlen(base)),
                            base),
       *path = sibling(input, name);
  free(name);
  Buffer result = {0};
  append(&result, data->data, data->len);
  if (exists(path)) {
    Buffer patch = readFile(path);
    Strings lines = linesOf(&patch);
    if (lines.len < 2 || strcmp(lines.items[0], "position\tpack\tship\tproof"))
      fail("invalid packing patch header");
    size_t previous = 0;
    for (size_t i = 1; i < lines.len; i++) {
      Strings f = split(lines.items[i], '\t');
      if (f.len != 4 || !*trim(f.items[3]))
        fail("packing patch needs four fields and proof");
      size_t at = number(trim(f.items[0]), 10, SIZE_MAX);
      unsigned pack = decimalOrHex(trim(f.items[1]), 255),
               ship = decimalOrHex(trim(f.items[2]), 255);
      if (at >= data->len || (i > 1 && at <= previous) ||
          ship != data->data[at] || pack == ship)
        fail("packing patch disagrees with asset");
      result.data[at] = pack;
      previous = at;
      freeStrings(&f);
    }
    freeStrings(&lines);
    free(patch.data);
  }
  free(path);
  return result;
}

Buffer buildResource(const char *input, const char *form, const char *codec,
                     unsigned depth) {
  if (depth > 32)
    fail("asset nesting is too deep");
  Buffer data = readFile(input), raw = {0};
  if (!strcmp(form, "table"))
    raw = table(&data);
  else if (!strcmp(form, "plane"))
    raw = plane(&data);
  else if (!strcmp(form, "script"))
    raw = script(&data);
  else if (!strcmp(form, "blocks"))
    raw = blocks(&data);
  else if (!strcmp(form, "parts"))
    raw = parts(input, &data, depth);
  else if (!strcmp(form, "icons4"))
    raw = iconBank(input, &data);
  else if (!strcmp(form, "sprites") || !strcmp(form, "spriteblocks"))
    raw = spriteSource(input, !strcmp(form, "spriteblocks"));
  else if (!strncmp(form, "delta", 5)) {
    Buffer t = table(&data);
    raw = deltaMap(&t, number(form + 5, 10, 2));
    free(t.data);
  } else
    raw = imageForm(input, form, &data);
  free(data.data);
  if (!codec)
    return raw;
  Buffer out;
  if (!strcmp(codec, "d7"))
    out = compressDelta7(&raw);
  else if (!strcmp(codec, "hlz"))
    out = compressHalfwords(&raw);
  else {
    Buffer pack = patched(input, &raw);
    out = compressLz(&raw, &pack, codec, packer);
    free(pack.data);
  }
  free(raw.data);
  return out;
}
