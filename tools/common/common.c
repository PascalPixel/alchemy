#define _POSIX_C_SOURCE 200809L
#include "common.h"
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *program = "ags";

void startTool(int argc, char **argv, const char *usage) {
  const char *slash = strrchr(argv[0], '/');
  program = slash ? slash + 1 : argv[0];
  if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) {
    puts(usage);
    exit(0);
  }
}

_Noreturn void fail(const char *format, ...) {
  va_list args;
  va_start(args, format);
  fprintf(stderr, "%s: ", program);
  vfprintf(stderr, format, args);
  va_end(args);
  fputc('\n', stderr);
  exit(1);
}

void *resize(void *data, size_t count, size_t size) {
  if (size && count > SIZE_MAX / size)
    fail("allocation is too large");
  void *result = realloc(data, count * size ? count * size : 1);
  if (!result)
    fail("out of memory");
  return result;
}

void *allocate(size_t count, size_t size) {
  void *result = resize(NULL, count, size);
  memset(result, 0, count * size);
  return result;
}

char *copyString(const char *text) {
  size_t size = strlen(text) + 1;
  char *result = allocate(size, 1);
  memcpy(result, text, size);
  return result;
}

static char *vformat(const char *format, va_list args) {
  va_list copy;
  va_copy(copy, args);
  int size = vsnprintf(NULL, 0, format, copy);
  va_end(copy);
  if (size < 0)
    fail("formatting failed");
  char *text = allocate((size_t)size + 1, 1);
  vsnprintf(text, (size_t)size + 1, format, args);
  return text;
}

char *formatString(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *text = vformat(format, args);
  va_end(args);
  return text;
}

void reserve(Buffer *b, size_t extra) {
  if (extra > SIZE_MAX - b->len - 1)
    fail("buffer is too large");
  size_t need = b->len + extra + 1;
  if (need > b->cap) {
    size_t cap = need < 256 ? 256 : need;
    if (b->cap <= SIZE_MAX / 2 && cap < b->cap * 2)
      cap = b->cap * 2;
    b->data = resize(b->data, cap, 1);
    b->cap = cap;
  }
  b->data[b->len] = 0;
}

void append(Buffer *b, const void *data, size_t size) {
  reserve(b, size);
  if (size)
    memcpy(b->data + b->len, data, size);
  b->len += size;
  b->data[b->len] = 0;
}

void byte(Buffer *b, unsigned value) {
  uint8_t v = value;
  append(b, &v, 1);
}

void half(Buffer *b, unsigned value) {
  byte(b, value);
  byte(b, value >> 8);
}

void word(Buffer *b, uint32_t v) {
  half(b, v);
  half(b, v >> 16);
}

void print(Buffer *b, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *text = vformat(format, args);
  va_end(args);
  append(b, text, strlen(text));
  free(text);
}

void alignBuffer(Buffer *b, size_t n) {
  if (!n)
    fail("zero alignment");
  while (b->len % n)
    byte(b, 0);
}

uint16_t read16(const uint8_t *p) { return p[0] | (unsigned)p[1] << 8; }
uint32_t read32(const uint8_t *p) {
  return read16(p) | (uint32_t)read16(p + 2) << 16;
}

void write16(uint8_t *p, unsigned v) {
  p[0] = v;
  p[1] = v >> 8;
}

void write32(uint8_t *p, uint32_t v) {
  write16(p, v);
  write16(p + 2, v >> 16);
}

Buffer readFile(const char *path) {
  FILE *file = fopen(path, "rb");
  if (!file)
    fail("%s: %s", path, strerror(errno));
  Buffer b = {0};
  uint8_t chunk[16384];
  size_t n;
  while ((n = fread(chunk, 1, sizeof(chunk), file)))
    append(&b, chunk, n);
  if (ferror(file))
    fail("%s: read failed", path);
  fclose(file);
  reserve(&b, 0);
  return b;
}

bool exists(const char *path) {
  struct stat s;
  return !stat(path, &s) && S_ISREG(s.st_mode);
}

void makeParents(const char *path) {
  if (!*path)
    fail("output path is empty");
  char *copy = copyString(path);
  for (char *p = copy + 1; *p; p++)
    if (*p == '/') {
      *p = 0;
      if (mkdir(copy, 0777) && errno != EEXIST)
        fail("%s: %s", copy, strerror(errno));
      *p = '/';
    }
  free(copy);
}
/* Editions may encode a shared asset at the same time. Rename a complete
 * sibling file so readers never see a partially written asset. */
void writeFile(const char *path, const Buffer *b) {
  char *temp = formatString("%s.XXXXXX", path);
  int fd = mkstemp(temp);
  if (fd < 0)
    fail("%s: %s", path, strerror(errno));
  FILE *file = fdopen(fd, "wb");
  if (!file) {
    close(fd);
    unlink(temp);
    fail("%s: open failed", path);
  }
  bool ok = !b->len || fwrite(b->data, 1, b->len, file) == b->len;
  if (fclose(file))
    ok = false;
  if (!ok || rename(temp, path)) {
    unlink(temp);
    fail("%s: write failed", path);
  }
  free(temp);
}

void writeChanged(const char *path, const Buffer *b) {
  if (exists(path)) {
    Buffer old = readFile(path);
    bool same =
        old.len == b->len && (!b->len || !memcmp(old.data, b->data, b->len));
    free(old.data);
    if (same)
      return;
  }
  makeParents(path);
  writeFile(path, b);
}

char *trim(char *s) {
  while (isspace((unsigned char)*s))
    s++;
  size_t len = strlen(s);
  while (len && isspace((unsigned char)s[len - 1]))
    s[--len] = 0;
  return s;
}

void addString(Strings *s, const char *text) {
  if (s->len == s->cap) {
    s->cap = s->cap ? s->cap * 2 : 16;
    s->items = resize(s->items, s->cap, sizeof(char *));
  }
  s->items[s->len++] = copyString(text);
}

bool hasString(const Strings *s, const char *text) {
  for (size_t i = 0; i < s->len; i++)
    if (!strcmp(s->items[i], text))
      return true;
  return false;
}

static int compareStrings(const void *a, const void *b) {
  return strcmp(*(char *const *)a, *(char *const *)b);
}

void sortStrings(Strings *s) {
  if (s->len > 1)
    qsort(s->items, s->len, sizeof(char *), compareStrings);
}

Strings split(const char *s, char separator) {
  Strings result = {0};
  const char *start = s;
  for (;; s++)
    if (*s == separator || !*s) {
      size_t n = s - start;
      char *part = allocate(n + 1, 1);
      memcpy(part, start, n);
      addString(&result, part);
      free(part);
      if (!*s)
        break;
      start = s + 1;
    }
  return result;
}

void freeStrings(Strings *s) {
  for (size_t i = 0; i < s->len; i++)
    free(s->items[i]);
  free(s->items);
  *s = (Strings){0};
}

Strings words(const char *s) {
  Strings out = {0};
  while (*s) {
    while (isspace((unsigned char)*s))
      s++;
    const char *start = s;
    while (*s && !isspace((unsigned char)*s))
      s++;
    if (s != start) {
      char *part = formatString("%.*s", (int)(s - start), start);
      addString(&out, part);
      free(part);
    }
  }
  return out;
}

uint64_t decimalOrHex(const char *s, uint64_t max) {
  return !strncmp(s, "0x", 2) ? number(s + 2, 16, max) : number(s, 10, max);
}

uint64_t number(const char *text, int base, uint64_t maximum) {
  if (!*text || *text == '-' || isspace((unsigned char)*text))
    fail("invalid number: %s", text);
  errno = 0;
  char *end;
  unsigned long long value = strtoull(text, &end, base);
  if (errno || *end || value > maximum)
    fail("number out of range: %s", text);
  return value;
}

char *beside(const char *path, const char *name) {
  const char *slash = strrchr(path, '/');
  return slash ? formatString("%.*s/%s", (int)(slash - path), path, name)
               : copyString(name);
}

const char *extension(const char *path) {
  const char *p = strrchr(path, '.');
  return p ? p + 1 : "";
}

void putBits(Bits *b, uint32_t value, unsigned count) {
  for (unsigned i = 0; i < count; i++) {
    if (!(b->count % 8))
      byte(&b->bytes, 0);
    b->bytes.data[b->count / 8] |= ((value >> i) & 1) << (b->count % 8);
    b->count++;
  }
}

void alignBits(Bits *b, unsigned n, unsigned fill) {
  while (b->count % n)
    putBits(b, fill, 1);
}

unsigned bitLength(size_t v) {
  unsigned n = 0;
  while (v) {
    n++;
    v >>= 1;
  }
  return n;
}
