// Draft of UiText_AppendArticleName: differ 26 of 203 (tla-en, 2026-10-18).
// Its ☀️ twin is games/tbs/src/window/window.c's UiText_AppendArticleName:
// it copies a name into the decoded text, with its article in front when the
// count is one, and an English plural ending after it otherwise.
//
// Left: instruction order only (the position increments and the 0x1ff mask
// build sit one or two places apart; the pool loads move with them).

#include "types.h"
#include "fixed_math.h"

// The article strings by kind, as The Broken Seal's window.c has them.
struct ArticleTable {
  u8 *text[8];
};

extern const struct ArticleTable gUiText_ArticleTable;

u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no,
                             s32 plural, s32 *suffix) {
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
    pos = (pos + 1) & 0x1ff;
  }
  return pos;
}
