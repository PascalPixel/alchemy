/// @file
/// The message decoder: articles, names and numbers written out.
// Part of window.c.

#include "../window_internal.h"

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

extern const struct ArticleTable gArticle_Table;
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
/// Copies a name into the decoded text at writePosition, with its English
/// article in front when articleForm is 1 (or 3 with plural clear) and a
/// plural ending after it when articleForm is 2 (or 3 with plural set); mode
/// wraps the name in line breaks.
/// Returns the next position in the 512-entry ring.
u32 Message_AppendArticleName(s32 mode, u16 *name, u32 writePosition,
                              u16 *entries, s32 articleForm, s32 plural,
                              s32 *suffix) {
  struct ArticleTable table;
  s32 kind;
  u16 head;
  u16 c;
  u8 *p;
  s8 c8;
  s32 cnt;

  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = 0x0a;
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = 0x0a;
    writePosition = (writePosition + 1) & 0x1ff;
  }
  if (articleForm == 1 || (articleForm == 3 && plural == 0)) {
    kind = 0;
    table = gArticle_Table;
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
      entries[writePosition] = c8;
      writePosition = (writePosition + 1) & 0x1ff;
    }
  } else {
    head = name[0];
    if (head == 29)
      name += 2;
  }
  while (*name != 0) {
    c = *name++;
    entries[writePosition] = (s16)c;
    writePosition = (writePosition + 1) & 0x1ff;
    if (c == 'S' || c == 's')
      *suffix = 1;
    else
      *suffix = 0;
  }
  if (articleForm == 2 || (articleForm == 3 && plural != 0)) {
    if (*suffix != 0) {
      entries[writePosition] = 'e';
      writePosition = (writePosition + 1) & 0x1ff;
    }
    entries[writePosition] = 's';
    writePosition = (writePosition + 1) & 0x1ff;
  }
  if (mode != 0) {
    entries[writePosition] = 0x0a;
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = 0x08;
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & 0x1ff;
  }
  return writePosition;
}

#elif (LANGUAGE != LANGUAGE_JAPANESE)
/// Copies a name into the decoded text at writePosition: forms 0 and 4 take no
/// article, the others the article of the name's kind (in Spanish, form bit 3
/// asks for the indefinite article), its first letter a capital when suffix is
/// 2. In French, form 4 puts "de" (elided before a capital vowel) in front.
/// mode wraps the name in line breaks.
/// Returns the next position in the 512-entry ring.
u32 Message_AppendArticleName(s32 mode, u16 *name, u32 writePosition,
                              u16 *entries, s32 articleForm, s32 plural,
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
  count = articleForm & 7;
  indefinite = articleForm & 8;
#endif
  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = 0x0a;
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = 0x0a;
    writePosition = (writePosition + 1) & 0x1ff;
  }
#if (LANGUAGE == LANGUAGE_FRENCH)
  if (articleForm == 4) {
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
      entries[writePosition] = letter;
      writePosition = (writePosition + 1) & 0x1ff;
    }
  }
#endif
#if (LANGUAGE == LANGUAGE_SPANISH)
  if (count == 0 || count == 4) {
#else
  if (articleForm == 0 || articleForm == 4) {
#endif
    head = name[0];
    if (head == 29)
      name += 2;
  } else {
    kind = 0;
    table = gArticle_Table;
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
      entries[writePosition] = c8;
      writePosition = (writePosition + 1) & 0x1ff;
    }
  }
  while (*name != 0) {
    c = *name++;
#if (LANGUAGE == LANGUAGE_ITALIAN)
    entries[writePosition] = c;
    writePosition = (writePosition + 1) & 0x1ff;
#else
    entries[writePosition] = (s16)c;
    writePosition = (writePosition + 1) & 0x1ff;
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
    entries[writePosition] = 0x0a;
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = 0x08;
    writePosition = (writePosition + 1) & 0x1ff;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & 0x1ff;
  }
  return writePosition;
}
#endif

#define TEXT_DECODER_SLOT 200
#define TEXT_BUFFER_MASK (UI_TEXT_ENTRY_COUNT - 1)

// The European editions keep a second item name table after the first,
// read for the plural forms; its distance from MsgItemName is a message id.
#define ITEM_PLURAL_NAME_OFFSET 461

// What suffix starts as: the European editions use 2 to capitalize the
// first letter of a sentence.
#if TLA_PATCH_2
#define SUFFIX_START 2
#else
#define SUFFIX_START 0
#endif

extern const u8 Text_DecodeSymbolCode[];
extern u8 Text_DecodeSymbolCodeSize[];
extern u8 MsgClassName[], MsgItemName[], MsgAbilityNothing[],
    MsgDebugAreaName[];

#if TLA_PATCH_2
#if LANGUAGE == LANGUAGE_FRENCH
extern const s8 gLocalizedWord_ItemPronoun[];
#else
// The possessives (articles.c), copied whole: Spanish {singular, plural},
// German and Italian one for each item gender (gLocalizedWord_ItemGenders).
#if LANGUAGE == LANGUAGE_SPANISH
#define POSSESSIVE_COUNT 2
#else
#define POSSESSIVE_COUNT 4
#endif

struct PossessiveTable {
  s8 *text[POSSESSIVE_COUNT];
};
extern const struct PossessiveTable gLocalizedWord_ItemPossessives;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
extern const s8 gLocalizedWord_ItemGenders[];
#endif
#if LANGUAGE == LANGUAGE_GERMAN
extern const u8 *const gText_NumberWords[];
#endif
#endif
#endif

u32 MessageArgument_Lookup(u32 name, u32 clear);
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);
void MessageArgument_ClearAll(void);
u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);
/* VIEW(window): routine Unit_Get as struct BattleUnit *Unit_Get(s32 unitId);
 * owner_state.h has void *Unit_Get(s32 owner); not tried: Unit_Get's
 * declaration holds a FAKEMATCH tag. */
struct BattleUnit *Unit_Get__window(s32 unitId) __asm__("Unit_Get");
#define Unit_Get Unit_Get__window

#if LANGUAGE != LANGUAGE_JAPANESE

u32 Message_AppendArticleName(s32 mode, u16 *name, u32 writePosition,
                              u16 *entries, s32 articleForm, s32 plural,
                              s32 *suffix);

s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 code = 0;
#if !TLA_PATCH_2
  s32 lineStart = 1;
#endif
  s32 quote = 0;
  u32 start = work->count;
  u32 decodedCount = 0;
#if TLA_PATCH_2
  s32 afterItem;
#endif
  s32 plural = 0;
  s32 suffix = SUFFIX_START;
  s32 running = 1;
  u32 writePosition = start;
  u32 previousCode;
  u16 *entry = work->entries;
  s32 overflow = 0;
#if TLA_PATCH_2
  s32 form;
  s8 *word;
  s8 c;
#endif
  s32 (*decode)(struct TextReader *);
  u32 nextPosition;
  s32 no;
  u32 j;
  u32 i;
  u32 value;
  s32 argument;
  s32 magnitude;
  u32 length;
  struct BattleUnit *unit;
  u8 *numberText;
  u8 *nameBytes;
  u16 *nameOutput;
  u16 *p;
  u16 name[24];
  u8 numberBuffer[16];
  struct TextReader reader;
#if TLA_PATCH_2 && LANGUAGE != LANGUAGE_FRENCH
  struct PossessiveTable possessives;
#endif
  struct TextReader *messageReader;
  struct HeapSlots *slots;

#if TLA_PATCH_2 && !TLA_PATCH_3
  // DEAD: German alone clears afterItem here; the first code always clears
  // it before it is read.
  afterItem = 0;
#endif
  if (script == -1) {
    start = work->unknownAfterCount;
  } else {
    RamCode_LoadSlot(TEXT_DECODER_SLOT, Text_DecodeSymbolCode,
                     Text_DecodeSymbolCodeSize);
    slots = Ram_HeapSlots;
    decode = (s32 (*)(struct TextReader *))slots->unkb8[4];
    messageReader = &reader;
    Message_Find(messageReader, script);

    do {
      previousCode = code;
      code = (u32)decode(messageReader);
#if TLA_PATCH_2
      // A full stop right after an item name that ends in one is skipped.
      if (previousCode == 20 && afterItem == 1) {
        if (code == '.')
          code = (u32)decode(messageReader);
        afterItem = 0;
      } else {
        afterItem = 0;
      }
#endif
      if (code > 255)
        code = 64;

      if (overflow != 0) {
        if (code < 32 || code == 176) {
          switch (code) {
          case 19:
            decode(messageReader);
            MessageArgument_Lookup(3, clear);
            break;
          case 0:
          case 2:
          case 30:
            running = 0;
            break;
          case 22:
            MessageArgument_Lookup(5, clear);
            break;
          case 20:
            decode(messageReader);
            MessageArgument_Lookup(2, clear);
            break;
          case 21:
            MessageArgument_Lookup(4, clear);
            break;
          case 23:
            MessageArgument_Lookup(6, clear);
            break;
          case 8:
          case 9:
            decode(messageReader);
            break;
          case 17:
            decode(messageReader);
            break;
          case 18:
            decode(messageReader);
            break;
          case 29:
            decode(messageReader);
            break;
          case 1:
            running = 0;
            code = 2;
            break;
          case 16:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
#if TLA_PATCH_2
        if (work->unk138a[0] != 0) {
          entry[writePosition] = 5;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }
#else
        if (work->unk138a[0] != 0 && lineStart == 0 && code != 222 &&
            code != 223) {
          entry[writePosition] = 5;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }

        if (work->unk138a[1] != 0 && lineStart == 0 && code != 222 &&
            code != 223 && previousCode <= 256 && previousCode > 127 &&
            previousCode != 222 && previousCode != 223 && previousCode != 32 &&
            previousCode != 165 && previousCode != 161 && previousCode != 164) {
          entry[writePosition] = 222;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }
#endif

        if (code > 31 && code != 176) {
          if (work->unk138a[0] != 0 && (code == 32 || decodedCount > 10)) {
            entry[writePosition] = '.';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = '.';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = '.';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            overflow = 1;
            if (decodedCount > 10)
              code = 32;
          }
          if (code == 34) {
            quote ^= 1;
            if (quote != 0)
              code = 142;
          }
          entry[writePosition] = (u16)code;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#if TLA_PATCH_2
          // suffix: 2 after a sentence ends, 1 after an s or z.
#if LANGUAGE == LANGUAGE_SPANISH
          if (code != ' ' && code != 161 && code != 191) {
#else
          if (code != ' ') {
#endif
            suffix = 0;
            if (code == '.' || code == '!' || code == '?')
              suffix = 2;
            if (code == 's' || code == 'S' || code == 'z' || code == 'Z')
              suffix = 1;
          }
#else
          lineStart = 0;
#endif
        } else {
          switch (code) {
          case 0:
          case 2:
          case 30:
            running = 0;
            break;
          case 8:
          case 9:
          case 29:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = decode(messageReader) + 0xFFFFU;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          case 22:
            value = MessageArgument_Lookup(5, clear);
            magnitude = value;
            if (magnitude < 0)
              magnitude = -value;
            plural = 1;
            if (magnitude <= 1)
              plural = 0;
            numberText = String_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != 16 && numberBuffer[i] != 0) {
              entry[writePosition] = numberBuffer[i];
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              i++;
            }
            break;
          case 19:
            no = decode(messageReader) - 1;
            argument = MessageArgument_Lookup(3, clear);
            Message_DecodeRaw(argument + (s32)MsgClassName, name, 24);
            writePosition = Message_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case 20:
            no = decode(messageReader) - 1;
#if TLA_PATCH_2
            {
              s32 lastChar;
              u16 k;

#if LANGUAGE == LANGUAGE_SPANISH
              form = no & 7;
#else
              form = no;
#endif
              argument = MessageArgument_Lookup(2, clear) & ITEM_ID_MASK;
              if (form == 2 || (form == 3 && plural != 0) || form == 4)
                Message_DecodeRaw(
                    argument + (s32)(MsgItemName + ITEM_PLURAL_NAME_OFFSET),
                    name, 24);
              else
                Message_DecodeRaw(argument + (s32)MsgItemName, name, 24);
              writePosition = Message_AppendArticleName(
                  0, name, writePosition, entry, no, plural, &suffix);
              for (k = 0; name[k] != 0; k++)
                lastChar = (s16)name[k];
              if ((u16)lastChar == '.')
                afterItem = 1;
              else
                afterItem = 0;
            }
#else
            argument = MessageArgument_Lookup(2, clear);
            Message_DecodeRaw((argument & ITEM_ID_MASK) + (s32)MsgItemName,
                              name, 24);
            writePosition = Message_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
#endif
            break;
          case 21:
            argument = MessageArgument_Lookup(4, clear);
            Message_DecodeRaw(argument + (s32)MsgAbilityNothing, name, 24);
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & TEXT_BUFFER_MASK;
            }
            writePosition = nextPosition;
            break;
          case 23:
            Message_DecodeRaw(
                Area_GetNameIndexFar(MessageArgument_Lookup(6, clear), 1) +
                    (s32)MsgDebugAreaName,
                name, 24);
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & TEXT_BUFFER_MASK;
            }
            writePosition = nextPosition;
            break;
          case 16:
            nameBytes = Unit_Get(gMainRoutineWork.currentOwner)->name;
            nameOutput = name;
            i = 0;
            do {
              *nameOutput = *nameBytes;
              i++;
              nameBytes++;
              nameOutput++;
            } while (i <= 14);
            writePosition = Message_AppendArticleName(0, name, writePosition,
                                                      entry, 0, 0, &suffix);
            break;
          case 18:
            no = decode(messageReader) - 1;
            nameBytes = Unit_Get(MessageArgument_Lookup(1, clear))->name;
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = Message_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case 17:
            argument = decode(messageReader) - 1;
            nameBytes = Unit_Get(argument)->name;
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = Message_AppendArticleName(0, name, writePosition,
                                                      entry, 0, 0, &suffix);
            break;
          case 28:
            argument = decode(messageReader) - 1;
            unit = Unit_Get(argument);
            length = decode(messageReader);
            nameBytes = unit->name;
            nameOutput = name;
#if TLA_PATCH_2
            // value counts the bytes here.
            for (value = 0; value < length; value++)
              *nameOutput++ = *nameBytes++;
#else
            for (j = 0; j < length; j++) {
              *nameOutput++ = *nameBytes++;
              if ((u8)(*nameBytes + 34) <= 1)
                *nameOutput++ = *nameBytes++;
            }
#endif
            *nameOutput = 0;
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & TEXT_BUFFER_MASK;
            }
            writePosition = nextPosition;
            break;
          case 26:
            argument = ((decode(messageReader) - 1) << 1) + 128;
            entry[writePosition] = argument;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = argument + 1;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
#if TLA_PATCH_2
          case 176:
            entry[writePosition] = ' ';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = '-';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = ' ';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
#if LANGUAGE == LANGUAGE_GERMAN
          case 13:
            // A counted noun, singular or plural, by its kind.
            no = decode(messageReader) - 1;
            for (nameBytes = (u8 *)gText_NumberWords[(no & 7) * 2 + plural];
                 *nameBytes != 0; nameBytes++) {
              entry[writePosition] = *nameBytes;
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            break;
#endif
#if LANGUAGE == LANGUAGE_ITALIAN
          case 13:
            // A gender ending, singular or plural: o or i after operand 1, else
            // a or e.
            if (decode(messageReader) == 1) {
              if (plural == 0)
                entry[writePosition] = 'o';
              else
                entry[writePosition] = 'i';
            } else {
              if (plural == 0)
                entry[writePosition] = 'a';
              else
                entry[writePosition] = 'e';
            }
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
          case 13:
            if (plural != 0) {
              if (suffix != 0) {
                entry[writePosition] = 'e';
                writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              }
              entry[writePosition] = 's';
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            break;
#endif
          case 25:
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
            // The item's possessive, by its gender; c holds the gender
            // first.
            argument = MessageArgument_Lookup(2, 0);
            argument &= ITEM_ID_MASK;
            possessives = gLocalizedWord_ItemPossessives;
            c = gLocalizedWord_ItemGenders[argument];
            word = possessives.text[c & 3];
#else
            // DEAD: the item argument is read and dropped.
            MessageArgument_Lookup(2, 0);
#if LANGUAGE == LANGUAGE_FRENCH
            word = (s8 *)gLocalizedWord_ItemPronoun;
#else
            possessives = gLocalizedWord_ItemPossessives;
            word = possessives.text[plural];
#endif
#endif
            // The word's first letter is a capital at a sentence's start;
            // value counts the letters here.
            for (value = 0; (c = *word++) != 0;) {
              if (value == 0 && suffix == 2)
                c -= 32;
              entry[writePosition] = c;
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              value++;
              if (value > 15)
                break;
            }
#if LANGUAGE != LANGUAGE_SPANISH
            suffix = 0;
#endif
            break;
          case 27:
#if LANGUAGE == LANGUAGE_GERMAN
            // An apostrophe after s or z, else an s.
            if (suffix == 1)
              entry[writePosition] = '\'';
            else
              entry[writePosition] = 's';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#endif
            break;
          case -1:
            break;
          case 1:
          default:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
#else
          case 176:
            entry[writePosition] = 143;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = '-';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          case 25:
            if (plural != 0) {
              if (suffix != 0) {
                entry[writePosition] = 'e';
                writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              }
              entry[writePosition] = 's';
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            break;
          case 27:
            entry[writePosition] = '\'';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            if (suffix == 0) {
              entry[writePosition] = 's';
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            break;
          case -1:
            break;
          case 1:
          case 3:
            lineStart = 1;
          default:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            if (code == 's' || code == 'S')
              suffix = 1;
            else
              suffix = 0;
            break;
#endif
          }
        }
      }
      decodedCount++;
    } while (running != 0 && decodedCount <= TEXT_BUFFER_MASK);

    entry[writePosition] = (u16)code;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    work->count = (u16)nextPosition;
    HeapSlot_Release(TEXT_DECODER_SLOT);
    work->unknownAfterCount = (u16)start;
  }

  if (clear != 0)
    MessageArgument_ClearAll();

  return start;
}

#endif

#if LANGUAGE == LANGUAGE_JAPANESE

/// The window work's menuState while BattleMessage_Open decodes a message (The
/// Broken Seal's WINDOW_BATTLE_MESSAGE).
#define WINDOW_BATTLE_MESSAGE 2

// An enemy's id is 8 above its name's place after MsgEnemyName.
/// ★ Named from the game: "Mystery Man" (MsgEnemyName + 344)
#define ENEMY_MYSTERY_MAN 352
/// ★ Named from the game: "Mystery Woman" (MsgEnemyName + 347)
#define ENEMY_MYSTERY_WOMAN 355

extern u8 MsgWaitingForOpponent[];

// なぞの男 ("Mystery Man"), the message after MsgWaitingForOpponent; なぞの女
// ("Mystery Woman") follows it.
#define MSG_MYSTERY_MAN ((s32)MsgWaitingForOpponent + 1)

/// Copies a name into the decoded text at writePosition and returns the
/// position after it. With mode set, a space and codes 10, 10 go before the
/// name and codes 10, 8 and a space after it (The Broken Seal reads code 10 as
/// line spacing, with its operand). The other editions' routine of this name,
/// at the top of this file, also writes articles; Japanese has none, and its
/// decoder inlines this copy.
static inline u32 Message_AppendArticleName(s32 mode, u16 *name,
                                            u32 writePosition, u16 *entries,
                                            s32 articleForm, s32 plural,
                                            s32 *suffix) {
  u16 c;

  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  while (*name != 0) {
    c = *name++;
    entries[writePosition] = c;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  if (mode != 0) {
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 8;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  return writePosition;
}

/// The Japanese Message_Decode: no articles or plurals, and a battle message
/// (WINDOW_BATTLE_MESSAGE) sets its names off and doubles each space. While
/// menuState is set, the Mystery Man and Mystery Woman are named from
/// MSG_MYSTERY_MAN and the message after it rather than their unit names.
s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 code = 0;
  s32 lineStart = 1;
  u32 start = work->count;
  u32 decodedCount = 0;
  s32 running = 1;
  u32 writePosition = start;
  u16 *entry = work->entries;
  u8 mode = work->menuState & WINDOW_BATTLE_MESSAGE;
  s32 (*decode)(struct TextReader *);
  s32 overflow = 0;
  u32 previousCode;
  u32 nextPosition;
  u32 i;
  u32 j;
  s32 argument;
  s32 message;
  s32 value;
  u32 length;
  struct BattleUnit *unit;
  u8 *numberText;
  u8 *nameBytes;
  u16 *nameOutput;
  u16 name[24];
  u8 numberBuffer[16];
  struct TextReader reader;
  struct TextReader *messageReader;
  struct HeapSlots *slots;

  if (script == -1) {
    start = work->unknownAfterCount;
  } else {
    RamCode_LoadSlot(TEXT_DECODER_SLOT, Text_DecodeSymbolCode,
                     Text_DecodeSymbolCodeSize);
    slots = Ram_HeapSlots;
    decode = (s32 (*)(struct TextReader *))slots->unkb8[4];
    messageReader = &reader;
    Message_Find(messageReader, script);

    do {
      previousCode = code;
      code = (u32)decode(messageReader);

      if (overflow != 0) {
        if (code < 32) {
          switch (code) {
          case 19:
            MessageArgument_Lookup(3, clear);
            break;
          case 0:
          case 2:
          case 30:
            running = 0;
            break;
          case 8:
          case 9:
            decode(messageReader);
            break;
          case 22:
            MessageArgument_Lookup(5, clear);
            break;
          case 20:
            MessageArgument_Lookup(2, clear);
            break;
          case 21:
            MessageArgument_Lookup(4, clear);
            break;
          case 23:
            MessageArgument_Lookup(6, clear);
            break;
          case 17:
            decode(messageReader);
            break;
          case 18:
            break;
          case 1:
            running = 0;
            code = 2;
            break;
          case 16:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        if (work->unk138a[0] != 0 && lineStart == 0 && code != 222 &&
            code != 223) {
          entry[writePosition] = 5;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }

        if (work->unk138a[1] != 0 && lineStart == 0 && code != 222 &&
            code != 223 && previousCode <= 256 && previousCode > 127 &&
            previousCode != 222 && previousCode != 223 && previousCode != 32 &&
            previousCode != 165 && previousCode != 161 && previousCode != 164) {
          entry[writePosition] = 222;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }

        if (code > 31) {
          if (mode != 0 && code == ' ') {
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
          }
          // The ellipsis here is three of code 165, then code 161.
          if (work->unk138a[0] != 0 && (code == 32 || decodedCount > 10)) {
            entry[writePosition] = 165;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = 165;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = 165;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = 161;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            overflow = 1;
            if (decodedCount > 10)
              code = 32;
          }
          entry[writePosition] = (u16)code;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
          lineStart = 0;
        } else {
          switch (code) {
          case 0:
          case 2:
          case 30:
            running = 0;
            break;
          case 8:
          case 9:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = decode(messageReader) + 0xFFFFU;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          case 22:
            value = MessageArgument_Lookup(5, clear);
            numberText = String_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != 16 && numberBuffer[i] != 0) {
              entry[writePosition] = numberBuffer[i];
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              i++;
            }
            break;
          case 19:
            argument = MessageArgument_Lookup(3, clear);
            Message_DecodeRaw(argument + (s32)MsgClassName, name, 24);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case 20:
            argument = MessageArgument_Lookup(2, clear);
            Message_DecodeRaw((argument & ITEM_ID_MASK) + (s32)MsgItemName,
                              name, 24);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case 21:
            argument = MessageArgument_Lookup(4, clear);
            Message_DecodeRaw(argument + (s32)MsgAbilityNothing, name, 24);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case 23:
            Message_DecodeRaw(
                Area_GetNameIndexFar(MessageArgument_Lookup(6, clear), 1) +
                    (s32)MsgDebugAreaName,
                name, 24);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case 16:
            nameBytes = Unit_Get(gMainRoutineWork.currentOwner)->name;
            nameOutput = name;
            i = 0;
            do {
              *nameOutput = *nameBytes;
              i++;
              nameBytes++;
              nameOutput++;
            } while (i <= 14);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case 18:
            unit = Unit_Get(MessageArgument_Lookup(1, clear));
            if (work->menuState != 0 &&
                (unit->enemyId == ENEMY_MYSTERY_MAN ||
                 unit->enemyId == ENEMY_MYSTERY_WOMAN)) {
              message = MSG_MYSTERY_MAN;
              if (unit->enemyId == ENEMY_MYSTERY_WOMAN)
                message++;
              Message_DecodeRaw(message, name, 10);
              writePosition = Message_AppendArticleName(
                  mode, name, writePosition, entry, 0, 0, 0);
            } else {
              nameBytes = unit->name;
              nameOutput = name;
              j = 0;
              do {
                *nameOutput = *nameBytes;
                j++;
                nameBytes++;
                nameOutput++;
              } while (j <= 14);
              writePosition = Message_AppendArticleName(
                  mode, name, writePosition, entry, 0, 0, 0);
            }
            break;
          case 17:
            argument = decode(messageReader) - 1;
            nameBytes = Unit_Get(argument)->name;
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case 28:
            argument = decode(messageReader) - 1;
            unit = Unit_Get(argument);
            length = decode(messageReader);
            nameBytes = unit->name;
            nameOutput = name;
            for (j = 0; j < length; j++) {
              *nameOutput++ = *nameBytes++;
              if ((u8)(*nameBytes + 34) <= 1)
                *nameOutput++ = *nameBytes++;
            }
            *nameOutput = 0;
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case -1:
            break;
          case 1:
          case 3:
            lineStart = 1;
          default:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          }
        }
      }
      decodedCount++;
    } while (running != 0 && decodedCount <= TEXT_BUFFER_MASK);

    entry[writePosition] = (u16)code;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    work->count = (u16)nextPosition;
    HeapSlot_Release(TEXT_DECODER_SLOT);
    work->unknownAfterCount = (u16)start;
  }

  if (clear != 0)
    MessageArgument_ClearAll();

  return start;
}

#endif

#undef ARTICLE_KINDS
#undef ARTICLE_KIND_MASK
#undef TEXT_DECODER_SLOT
#undef TEXT_BUFFER_MASK
#undef ITEM_PLURAL_NAME_OFFSET
#undef SUFFIX_START
#undef POSSESSIVE_COUNT
#undef WINDOW_BATTLE_MESSAGE
#undef ENEMY_MYSTERY_MAN
#undef ENEMY_MYSTERY_WOMAN
#undef MSG_MYSTERY_MAN
