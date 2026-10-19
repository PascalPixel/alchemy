/// @file
/// The message decoder: articles, names and numbers written out.
/// Part of window.c, the window object, compiled only through it.

#include "../window_internal.h"

#if (LANGUAGE != LANGUAGE_JAPANESE)
#if (LANGUAGE == LANGUAGE_ITALIAN)
#define ARTICLE_KINDS 11
#define ARTICLE_KIND_MASK 15
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define ARTICLE_KINDS 5
#define ARTICLE_KIND_MASK 15
#else
#define ARTICLE_KINDS 8
#define ARTICLE_KIND_MASK 7
#endif

/// The article written before a name of each article class.
struct ArticleTable {
  s8 *text[ARTICLE_KINDS];
};

#endif

// UiText_AppendArticleName copies a name into the decoded text at pos and
// returns the position after it. A name may start with an article class
// ({article_class:N}, MSG_ARTICLE_CLASS and its class), which picks the article
// written before it. no is the article form the message asks for (its
// argument code's operand); plural says the last number shown was not 1;
// suffix tracks what the text last ended with, for the suffix codes (1: an s,
// or in German s or z; 2: the end of a sentence, so the next word is
// capitalised; 0: anything else). With mode set (battle messages) the name is
// set off with spaces and the style codes around it.

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
/// The German, Spanish, French and Italian form: forms 0 and 4 take no article,
/// the others the class's article (in Spanish, form bit 3 asks for the
/// indefinite article), capitalised at the start of a sentence.
/// The write position wraps in the decoded-text ring; language grammar remains
/// separate.
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 writePosition,
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
  u16 code;
  s8 *p;
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
    table = gUiText_ArticleTable;
#if (LANGUAGE == LANGUAGE_SPANISH)
    indefiniteTable = gUiText_IndefiniteArticles;
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
        c8 -= 0x20;
      entries[writePosition] = c8;
      writePosition = (writePosition + 1) & 0x1ff;
    }
  }
  while (*name != 0) {
    code = *name++;
    entries[writePosition] = code;
    writePosition = (writePosition + 1) & 0x1ff;
#if (LANGUAGE == LANGUAGE_GERMAN)
    if (code == 's' || code == 'S' || code == 'z' || code == 'Z')
      *suffix = 1;
    else
      *suffix = 0;
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
    if (code == 's' || code == 'S')
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

#if (LANGUAGE == LANGUAGE_ENGLISH)
/// The English form: form 1 (or 3 with a single item) takes "a" or "an" (by the
/// name's first letter unless its class says otherwise), and form 2 (or 3 with
/// several) adds the plural "s" or "es".
/// The write position wraps in the decoded-text ring; language grammar remains
/// separate.
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 writePosition,
                             u16 *entries, s32 articleForm, s32 plural,
                             s32 *suffix) {
  struct ArticleTable table;
  s32 kind;
  u16 head;
  u16 code;
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
      entries[writePosition] = c8;
      writePosition = (writePosition + 1) & 0x1ff;
    }
  } else {
    head = name[0];
    if (head == 29)
      name += 2;
  }
  while (*name != 0) {
    code = *name++;
    entries[writePosition] = code;
    writePosition = (writePosition + 1) & 0x1ff;
    if (code == 'S' || code == 's')
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
#endif

/// The control codes of a message as stored (the PO catalogs' names), which
/// UiText_BuildRenderEntries expands into decoded text.
enum {
  MSG_END = 0,
  MSG_PAGE_BREAK = 1,
  MSG_END_WAIT = 2,
  MSG_LINE_BREAK = 3,
  MSG_TEXT_COLOR = 8,
  MSG_TEXT_EFFECT = 9,
  /// German: a counted noun; Italian: a gender ending; Spanish and French: the
  /// plural suffix
  MSG_LOCALIZED_WORD = 13,
  MSG_PROTAGONIST_NAME = 16,
  MSG_ACTOR_NAME = 17,
  MSG_ARGUMENT_ACTOR_NAME = 18,
  MSG_ARGUMENT_CLASS_NAME = 19,
  MSG_ARGUMENT_ITEM_NAME = 20,
  MSG_ARGUMENT_ABILITY_NAME = 21,
  MSG_ARGUMENT_SIGNED_NUMBER = 22,
  MSG_ARGUMENT_LOCATION_NAME = 23,
  MSG_EM_DASH = 24,
  /// English: the plural suffix; German, Spanish, French and Italian: the
  /// item's possessive
  MSG_PLURAL_SUFFIX = 25,
  MSG_BUTTON_ICON = 26,
  MSG_POSSESSIVE_SUFFIX = 27,
  MSG_ARTICLE_CLASS = 29,
  MSG_END_NOW = 30,
};

/// What halting speech (UiWork_SetBusyFlags) puts between characters.
#define ENTRY_NARROW_GAP TEXT_PAUSE_SHORT
#define ENTRY_WIDE_GAP TEXT_VOICED_MARK

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#if (LANGUAGE == LANGUAGE_FRENCH)
extern s8 gUiText_ItemPronoun[];
#else
/// The possessive written before an item's name, chosen by the item's gender
/// or by number.
struct ItemPossessiveTable {
  s8 *text[ITEM_POSSESSIVE_TEXT_COUNT];
};
extern const struct ItemPossessiveTable gUiText_ItemPossessives;
extern const s8 gUiText_ItemGenders[];
extern u8 *gUiText_NumberWords[];
#endif
#endif

/// Decodes message script into the ring of decoded text (UiRenderWork.entries)
/// and returns where it starts; script -1 returns the last message's start
/// again. Argument codes take their values from the argument table
/// (UiWork_PushValueSlot), and clear empties the table afterwards. Names,
/// numbers and articles are written out; style codes are kept with their
/// operands for the message channel to run.
/// script -1 reuses messageStart; clear consumes argument slots after decoding.
s32 UiText_BuildRenderEntries(s32 script, s32 clear) {
  struct UiRenderWork *work = UI_WORK;
  u32 code = 0;
#if (LANGUAGE == LANGUAGE_ENGLISH)
  s32 lineStart = 1;
#endif
  s32 quote = 0;
  u32 start = work->count;
  u32 decodedCount = 0;
  s32 plural = 0;
#if !(LANGUAGE == LANGUAGE_GERMAN) && !(LANGUAGE == LANGUAGE_ITALIAN)
  s32 running = 1;
#endif
#if (LANGUAGE == LANGUAGE_ENGLISH)
  u32 writePosition = start;
  u32 previousCode;
  s32 suffix = 0;
#else
#if (LANGUAGE == LANGUAGE_SPANISH)
  struct ItemPossessiveTable possessives;
  s32 form;
#endif
  s32 suffix = 2;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_ITALIAN)
  s32 running = 1;
#endif
  s8 *word;
  s8 c8;
  u32 writePosition = start;
#endif
  u16 *entry = work->entries;
  s32 overflow = 0;
  s32 (*decode)(struct TextReader *);
  u32 nextPosition;
  s32 no;
  u32 j;
  u32 i;
  s32 value;
  s32 argument;
#if (LANGUAGE == LANGUAGE_ITALIAN)
  s32 ending;
#endif
#ifdef UBFIX
  u32 magnitude;
#else
  s32 magnitude;
#endif
  u8 *numberText;
  u8 *nameBytes;
  u16 *nameOutput;
  u16 *p;
  u16 name[UI_MESSAGE_NAME_HALFWORDS];
  u8 numberBuffer[UI_NUMBER_BUFFER_SIZE];
  struct TextReader reader;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_ITALIAN)
  struct ItemPossessiveTable possessives;
  s32 kind;
#endif

  if (script == -1) {
    start = work->messageStart;
  } else {
    {
      u32 size;

      /* FAKEMATCH(one-pass): the size set in a block of its own, so the slot
       * number goes to r0 before the size's copy to r1; as a plain
       * assignment, 22 of 706 instructions differ in tbs-en. */
      do {
        size = (u32)Text_DecodeSymbolCodeSize;
      } while (0);
      Dma_Set((const void *)Text_DecodeSymbol,
              Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, size),
              DMA_COPY32(size >> 2), REG_DMA3);
    }
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(
        struct TextReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
    UiText_LookupMessage(&reader, script);

    do {
#if (LANGUAGE == LANGUAGE_ENGLISH)
      previousCode = code;
#endif
      code = (u32)decode(&reader);
      if (code > 255)
        code = 64;

      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (code < 32) {
          switch (code) {
          case MSG_ARGUMENT_CLASS_NAME:
            decode(&reader);
            UiText_LookupArgument(MSG_ARG_CLASS, clear);
            break;
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            decode(&reader);
            UiText_LookupArgument(MSG_ARG_ITEM, clear);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_LookupArgument(MSG_ARG_LOCATION, clear);
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
            decode(&reader);
            break;
          case MSG_ACTOR_NAME:
            decode(&reader);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            decode(&reader);
            break;
          case MSG_ARTICLE_CLASS:
            decode(&reader);
            break;
          case MSG_PAGE_BREAK:
            running = 0;
            code = 2;
            break;
          case MSG_PROTAGONIST_NAME:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        // Halting speech pauses before each character.
        if (work->spacingBefore != 0
#if (LANGUAGE == LANGUAGE_ENGLISH)
            && lineStart == 0 && code != 222 && code != 223
#endif
        ) {
          entry[writePosition] = ENTRY_NARROW_GAP;
          writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
        }

#if (LANGUAGE == LANGUAGE_ENGLISH)
        if (work->spacingAfter != 0 && lineStart == 0 && code != 222 &&
            code != 223 && previousCode <= 256 && previousCode > 127 &&
            previousCode != 222 && previousCode != 223 && previousCode != 32 &&
            previousCode != 165 && previousCode != 161 && previousCode != 164) {
          entry[writePosition] = ENTRY_WIDE_GAP;
          writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
        }

#endif
        // A character. Halting speech breaks off with an ellipsis at the
        // first space or after 11 characters.
        if (code > 31) {
          if (work->spacingBefore != 0 && (code == 32 || decodedCount > 10)) {
            entry[writePosition] = '.';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = '.';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = '.';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
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
          writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
#if (LANGUAGE == LANGUAGE_ENGLISH)
          lineStart = 0;
#else
          if (code != ' '
#if (LANGUAGE == LANGUAGE_SPANISH)
              && code != 161 && code != 191
#endif
          ) {
            suffix = 0;
            if (code == '.' || code == '!' || code == '?')
              suffix = 2;
            if (code == 's' || code == 'S'
#if (LANGUAGE == LANGUAGE_GERMAN)
                || code == 'z' || code == 'Z'
#endif
            )
              suffix = 1;
          }
#endif
        } else {
          // A control code: write out names, numbers and articles, and keep
          // style codes with their operands.
          switch (code) {
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
          case MSG_ARTICLE_CLASS:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = decode(&reader) + 0xFFFFU;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            value = (s32)UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            magnitude = value;
            if (value < 0)
#ifdef UBFIX
              // Signed -value overflows for INT_MIN; its unsigned magnitude
              // still gives the correct singular/plural comparison.
              magnitude = 0U - (u32)value;
#else
              magnitude = -value;
#endif
            plural = 1;
            if (magnitude <= 1)
              plural = 0;
            numberText = UiText_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != 16 && numberBuffer[i] != 0) {
              entry[writePosition] = numberBuffer[i];
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
              i++;
            }
            break;
          case MSG_ARGUMENT_CLASS_NAME:
            no = decode(&reader) - 1;
            argument = UiText_LookupArgument(MSG_ARG_CLASS, clear);
            UiText_DecodeMessage(argument + MsgClassName, name,
                                 UI_MESSAGE_NAME_HALFWORDS);
            writePosition = UiText_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            no = decode(&reader) - 1;
#if (LANGUAGE == LANGUAGE_SPANISH)
            form = no & 7;
#endif
            argument = UiText_LookupArgument(MSG_ARG_ITEM, clear);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
            argument &= RENDER_ENTRY_MASK;
#if (LANGUAGE == LANGUAGE_SPANISH)
            if (form == 2 || (form == 3 && plural != 0) || form == 4)
#else
            if (no == 2 || (no == 3 && plural != 0) || no == 4)
#endif
              UiText_DecodeMessage(argument + MsgItemPluralName, name,
                                   UI_MESSAGE_NAME_HALFWORDS);
            else
              UiText_DecodeMessage(argument + MsgItemName, name,
                                   UI_MESSAGE_NAME_HALFWORDS);
#else
            UiText_DecodeMessage((argument & RENDER_ENTRY_MASK) + MsgItemName,
                                 name, 24);
#endif
            writePosition = UiText_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            argument = UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            UiText_DecodeMessage(argument + MsgAbilityName, name,
                                 UI_MESSAGE_NAME_HALFWORDS);
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & RENDER_ENTRY_MASK;
            }
            writePosition = nextPosition;
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_DecodeMessage(
                Scene_GetAreaNameIndexFar(
                    UiText_LookupArgument(MSG_ARG_LOCATION, clear), 1) +
                    MsgDebugEntryName,
                name, UI_MESSAGE_NAME_HALFWORDS);
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & RENDER_ENTRY_MASK;
            }
            writePosition = nextPosition;
            break;
          case MSG_PROTAGONIST_NAME:
            nameBytes = UnitGetFar(gGameState.selectedActor)->name;
            nameOutput = name;
            i = 0;
            do {
              *nameOutput = *nameBytes;
              i++;
              nameBytes++;
              nameOutput++;
            } while (i <= 14);
            writePosition = UiText_AppendArticleName(0, name, writePosition,
                                                     entry, 0, 0, &suffix);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            no = decode(&reader) - 1;
            nameBytes =
                UnitGetFar(UiText_LookupArgument(MSG_ARG_ACTOR, clear))->name;
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = UiText_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case MSG_ACTOR_NAME:
            argument = decode(&reader) - 1;
            nameBytes = UnitGetFar(argument)->name;
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = UiText_AppendArticleName(0, name, writePosition,
                                                     entry, 0, 0, &suffix);
            break;
          case MSG_BUTTON_ICON:
            argument = ((decode(&reader) - 1) << 1) + 128;
            entry[writePosition] = argument;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = argument + 1;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            break;
          case MSG_EM_DASH:
            entry[writePosition] = TEXT_ARGUMENT_SPACE;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = '-';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
            entry[writePosition] = ' ';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
#endif
            break;
#if (LANGUAGE == LANGUAGE_GERMAN)
          case MSG_LOCALIZED_WORD:
            no = decode(&reader) - 1;
            nameBytes = gUiText_NumberWords[(no & 7) * 2 + plural];
            while (*nameBytes != 0) {
              entry[writePosition] = *nameBytes++;
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            }
            break;
#endif
#if (LANGUAGE == LANGUAGE_ITALIAN)
          case MSG_LOCALIZED_WORD:
            ending = decode(&reader) - 1;
            if (ending == 0) {
              if (plural == 0)
                entry[writePosition] = 'o';
              else
                entry[writePosition] = 'i';
            } else if (plural == 0)
              entry[writePosition] = 'a';
            else
              entry[writePosition] = 'e';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            break;
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
          case MSG_LOCALIZED_WORD:
#elif (LANGUAGE == LANGUAGE_ENGLISH)
          case MSG_PLURAL_SUFFIX:
#endif
#if !(LANGUAGE == LANGUAGE_GERMAN) && !(LANGUAGE == LANGUAGE_ITALIAN)
            if (plural != 0) {
              if (suffix != 0) {
                entry[writePosition] = 'e';
                writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
              }
              entry[writePosition] = 's';
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            }
            break;
#endif
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
          case MSG_PLURAL_SUFFIX:
#if (LANGUAGE == LANGUAGE_SPANISH)
            UiText_LookupArgument(MSG_ARG_ITEM, 0);
            possessives = gUiText_ItemPossessives;
            word = possessives.text[plural];
#else
            argument = UiText_LookupArgument(MSG_ARG_ITEM, 0);
            argument &= RENDER_ENTRY_MASK;
            possessives = gUiText_ItemPossessives;
            kind = gUiText_ItemGenders[argument];
            word = possessives.text[kind & 3];
#endif
            /* FAKEMATCH: value counts the letters, compared unsigned, keeping
             * the allocation of Spanish and Italian. With the function's u32 j
             * or i as the counter: differ 44 or 58 in tbs-es, 476 or 486 in
             * tbs-it. */
            for (value = 0; (u32)value < 16; value++) {
              if ((c8 = *word++) == 0)
                break;
              if (value == 0 && suffix == 2)
                c8 -= 0x20;
              entry[writePosition] = c8;
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            }
            break;
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
          case MSG_PLURAL_SUFFIX:
#if (LANGUAGE == LANGUAGE_GERMAN)
            argument = UiText_LookupArgument(MSG_ARG_ITEM, 0);
            argument &= RENDER_ENTRY_MASK;
            possessives = gUiText_ItemPossessives;
            kind = gUiText_ItemGenders[argument];
            word = possessives.text[kind & 3];
#else
            UiText_LookupArgument(MSG_ARG_ITEM, 0);
            word = gUiText_ItemPronoun;
#endif
            for (i = 0; i < 16; i++) {
              if ((c8 = *word++) == 0)
                break;
              if (i == 0 && suffix == 2)
                c8 -= 0x20;
              entry[writePosition] = c8;
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            }
            suffix = 0;
            break;
#endif
          case MSG_POSSESSIVE_SUFFIX:
#if (LANGUAGE == LANGUAGE_GERMAN)
            if (suffix == 1)
              entry[writePosition] = '\'';
            else
              entry[writePosition] = 's';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
#elif (LANGUAGE == LANGUAGE_ENGLISH)
            entry[writePosition] = '\'';
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            if (suffix == 0) {
              entry[writePosition] = 's';
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            }
#endif
            break;
          case -1:
            break;
          case MSG_PAGE_BREAK:
          case MSG_LINE_BREAK:
#if (LANGUAGE == LANGUAGE_ENGLISH)
            lineStart = 1;
#endif
            // Fall through: keep the break in the decoded text.
          default:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
#if (LANGUAGE == LANGUAGE_ENGLISH)
            if (code == 's' || code == 'S')
              suffix = 1;
            else
              suffix = 0;
#endif
            break;
          }
        }
      }
      decodedCount++;
    } while (running != 0 && decodedCount <= RENDER_ENTRY_MASK);

    entry[writePosition] = (u16)code;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    work->count = (u16)nextPosition;
    Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
    work->messageStart = (u16)start;
  }

  if (clear != 0)
    UiWork_ClearValueNameTables();

  return start;
}

#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)

/// The Japanese form: Japanese has no articles, so the name is copied as it is.
/// The write position wraps in the decoded-text ring; language grammar remains
/// separate.
extern __inline__ u32 UiText_AppendArticleName(s32 mode, u16 *name,
                                               u32 writePosition, u16 *entries,
                                               s32 articleForm, s32 plural,
                                               s32 *suffix) {
  u16 code;

  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
  }
  while (*name != 0) {
    code = *name++;
    entries[writePosition] = code;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
  }
  if (mode != 0) {
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    entries[writePosition] = 8;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
  }
  return writePosition;
}

/// The Japanese UiText_BuildRenderEntries, as above with no articles or
/// plurals.
/// script -1 reuses messageStart; clear consumes argument slots after decoding.
s32 UiText_BuildRenderEntries(s32 script, s32 clear) {
  struct UiRenderWork *work = UI_WORK;
  u32 code = 0;
  s32 lineStart = 1;
  u32 start = work->count;
  u32 decodedCount = 0;
  s32 running = 1;
  u32 writePosition = start;
  u16 *entry = work->entries;
  u8 mode = work->menuState & 2;
  s32 (*decode)(struct TextReader *);
  s32 overflow = 0;
  u32 previousCode;
  u32 nextPosition;
  u32 i;
  u32 j;
  s32 argument;
  s32 message;
  s32 value;
  u8 *numberText;
  u8 *nameBytes;
  u16 *nameOutput;
  struct BattleUnit *unit;
  u16 name[UI_MESSAGE_NAME_HALFWORDS];
  u8 numberBuffer[UI_NUMBER_BUFFER_SIZE];
  struct TextReader reader;

  if (script == -1) {
    start = work->messageStart;
  } else {
    {
      u32 size;

      /* FAKEMATCH(one-pass): the size set in a block of its own, as in the
       * international copy above; as a plain assignment, 24 of 895
       * instructions differ in tbs-ja. */
      do {
        size = (u32)Text_DecodeSymbolCodeSize;
      } while (0);
      Dma_Set((const void *)Text_DecodeSymbol,
              Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, size),
              DMA_COPY32(size >> 2), REG_DMA3);
    }
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(
        struct TextReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
    UiText_LookupMessage(&reader, script);
    do {
      previousCode = code;
      code = (u32)decode(&reader);
      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (code < 32) {
          switch (code) {
          case MSG_ARGUMENT_CLASS_NAME:
            UiText_LookupArgument(MSG_ARG_CLASS, clear);
            break;
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
            decode(&reader);
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            UiText_LookupArgument(MSG_ARG_ITEM, clear);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_LookupArgument(MSG_ARG_LOCATION, clear);
            break;
          case MSG_ACTOR_NAME:
            decode(&reader);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            break;
          case MSG_PAGE_BREAK:
            running = 0;
            code = 2;
            break;
          case MSG_PROTAGONIST_NAME:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        // Halting speech pauses before each character.
        if (work->spacingBefore != 0 && lineStart == 0 && code != 222 &&
            code != 223) {
          entry[writePosition] = 5;
          writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
        }
        if (work->spacingAfter != 0 && lineStart == 0 && code != 222 &&
            code != 223 && previousCode <= 256 && previousCode > 127 &&
            previousCode != 222 && previousCode != 223 && previousCode != 32 &&
            previousCode != 165 && previousCode != 161 && previousCode != 164) {
          entry[writePosition] = 222;
          writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
        }
        // A character. Halting speech breaks off with an ellipsis at the
        // first space or after 11 characters.
        if (code > 31) {
          if (mode != 0 && code == ' ') {
            entry[writePosition] = code;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
          }
          if (work->spacingBefore != 0 && (code == 32 || decodedCount > 10)) {
            entry[writePosition] = 165;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = 165;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = 165;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = 161;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            overflow = 1;
            if (decodedCount > 10)
              code = 32;
          }
          entry[writePosition] = (u16)code;
          writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
          lineStart = 0;
        } else {
          // A control code: write out names and numbers, and keep style
          // codes with their operands.
          switch (code) {
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            entry[writePosition] = decode(&reader) + 0xffffU;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            value = UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            numberText = UiText_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != 16 && numberBuffer[i] != 0) {
              entry[writePosition] = numberBuffer[i];
              writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
              i++;
            }
            break;
          case MSG_ARGUMENT_CLASS_NAME:
            argument = UiText_LookupArgument(MSG_ARG_CLASS, clear);
            UiText_DecodeMessage(argument + MsgClassName, name,
                                 UI_MESSAGE_NAME_HALFWORDS);
            writePosition = UiText_AppendArticleName(mode, name, writePosition,
                                                     entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            argument = UiText_LookupArgument(MSG_ARG_ITEM, clear);
            UiText_DecodeMessage((argument & 511) + MsgItemName, name,
                                 UI_MESSAGE_NAME_HALFWORDS);
            writePosition = UiText_AppendArticleName(mode, name, writePosition,
                                                     entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            argument = UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            UiText_DecodeMessage(argument + MsgAbilityName, name,
                                 UI_MESSAGE_NAME_HALFWORDS);
            writePosition = UiText_AppendArticleName(mode, name, writePosition,
                                                     entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_DecodeMessage(
                Scene_GetAreaNameIndexFar(
                    UiText_LookupArgument(MSG_ARG_LOCATION, clear), 1) +
                    MsgDebugEntryName,
                name, UI_MESSAGE_NAME_HALFWORDS);
            writePosition = UiText_AppendArticleName(mode, name, writePosition,
                                                     entry, 0, 0, 0);
            break;
          case MSG_PROTAGONIST_NAME:
            nameBytes = UnitGetFar(gGameState.selectedActor)->name;
            nameOutput = name;
            i = 0;
            do {
              *nameOutput = *nameBytes;
              i++;
              nameBytes++;
              nameOutput++;
            } while (i <= 14);
            writePosition = UiText_AppendArticleName(mode, name, writePosition,
                                                     entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            unit = UnitGetFar(UiText_LookupArgument(MSG_ARG_ACTOR, clear));
            if (work->menuState != 0 &&
                (unit->classId == 122 || unit->classId == 125)) {
              message = MsgMysteryPersonName;
              if (unit->classId == 125)
                message++;
              UiText_DecodeMessage(message, name, 10);
              writePosition = UiText_AppendArticleName(
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
              writePosition = UiText_AppendArticleName(
                  mode, name, writePosition, entry, 0, 0, 0);
            }
            break;
          case MSG_ACTOR_NAME:
            argument = decode(&reader) - 1;
            nameBytes = UnitGetFar(argument)->name;
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = UiText_AppendArticleName(mode, name, writePosition,
                                                     entry, 0, 0, 0);
            break;
          case -1:
            break;
          case MSG_PAGE_BREAK:
          case MSG_LINE_BREAK:
            lineStart = 1;
          default:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
            break;
          }
        }
      }
      decodedCount++;
    } while (running != 0 && decodedCount <= RENDER_ENTRY_MASK);
    entry[writePosition] = (u16)code;
    writePosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & RENDER_ENTRY_MASK;
    work->count = (u16)nextPosition;
    Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
    work->messageStart = (u16)start;
  }
  if (clear != 0)
    UiWork_ClearValueNameTables();
  return start;
}

#endif
