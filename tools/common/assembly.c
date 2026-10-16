/* Render bytes with labels and relocatable pointers as GNU assembler source. */
#include "assembly.h"
#include <ctype.h>
#include <inttypes.h>
bool identifier(const char *s) {
  if (*s != '_' && !isalpha((unsigned char)*s))
    return false;
  for (s++; *s; s++)
    if (*s != '_' && !isalnum((unsigned char)*s))
      return false;
  return true;
}

void labelAt(Assembly *a, const char *name, size_t at, bool global) {
  if (!identifier(name))
    fail("invalid assembly symbol: %s", name);
  if (a->labelCount && at < a->labels[a->labelCount - 1].offset)
    fail("assembly labels must follow byte order");
  for (size_t i = 0; i < a->labelCount; i++)
    if (!strcmp(a->labels[i].name, name))
      fail("duplicate label: %s", name);
  a->labels = resize(a->labels, a->labelCount + 1, sizeof(Label));
  a->labels[a->labelCount++] = (Label){at, copyString(name), global};
}

void pointerAt(Assembly *a, const char *name, size_t at, int64_t addend) {
  if (!identifier(name))
    fail("invalid pointer symbol: %s", name);
  if (a->pointerCount && at < a->pointers[a->pointerCount - 1].offset + 4)
    fail("assembly pointers overlap or are out of order");
  a->pointers = resize(a->pointers, a->pointerCount + 1, sizeof(Pointer));
  a->pointers[a->pointerCount++] = (Pointer){at, copyString(name), addend};
}

void addPointer(Assembly *a, const char *name, int64_t addend) {
  pointerAt(a, name, a->bytes.len, addend);
  word(&a->bytes, 0);
}

void alignAssembly(Assembly *a, size_t n, uint8_t fill) {
  if (!n || (n & (n - 1)))
    fail("alignment is not a power of two");
  if (a->alignment < n)
    a->alignment = n;
  while (a->bytes.len % n)
    byte(&a->bytes, fill);
}

void appendAssembly(Assembly *a, Assembly *other) {
  size_t at = a->bytes.len;
  if (other->alignment && at % other->alignment)
    fail("appended assembly is unaligned");
  if (a->alignment < other->alignment)
    a->alignment = other->alignment;
  append(&a->bytes, other->bytes.data, other->bytes.len);
  for (size_t i = 0; i < other->labelCount; i++)
    labelAt(a, other->labels[i].name, at + other->labels[i].offset,
            other->labels[i].global);
  for (size_t i = 0; i < other->pointerCount; i++)
    pointerAt(a, other->pointers[i].symbol, at + other->pointers[i].offset,
              other->pointers[i].addend);
}

Buffer assemblySource(const Assembly *a) {
  Buffer text = {0};
  if (a->alignment > 1)
    print(&text, "\t.balign %zu\n", a->alignment);
  /* Assemblies are built in byte order. Walk each label and pointer once,
   * even when a sprite bank contains several megabytes of pixel data. */
  size_t row = 0, label = 0, pointer = 0;
  for (size_t at = 0; at <= a->bytes.len;) {
    while (label < a->labelCount && a->labels[label].offset == at) {
      const Label *l = a->labels + label++;
      if (row) {
        byte(&text, '\n');
        row = 0;
      }
      if (l->global)
        print(&text, "\t.global %s\n", l->name);
      print(&text, "%s:\n", l->name);
    }
    if (at == a->bytes.len)
      break;
    const Pointer *p =
        pointer < a->pointerCount && a->pointers[pointer].offset == at
            ? a->pointers + pointer++
            : NULL;
    if (p) {
      if (at + 4 > a->bytes.len || read32(a->bytes.data + at))
        fail("invalid assembly pointer at %zu", at);
      if (row) {
        byte(&text, '\n');
        row = 0;
      }
      print(&text, "\t.4byte %s", p->symbol);
      if (p->addend > 0)
        print(&text, " + %" PRId64, p->addend);
      else if (p->addend < 0)
        print(&text, " - %" PRId64, -p->addend);
      byte(&text, '\n');
      at += 4;
      continue;
    }
    static const char hex[] = "0123456789abcdef";
    uint8_t value = a->bytes.data[at++];
    const char digits[] = {'0', 'x', hex[value >> 4], hex[value & 15]};
    append(&text, row ? ", " : "\t.byte ", row ? 2 : 7);
    append(&text, digits, sizeof(digits));
    if (++row == 16) {
      byte(&text, '\n');
      row = 0;
    }
  }
  if (label != a->labelCount || pointer != a->pointerCount)
    fail("assembly label or pointer lies outside its data");
  if (row)
    byte(&text, '\n');
  return text;
}

void freeAssembly(Assembly *a) {
  free(a->bytes.data);
  for (size_t i = 0; i < a->labelCount; i++)
    free(a->labels[i].name);
  for (size_t i = 0; i < a->pointerCount; i++)
    free(a->pointers[i].symbol);
  free(a->labels);
  free(a->pointers);
  *a = (Assembly){0};
}
