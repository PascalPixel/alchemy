#include "types.h"
#include "fixed_math.h"

// The article strings by kind, copied whole (articles.c).
struct ArticleTable {
  u8 *text[8];
};

extern const struct ArticleTable gUiText_ArticleTable;

/// Copies a name into the decoded text at pos, with its English article in
/// front when no is 1 (or 3 with plural clear) and a plural ending after it
/// when no is 2 (or 3 with plural set); mode wraps the name in line breaks.
/// The Broken Seal's UiText_AppendArticleName (window.c) is its twin.
/// Returns the next position in the 512-entry ring.
u32 Func_0803aed8(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural,
                  s32 *suffix) {
  struct ArticleTable table;
  s32 kind;
  u16 head;
  u16 c;
  u8 *p;
  s8 c8;
  s32 cnt;

  if (mode != 0) {
    entry[pos] = ' ';
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    /* FAKEMATCH(barrier): the last code is stored before pos steps; as plain
     * statements, 2 of 203 instructions differ in tla-en. */
    asm("");
    pos = (pos + 1) & 0x1ff;
  }
  if (no == 1 || (no == 3 && plural == 0)) {
    kind = 0;
    table = gUiText_ArticleTable;
    head = name[0];
    if (head == 29) {
      kind = name[1] - 1;
      name += 2;
    }
    if (kind == 0) {
      if (head == 'A' || head == 'I' || head == 'U' || head == 'E' ||
          head == 'O')
        kind = 2;
      else
        kind = 1;
    }
    p = table.text[kind & 7];
    for (cnt = 0; cnt < 8; cnt++) {
      if ((c8 = *p++) == 0)
        break;
      entry[pos] = c8;
      pos = (pos + 1) & 0x1ff;
    }
  } else {
    head = name[0];
    if (head == 29)
      name += 2;
  }
  while (*name != 0) {
    c = *name++;
    entry[pos] = (s16)c;
    pos = (pos + 1) & 0x1ff;
    if (c == 'S' || c == 's')
      *suffix = 1;
    else
      *suffix = 0;
  }
  if (no == 2 || (no == 3 && plural != 0)) {
    if (*suffix != 0) {
      entry[pos] = 'e';
      pos = (pos + 1) & 0x1ff;
    }
    entry[pos] = 's';
    pos = (pos + 1) & 0x1ff;
  }
  if (mode != 0) {
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x08;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = ' ';
    /* FAKEMATCH(barrier): the last code is stored before pos steps; as plain
     * statements, 2 of 203 instructions differ in tla-en. */
    asm("");
    pos = (pos + 1) & 0x1ff;
  }
  return pos;
}
