/* The game's compression formats. Match selection preserves the packer's tie
 * breaks. */
#include "compress.h"

const LzMachine packer = {4123, 485, 4126, 272};
typedef struct {
  size_t length, distance;
} Token;
typedef struct {
  const Buffer *data;
  size_t *previous;
} Matcher;

/* A chain per byte pair, newest first, preserves the nearest-match tie break.
 */
static Matcher matcher(const Buffer *data) {
  Matcher m = {data, allocate(data->len, sizeof(size_t))};
  size_t *heads = allocate(65536, sizeof(size_t));
  for (size_t i = 0; i < 65536; i++)
    heads[i] = SIZE_MAX;
  for (size_t i = 0; i + 1 < data->len; i++) {
    unsigned pair = read16(data->data + i);
    m.previous[i] = heads[pair];
    heads[pair] = i;
  }
  free(heads);
  return m;
}

static Token search(const Matcher *m, size_t at, size_t longest,
                    size_t lowest) {
  Token best = {1, 0};
  size_t max = m->data->len - at;
  if (max > longest)
    max = longest;
  if (max < 2)
    return best;
  const uint8_t *data = m->data->data;
  for (size_t from = m->previous[at]; from != SIZE_MAX && from >= lowest;
       from = m->previous[from]) {
    if (data[from + best.length] != data[at + best.length])
      continue;
    size_t n = 2;
    while (n < max && data[from + n] == data[at + n])
      n++;
    if (n > best.length)
      best = (Token){n, at - from};
    if (best.length == max)
      break;
  }
  return best;
}

static size_t oldest(size_t at, size_t size, size_t window, size_t ahead) {
  size_t read = ahead > size - at ? size : at + ahead;
  return read > window + ahead ? read - window - ahead : 0;
}

static size_t lowest(size_t old, size_t at, size_t reach) {
  return at > reach && at - reach > old ? at - reach : old;
}

static void lengthBits(Bits *b, size_t n) {
  putBits(b, 0, 1);
  if (n <= 5) {
    putBits(b, (1u << (n - 2)) - 1, (unsigned)n - 1);
  } else if (n <= 7) {
    putBits(b, 15, 5);
    putBits(b, (unsigned)n - 6, 1);
  } else if (n <= 10) {
    putBits(b, 31, 5);
    putBits(b, (unsigned)n - 7, 2);
  } else {
    putBits(b, 31, 7);
    putBits(b, (unsigned)n - 10, 7);
  }
}

static void distanceBits(Bits *b, size_t at, size_t distance) {
  if (distance <= 32) {
    putBits(b, 1, 1);
    putBits(b, (unsigned)distance - 1, 5);
  } else {
    unsigned width = at >= 33 && at - 33 < 2048 ? bitLength(at - 33) : 12;
    if (distance - 33 >= (1u << width))
      fail("LZ distance exceeds its position-dependent width");
    putBits(b, 0, 1);
    putBits(b, (unsigned)distance - 33, width);
  }
}

static unsigned mtf(unsigned char table[16], unsigned value) {
  unsigned i = 0;
  while (i < 16 && table[i] != value)
    i++;
  if (i == 16)
    fail("pixel exceeds four bits");
  memmove(table + 1, table, i);
  table[0] = value;
  return i;
}

static Buffer encode(const Buffer *ship, const Buffer *pack, LzMachine machine,
                     int kind) {
  Matcher m = matcher(pack);
  size_t at = 0;
  bool deferred = false;
  bool palette = kind == 1;
  Bits bits = {0};
  Buffer out = {0};
  unsigned char table[16];
  for (unsigned i = 0; i < 16; i++)
    table[i] = i;
  size_t flagAt = 0;
  unsigned group = 0;
  if (kind != 3)
    byte(&out, kind);
  if (kind == 3)
    palette = true;
  size_t window = palette ? 4092 : machine.window;
  size_t ahead = palette ? machine.paletteAhead : machine.ahead;
  size_t reach = palette ? 4095 : machine.distance;
  while (at < pack->len) {
    size_t old = oldest(at, pack->len, window, ahead);
    Token t = search(&m, at, palette ? 272 : 137, lowest(old, at, reach));
    bool lazy = !deferred;
    if (palette)
      deferred = false;
    if (kind != 2 && lazy && t.length > 1 && at + t.length < pack->len) {
      size_t alt =
          search(&m, at + 1, palette ? 272 : 137, lowest(old, at + 1, reach))
              .length;
      size_t next = search(&m, at + t.length, palette ? 272 : 137,
                           lowest(old, at + t.length, reach))
                        .length;
      if (alt > 2 && alt + 1 >= t.length + next) {
        t = (Token){1, 0};
        deferred = true;
      }
    }
    if (t.distance) {
      for (size_t i = 0; i < t.length; i++)
        if (ship->data[at + i] != ship->data[at + i - t.distance])
          fail("LZ plan does not reconstruct the shipped bytes");
    }
    if (palette) {
      if (!group) {
        flagAt = out.len;
        byte(&out, 0);
      }
      if (t.distance) {
        out.data[flagAt] |= 1 << (7 - group);
        byte(&out,
             (t.distance >> 8) << 4 | (t.length <= 16 ? t.length - 1 : 0));
        byte(&out, t.distance);
        if (t.length >= 17)
          byte(&out, t.length - 17);
      } else
        byte(&out, ship->data[at]);
      group = (group + 1) % 8;
    } else if (t.distance) {
      lengthBits(&bits, t.length);
      distanceBits(&bits, at, t.distance);
    } else {
      putBits(&bits, 1, 1);
      unsigned value = ship->data[at];
      if (kind == 2) {
        unsigned lo = mtf(table, value & 15), hi = mtf(table, value >> 4);
        unsigned width = bitLength(lo > hi ? lo : hi);
        if (width < 2)
          width = 2;
        if (width == 2)
          putBits(&bits, 1, 1);
        else {
          putBits(&bits, 0, 1);
          putBits(&bits, width == 3, 1);
        }
        putBits(&bits, lo, width);
        putBits(&bits, hi, width);
      } else
        putBits(&bits, value, 8);
    }
    at += t.length;
  }
  if (palette) {
    if (!group) {
      flagAt = out.len;
      byte(&out, 0);
    }
    out.data[flagAt] |= 1 << (7 - group);
    half(&out, 0);
  } else {
    putBits(&bits, 0, 1);
    putBits(&bits, 31, 7);
    putBits(&bits, 0, 7);
    append(&out, bits.bytes.data, bits.bytes.len);
    free(bits.bytes.data);
  }
  free(m.previous);
  return out;
}

Buffer compressLz(const Buffer *ship, const Buffer *pack, const char *kind,
                  LzMachine machine) {
  if (ship->len != pack->len)
    fail("packing inputs differ in length");
  if (!machine.window || !machine.distance || machine.distance > 4128 ||
      machine.ahead > SIZE_MAX - machine.window)
    fail("invalid LZ machine");
  if (!strcmp(kind, "general"))
    return encode(ship, pack, machine, 0);
  if (!strcmp(kind, "palette"))
    return encode(ship, pack, machine, 1);
  if (!strcmp(kind, "plz"))
    return encode(ship, pack, machine, 3);
  if (!strcmp(kind, "mtf") || !strcmp(kind, "mtf4"))
    return encode(ship, pack, packer, 2);
  if (strcmp(kind, "lz") && strcmp(kind, "tagged"))
    fail("unknown LZ kind: %s", kind);
  Buffer a = encode(ship, pack, machine, 0), b = encode(ship, pack, machine, 1);
  if (b.len <= a.len) {
    free(a.data);
    return b;
  }
  free(b.data);
  return a;
}

Buffer compressZeros(const Buffer *p) {
  Buffer out = {0};
  for (size_t i = 0; i < p->len;) {
    if (p->data[i]) {
      if (p->data[i] > 0xdf)
        fail("zero-skip literal exceeds 0xdf");
      byte(&out, p->data[i++]);
    } else {
      size_t n = 1;
      while (n < 32 && i + n < p->len && !p->data[i + n])
        n++;
      byte(&out, 0xdf + n);
      i += n;
    }
  }
  byte(&out, 0);
  return out;
}

Buffer compressDelta7(const Buffer *data) {
  Bits b = {0};
  unsigned previous = 0;
  for (size_t i = 0; i < data->len; i++) {
    unsigned p = data->data[i], d = (p - previous) & 127;
    if (p > 127)
      fail("delta7 pixel exceeds seven bits");
    if (!d)
      putBits(&b, 0, 2);
    else if (d <= 2) {
      putBits(&b, 1, 3);
      putBits(&b, d - 1, 1);
    } else if (d <= 10) {
      putBits(&b, 3, 3);
      putBits(&b, d - 3, 3);
    } else if (d <= 26) {
      putBits(&b, 2, 4);
      putBits(&b, d - 11, 4);
    } else if (d <= 101) {
      putBits(&b, 6, 3);
      putBits(&b, p, 7);
    } else if (d <= 117) {
      putBits(&b, 10, 4);
      putBits(&b, 117 - d, 4);
    } else if (d <= 125) {
      putBits(&b, 7, 3);
      putBits(&b, 125 - d, 3);
    } else {
      putBits(&b, 5, 3);
      putBits(&b, 127 - d, 1);
    }
    previous = p;
  }
  putBits(&b, 0, 1);
  alignBits(&b, 16, 0);
  return b.bytes;
}

Buffer deltaMap(const Buffer *data, unsigned mode) {
  if (mode > 2 || !data->len || data->len % 2)
    fail("delta map needs mode 0..2 and whole halfwords");
  Buffer transformed = {0}, out = {0};
  unsigned previous = 0;
  byte(&out, mode);
  for (size_t i = 0; i < data->len; i += 2) {
    unsigned v = read16(data->data + i);
    half(&transformed, mode ? v ^ previous : v);
    previous = v;
  }
  if (mode == 1) {
    for (size_t i = 1; i < transformed.len; i += 2)
      byte(&out, transformed.data[i]);
    for (size_t i = 0; i < transformed.len; i += 2)
      byte(&out, transformed.data[i]);
  } else
    append(&out, transformed.data, transformed.len);
  free(transformed.data);
  return out;
}

Buffer compressIcon(const Buffer *data) {
  static const char *codes[] = {
      "0",          "100",       "1010",     "1011",       "1100",
      "1101",       "1110",      "111100",   "111101",     "111110",
      "11111100",   "11111101",  "11111110", "1111111100", "1111111101",
      "1111111110", "1111111111"};
  Bits b = {0};
  uint8_t table[16];
  for (unsigned i = 0; i < 16; i++)
    table[i] = i;
  for (size_t i = 0; i <= data->len; i++) {
    unsigned index = i == data->len ? 16 : mtf(table, data->data[i]);
    for (const char *s = codes[index]; *s; s++)
      putBits(&b, *s - '0', 1);
  }
  alignBits(&b, 8, 1);
  return b.bytes;
}

Buffer compressHalfwords(const Buffer *data) {
  if (data->len % 2)
    fail("halfword LZ needs whole halfwords");
  Buffer out = {0};
  size_t at = 0, flags = 0;
  unsigned group = 0;
  for (;;) {
    if (!group) {
      flags = out.len;
      half(&out, 0);
    }
    size_t length = 0, distance = 0;
    for (size_t back = 1; back <= at && back <= 2047; back++) {
      size_t n = 0;
      while (n < 33 && at + n < data->len / 2 &&
             read16(data->data + 2 * (at + n)) ==
                 read16(data->data + 2 * (at + n - back)))
        n++;
      if (n > length) {
        length = n;
        distance = back;
      }
    }
    if (at == data->len / 2 || length >= 2) {
      write16(out.data + flags, read16(out.data + flags) | (0x8000 >> group));
      half(&out, at == data->len / 2 ? 0 : (distance << 5) | (length - 2));
      if (at == data->len / 2)
        break;
      at += length;
    } else {
      half(&out, read16(data->data + 2 * at));
      at++;
    }
    group = (group + 1) % 16;
  }
  return out;
}

Buffer compressArena(const Buffer *data, const Buffer *arena) {
  size_t n = data->len, base = arena->len + 2;
  size_t low = base > 4095 ? base - 4095 : 0;
  if (low > arena->len)
    low = arena->len;
  size_t *lengths = allocate(n, sizeof(size_t)),
         *sources = allocate(n, sizeof(size_t)),
         *steps = allocate(n, sizeof(size_t));
  for (size_t at = 0; at < n; at++) {
    steps[at] = 1;
    size_t limit = n - at;
    if (limit > 273)
      limit = 273;
    size_t available = arena->len > at ? arena->len - at : 0;
    if (limit > available)
      limit = available;
    if (limit < 3)
      continue;
    for (size_t start = low; start + 1 < arena->len; start++) {
      if (read16(arena->data + start) != read16(data->data + at))
        continue;
      size_t count = 2;
      while (count < limit && start + count < arena->len &&
             arena->data[start + count] == data->data[at + count])
        count++;
      if (count > lengths[at]) {
        lengths[at] = count;
        sources[at] = start;
      }
    }
    if (lengths[at] < 3)
      lengths[at] = 0;
  }
  for (size_t i = n; i; i--) {
    size_t at = i - 1, own = lengths[at];
    if (own < 3)
      continue;
    steps[at] = own;
    size_t next = at + 1;
    while (next < at + own && lengths[next] <= own)
      next++;
    while (next < n && next - at < own) {
      if (steps[next] > own) {
        steps[at] = next - at < 3 ? 1 : next - at;
        break;
      }
      next += steps[next];
    }
  }
  Buffer literals = {0}, coded = {0};
  size_t flagAt = 0;
  unsigned group = 0;
  for (size_t at = 0;;) {
    if (!group) {
      flagAt = coded.len;
      byte(&coded, 0);
    }
    if (at == n) {
      half(&coded, 0);
      break;
    }
    size_t step = steps[at];
    if (step == 1) {
      coded.data[flagAt] |= 1 << group;
      byte(&literals, data->data[at]);
    } else {
      size_t distance = base - sources[at];
      unsigned value =
          (unsigned)distance | (step <= 17 ? (unsigned)(step - 2) << 12 : 0);
      byte(&coded, value >> 8);
      byte(&coded, value);
      if (step >= 18)
        byte(&coded, step - 18);
    }
    at += step;
    group = (group + 1) % 8;
  }
  Buffer out = {0};
  if (literals.len + 2 > 65535)
    fail("arena literal block exceeds its split field");
  size_t compressed = literals.len + 2 + coded.len;
  if (compressed > 1024 || compressed * 10 >= (n ? n - 1 : 0) * 9) {
    if (n && (data->data[n - 1] || memchr(data->data, 0, n - 1)))
      fail("raw arena stream must end in its only zero");
    half(&out, 0);
    append(&out, data->data, n);
  } else {
    half(&out, (unsigned)literals.len + 2);
    append(&out, literals.data, literals.len);
    append(&out, coded.data, coded.len);
  }
  free(lengths);
  free(sources);
  free(steps);
  free(literals.data);
  free(coded.data);
  return out;
}
