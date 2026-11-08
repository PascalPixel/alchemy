#include "types.h"
#include "edition.h"
#include "fixed_math.h"

// The number of article kinds in each edition's table (articles.c), and the
// mask a name's kind is read through.
#if (LANGUAGE == LANGUAGE_ITALIAN)
#define ARTICLE_KINDS 11
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define ARTICLE_KINDS 5
#else
#define ARTICLE_KINDS 8
#endif

#if (LANGUAGE == LANGUAGE_SPANISH)
#define ARTICLE_KIND_MASK 15
#else
#define ARTICLE_KIND_MASK 7
#endif

// The article strings by kind, copied whole (articles.c).
struct ArticleTable {
  u8 *text[ARTICLE_KINDS];
};

extern const struct ArticleTable gText_ArticleTable;
#if (LANGUAGE == LANGUAGE_SPANISH)
extern const struct ArticleTable gText_IndefiniteArticles;
#endif
#if (LANGUAGE == LANGUAGE_FRENCH)
// "de", elided and whole, copied whole (articles.c).
struct DeArticles {
  u8 *text[2];
};

extern const struct DeArticles gText_DeArticles;
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH)
/// Copies a name into the decoded text at pos, with its English article in
/// front when no is 1 (or 3 with plural clear) and a plural ending after it
/// when no is 2 (or 3 with plural set); mode wraps the name in line breaks.
/// The Broken Seal's Message_AppendArticleName (window.c) is its twin.
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
    table = gText_ArticleTable;
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

#elif (LANGUAGE != LANGUAGE_JAPANESE)
/// Copies a name into the decoded text at pos: forms 0 and 4 take no article,
/// the others the article of the name's kind (in Spanish, form bit 3 asks for
/// the indefinite article), its first letter a capital when suffix is 2. In
/// French, form 4 puts "de" (elided before a capital vowel) in front. mode
/// wraps the name in line breaks.
/// The Broken Seal's Message_AppendArticleName (window.c) is its twin.
/// Returns the next position in the 512-entry ring.
u32 Func_0803aed8(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural,
                  s32 *suffix) {
  struct ArticleTable table;
#if (LANGUAGE == LANGUAGE_SPANISH)
  struct ArticleTable indefiniteTable;
  u16 count;
  s16 indefinite;
#endif
  s32 kind;
  u16 head;
  u16 c;
  u8 *p;
  s8 c8;
  s32 cnt;

#if (LANGUAGE == LANGUAGE_SPANISH)
  count = no & 7;
  indefinite = no & 8;
#endif
  if (mode != 0) {
    entry[pos] = ' ';
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
  }
#if (LANGUAGE == LANGUAGE_FRENCH)
  if (no == 4) {
    struct DeArticles de;
    u8 *word;
    s8 letter;
    s32 n;

    head = name[0];
    de = gText_DeArticles;
    if (head == 'A' || head == 'I' || head == 'U' || head == 'E' || head == 'O')
      word = de.text[0];
    else
      word = de.text[1];
    for (n = 0; n < 8; n++) {
      if ((letter = *word++) == 0)
        break;
      entry[pos] = letter;
      pos = (pos + 1) & 0x1ff;
    }
  }
#endif
#if (LANGUAGE == LANGUAGE_SPANISH)
  if (count == 0 || count == 4) {
#else
  if (no == 0 || no == 4) {
#endif
    head = name[0];
    if (head == 29)
      name += 2;
  } else {
    kind = 0;
    table = gText_ArticleTable;
#if (LANGUAGE == LANGUAGE_SPANISH)
    indefiniteTable = gText_IndefiniteArticles;
#endif
    if (name[0] == 29) {
      kind = name[1];
      name += 2;
    }
#if (LANGUAGE == LANGUAGE_SPANISH)
    if (indefinite == 0)
      p = table.text[kind & ARTICLE_KIND_MASK];
    else
      p = indefiniteTable.text[kind & ARTICLE_KIND_MASK];
#else
    p = table.text[kind & ARTICLE_KIND_MASK];
#endif
    for (cnt = 0; cnt < 8; cnt++) {
      if ((c8 = *p++) == 0)
        break;
      if (cnt == 0 && *suffix == 2)
        c8 -= 'a' - 'A';
      entry[pos] = c8;
      pos = (pos + 1) & 0x1ff;
    }
  }
  while (*name != 0) {
    c = *name++;
#if (LANGUAGE == LANGUAGE_ITALIAN)
    entry[pos] = c;
    pos = (pos + 1) & 0x1ff;
#else
    entry[pos] = (s16)c;
    pos = (pos + 1) & 0x1ff;
#if (LANGUAGE == LANGUAGE_GERMAN)
    if (c == 's' || c == 'S' || c == 'z' || c == 'Z' || c == 'x' || c == 'X')
#else
    if (c == 'S' || c == 's')
#endif
      *suffix = 1;
    else
      *suffix = 0;
#endif
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
#endif
