/// @file
/// The message decoder: articles, names and numbers written out.
// Part of window.c.

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

// Message_AppendArticleName copies a name into the decoded text at pos and
// returns the position after it. A name may start with an article class
// ({article_class:N}, MESSAGE_ARTICLE_CLASS and its class), which picks the
// article written before it. no is the article form the message asks for (its
// argument code's operand); plural says the last number shown was not 1;
// suffix tracks what the text last ended with, for the suffix codes (1: an s,
// or in German s or z; 2: the end of a sentence, so the next word is
// capitalised; 0: anything else). With mode set (battle messages) the name is
// set off with spaces and style codes: a space and line spacing 10 before it,
// line spacing 8 and a space after. Only the Japanese decoder sets mode, from
// WindowWork.battleMode's WINDOW_BATTLE_MESSAGE, which BattleMessage_Open
// sets while it decodes; the others always pass 0. The write position wraps at
// TEXT_BUFFER_MASK, the ring's mask.

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
/// The German, Spanish, French and Italian form: forms 0 and 4 take no article,
/// the others the class's article (in Spanish, form bit 3 asks for the
/// indefinite article), capitalised at the start of a sentence.
/// The write position wraps in the text buffer; language grammar remains
/// separate.
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
  u16 code;
  s8 *p;
  s8 letter;
  s32 cnt;

#if (LANGUAGE == LANGUAGE_SPANISH)
  count = articleForm & 7;
  indefinite = articleForm & 8;
#endif
  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = TEXT_LINE_SPACING;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  // A name's leading article class (MESSAGE_ARTICLE_CLASS and its
  // operand) is never copied.
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
    indefiniteTable = gArticle_Indefinite;
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
    // At most 8 letters, the first capitalised at the start of a
    // sentence.
    for (cnt = 0; cnt < 8; cnt++) {
      if ((letter = *p++) == 0)
        break;
      if (cnt == 0 && *suffix == 2)
        letter -= 'a' - 'A';
      entries[writePosition] = letter;
      writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    }
  }
  while (*name != 0) {
    code = *name++;
    entries[writePosition] = code;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
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
    entries[writePosition] = TEXT_LINE_SPACING;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 8;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  return writePosition;
}
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH)
/// The English form: form 1 (or 3 with a single item) takes "a" or "an" (by the
/// name's first letter unless its class says otherwise), and form 2 (or 3 with
/// several) adds the plural "s" or "es".
/// The write position wraps in the text buffer; language grammar remains
/// separate.
u32 Message_AppendArticleName(s32 mode, u16 *name, u32 writePosition,
                              u16 *entries, s32 articleForm, s32 plural,
                              s32 *suffix) {
  struct ArticleTable table;
  s32 kind;
  u16 head;
  u16 code;
  u8 *p;
  s8 letter;
  s32 cnt;

  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = TEXT_LINE_SPACING;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  if (articleForm == 1 || (articleForm == 3 && plural == 0)) {
    kind = 0;
    table = gArticle_Table;
    head = name[0];
    // The class operand is stored one above its value. A name with no class
    // takes "an" (class 2) before a capital vowel and "a" (class 1) before
    // anything else.
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
      if ((letter = *p++) == 0)
        break;
      entries[writePosition] = letter;
      writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    }
  } else {
    head = name[0];
    if (head == 29)
      name += 2;
  }
  while (*name != 0) {
    code = *name++;
    entries[writePosition] = code;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    if (code == 'S' || code == 's')
      *suffix = 1;
    else
      *suffix = 0;
  }
  if (articleForm == 2 || (articleForm == 3 && plural != 0)) {
    if (*suffix != 0) {
      entries[writePosition] = 'e';
      writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    }
    entries[writePosition] = 's';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  if (mode != 0) {
    entries[writePosition] = TEXT_LINE_SPACING;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 8;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  return writePosition;
}
#endif

/// The control codes of a message as stored (the PO catalogs' names), which
/// Message_Decode expands into decoded text. Codes it does not
/// name here are kept as they are; an operand is stored one above its value.
enum {
  MESSAGE_END = 0,        ///< kept as TEXT_END: the window stays open
  MESSAGE_PAGE_BREAK = 1, ///< kept as TEXT_PAGE
  MESSAGE_END_WAIT = 2,   ///< kept as TEXT_WAIT_KEY: a button closes the window
  MESSAGE_LINE_BREAK = 3, ///< kept as TEXT_NEWLINE
  MESSAGE_TEXT_COLOR = 8, ///< kept with its operand as TEXT_COLOR
  /// Kept with its operand as TEXT_OUTLINE: the style of sprite text, 1
  /// outlined (Text_DrawGlyphPair), 2 to 5 a motion (Glyph_Draw).
  MESSAGE_TEXT_EFFECT = 9,
  MESSAGE_LINE_SPACING = 10, ///< kept with its operand as TEXT_LINE_SPACING
  /// German: a counted noun; Italian: a gender ending; Spanish and French: the
  /// plural suffix
  MESSAGE_LOCALIZED_WORD = 13,
  /// Kept with its operand as TEXT_WINDOW_STATE: the window animation to run.
  MESSAGE_WINDOW_EFFECT = 15,
  /// The name of the unit gMainRoutineWork.leaderActorId names.
  MESSAGE_PROTAGONIST_NAME = 16,
  MESSAGE_ACTOR_NAME = 17, ///< the name of the unit its operand names
  // The argument codes write out the next message argument of their kind
  // (MESSAGE_ARGUMENT_). Outside Japanese the actor, class and item codes have
  // an operand, the article form.
  MESSAGE_ARGUMENT_ACTOR_NAME = 18,
  MESSAGE_ARGUMENT_CLASS_NAME = 19,
  MESSAGE_ARGUMENT_ITEM_NAME = 20,
  MESSAGE_ARGUMENT_ABILITY_NAME = 21,
  /// The number argument, with a minus sign when negative; outside Japanese
  /// any number but -1, 0 and 1 makes what follows plural.
  MESSAGE_ARGUMENT_SIGNED_NUMBER = 22,
  MESSAGE_ARGUMENT_LOCATION_NAME = 23,
  /// TEXT_ARGUMENT_SPACE (code 143 in English, a space in German, Spanish,
  /// French and Italian) and a hyphen, and in those four a space after; the
  /// Japanese decoder keeps the code as it is.
  MESSAGE_EM_DASH = 24,
  /// English: the plural suffix; German, Spanish, French and Italian: the
  /// item's possessive
  MESSAGE_PLURAL_SUFFIX = 25,
  /// A button drawn as two characters, codes 128 + 2 * n and the next for
  /// operand value n; the Japanese decoder keeps the code as it is.
  MESSAGE_BUTTON_ICON = 26,
  /// English: an apostrophe, and an s unless the name ended in s; German: an
  /// apostrophe after s or z, else an s; nothing in Spanish, French and
  /// Italian. The Japanese decoder keeps the code as it is.
  MESSAGE_POSSESSIVE_SUFFIX = 27,
  /// The article class of the name it starts, in item and class names, which
  /// Message_AppendArticleName reads and drops.
  MESSAGE_ARTICLE_CLASS = 29,
  /// Kept as TEXT_END_ALT, which the message ends as it ends TEXT_END.
  MESSAGE_END_NOW = 30,
};

/// What the decode flags of Message_SetDecodeFlags put between characters.
/// Halting speech (haltingSpeech): a short pause before each decoded code; in
/// English and Japanese not at a line's start or before a kana mark.
#define ENTRY_NARROW_GAP TEXT_PAUSE_SHORT
/// With spacingAfter (English and Japanese): code 222 after a character of
/// codes 128 to 256, other than the kana marks and codes 161, 164 and 165,
/// unless the code after it is a kana mark.
#define ENTRY_WIDE_GAP TEXT_VOICED_MARK

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#if (LANGUAGE == LANGUAGE_FRENCH)
extern s8 gLocalizedWord_ItemPronoun[];
#else
/// The possessive written before an item's name, chosen by the item's gender
/// or by number.
struct ItemPossessiveTable {
  s8 *text[ITEM_POSSESSIVE_TEXT_COUNT];
};
extern const struct ItemPossessiveTable gLocalizedWord_ItemPossessives;
extern const s8 gLocalizedWord_ItemGenders[];
extern u8 *gLocalizedWord_CountedNouns[];
#endif
#endif

/// Decodes message script into the ring of decoded text (WindowWork.textBuffer)
/// and returns where it starts; script -1 returns the last message's start
/// again. Argument codes take their values from the argument table
/// (MessageArgument_Push), and clear empties the table afterwards. Names,
/// numbers and articles are written out; style codes are kept with their
/// operands for the message to run.
/// The message is written at WindowWork.textBufferEnd, which it leaves just
/// past the message's terminator, and is cut at the ring's length. The decoder
/// routine is copied into heap slot TEXT_WORK_BLOCK for the call and freed at
/// its end.
s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = WINDOW_WORK;
  u32 code = 0;
#if (LANGUAGE == LANGUAGE_ENGLISH)
  s32 lineStart = 1;
#endif
  s32 quote = 0;
  u32 start = work->textBufferEnd;
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
  s8 letter;
  u32 writePosition = start;
#endif
  u16 *entry = work->textBuffer;
  s32 overflow = 0;
  s32 (*decode)(struct MessageReader *);
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
  u16 name[MESSAGE_NAME_HALFWORDS];
  u8 numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
  struct MessageReader reader;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_ITALIAN)
  struct ItemPossessiveTable possessives;
  s32 kind;
#endif

  if (script == -1) {
    start = work->lastMessageStart;
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
              HeapSlot_Alloc(TEXT_WORK_BLOCK, size), DMA_COPY32(size >> 2),
              REG_DMA3);
    }
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(
        struct MessageReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
    Message_Find(&reader, script);

    do {
#if (LANGUAGE == LANGUAGE_ENGLISH)
      previousCode = code;
#endif
      code = (u32)decode(&reader);
      // These editions have no characters above 255: such a symbol becomes
      // code 64.
      if (code > 255)
        code = 64;

      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (code < 32) {
          switch (code) {
          case MESSAGE_ARGUMENT_CLASS_NAME:
            decode(&reader);
            MessageArgument_Lookup(MESSAGE_ARGUMENT_CLASS, clear);
            break;
          case MESSAGE_END:
          case MESSAGE_END_WAIT:
          case MESSAGE_END_NOW:
            running = 0;
            break;
          case MESSAGE_ARGUMENT_SIGNED_NUMBER:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_NUMBER, clear);
            break;
          case MESSAGE_ARGUMENT_ITEM_NAME:
            decode(&reader);
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, clear);
            break;
          case MESSAGE_ARGUMENT_ABILITY_NAME:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ABILITY, clear);
            break;
          case MESSAGE_ARGUMENT_LOCATION_NAME:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_LOCATION, clear);
            break;
          case MESSAGE_TEXT_COLOR:
          case MESSAGE_TEXT_EFFECT:
            decode(&reader);
            break;
          case MESSAGE_ACTOR_NAME:
            decode(&reader);
            break;
          case MESSAGE_ARGUMENT_ACTOR_NAME:
            decode(&reader);
            break;
          case MESSAGE_ARTICLE_CLASS:
            decode(&reader);
            break;
          case MESSAGE_PAGE_BREAK:
            // The message ends at its first page, waiting for a button.
            running = 0;
            code = 2;
            break;
          case MESSAGE_PROTAGONIST_NAME:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        // Halting speech pauses before each code.
        if (work->haltingSpeech != 0
#if (LANGUAGE == LANGUAGE_ENGLISH)
            && lineStart == 0 && code != 222 && code != 223
#endif
        ) {
          entry[writePosition] = ENTRY_NARROW_GAP;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }

#if (LANGUAGE == LANGUAGE_ENGLISH)
        if (work->spacingAfter != 0 && lineStart == 0 && code != 222 &&
            code != 223 && previousCode <= 256 && previousCode > 127 &&
            previousCode != 222 && previousCode != 223 && previousCode != 32 &&
            previousCode != 165 && previousCode != 161 && previousCode != 164) {
          entry[writePosition] = ENTRY_WIDE_GAP;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }

#endif
        // A character. Halting speech breaks off with an ellipsis at the
        // first space or after 11 characters.
        if (code > 31) {
          if (work->haltingSpeech != 0 && (code == 32 || decodedCount > 10)) {
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
          // Every other double quote, the opening one, becomes code 142.
          if (code == 34) {
            quote ^= 1;
            if (quote != 0)
              code = 142;
          }
          entry[writePosition] = (u16)code;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
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
          case MESSAGE_END:
          case MESSAGE_END_WAIT:
          case MESSAGE_END_NOW:
            running = 0;
            break;
          case MESSAGE_TEXT_COLOR:
          case MESSAGE_TEXT_EFFECT:
          case MESSAGE_ARTICLE_CLASS:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = decode(&reader) + 0xFFFFU;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          case MESSAGE_ARGUMENT_SIGNED_NUMBER:
            value = (s32)MessageArgument_Lookup(MESSAGE_ARGUMENT_NUMBER, clear);
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
            numberText = String_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != 16 && numberBuffer[i] != 0) {
              entry[writePosition] = numberBuffer[i];
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              i++;
            }
            break;
          case MESSAGE_ARGUMENT_CLASS_NAME:
            no = decode(&reader) - 1;
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_CLASS, clear);
            Message_DecodeRaw(argument + MsgClassName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case MESSAGE_ARGUMENT_ITEM_NAME:
            no = decode(&reader) - 1;
#if (LANGUAGE == LANGUAGE_SPANISH)
            form = no & 7;
#endif
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, clear);
            // The argument is an inventory code: its low 9 bits are the item
            // (INVENTORY_ITEM_ID_MASK), which TEXT_BUFFER_MASK spells here. In
            // German, Spanish, French and Italian, forms 2 and 4, and 3 after a
            // plural number, take the item's plural name.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
            argument &= TEXT_BUFFER_MASK;
#if (LANGUAGE == LANGUAGE_SPANISH)
            if (form == 2 || (form == 3 && plural != 0) || form == 4)
#else
            if (no == 2 || (no == 3 && plural != 0) || no == 4)
#endif
              Message_DecodeRaw(argument + MsgItemPluralName, name,
                                MESSAGE_NAME_HALFWORDS);
            else
              Message_DecodeRaw(argument + MsgItemName, name,
                                MESSAGE_NAME_HALFWORDS);
#else
            Message_DecodeRaw((argument & TEXT_BUFFER_MASK) + MsgItemName, name,
                              24);
#endif
            writePosition = Message_AppendArticleName(
                0, name, writePosition, entry, no, plural, &suffix);
            break;
          case MESSAGE_ARGUMENT_ABILITY_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ABILITY, clear);
            Message_DecodeRaw(argument + MsgAbilityName, name,
                              MESSAGE_NAME_HALFWORDS);
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & TEXT_BUFFER_MASK;
            }
            writePosition = nextPosition;
            break;
          case MESSAGE_ARGUMENT_LOCATION_NAME:
            Message_DecodeRaw(
                Area_GetNameIndexFar(
                    MessageArgument_Lookup(MESSAGE_ARGUMENT_LOCATION, clear),
                    1) +
                    MsgDebugAreaName,
                name, MESSAGE_NAME_HALFWORDS);
            p = name;
            nextPosition = writePosition;
            while (*p != 0) {
              entry[nextPosition] = *p;
              p++;
              nextPosition = (nextPosition + 1) & TEXT_BUFFER_MASK;
            }
            writePosition = nextPosition;
            break;
          case MESSAGE_PROTAGONIST_NAME:
            // A unit's name is 15 bytes, widened here to codes.
            nameBytes = Unit_GetFar(gMainRoutineWork.leaderActorId)->name;
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
          case MESSAGE_ARGUMENT_ACTOR_NAME:
            no = decode(&reader) - 1;
            nameBytes = Unit_GetFar(MessageArgument_Lookup(
                                        MESSAGE_ARGUMENT_ACTOR, clear))
                            ->name;
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
          case MESSAGE_ACTOR_NAME:
            argument = decode(&reader) - 1;
            nameBytes = Unit_GetFar(argument)->name;
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
          case MESSAGE_BUTTON_ICON:
            argument = ((decode(&reader) - 1) << 1) + 128;
            entry[writePosition] = argument;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = argument + 1;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          case MESSAGE_EM_DASH:
            entry[writePosition] = TEXT_ARGUMENT_SPACE;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = '-';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
            entry[writePosition] = ' ';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#endif
            break;
#if (LANGUAGE == LANGUAGE_GERMAN)
          case MESSAGE_LOCALIZED_WORD:
            no = decode(&reader) - 1;
            nameBytes = gLocalizedWord_CountedNouns[(no & 7) * 2 + plural];
            while (*nameBytes != 0) {
              entry[writePosition] = *nameBytes++;
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            break;
#endif
#if (LANGUAGE == LANGUAGE_ITALIAN)
          case MESSAGE_LOCALIZED_WORD:
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
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
          case MESSAGE_LOCALIZED_WORD:
#elif (LANGUAGE == LANGUAGE_ENGLISH)
          case MESSAGE_PLURAL_SUFFIX:
#endif
#if !(LANGUAGE == LANGUAGE_GERMAN) && !(LANGUAGE == LANGUAGE_ITALIAN)
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
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
          case MESSAGE_PLURAL_SUFFIX:
            // The item argument is only looked at (clear 0), so the item's
            // name code after the possessive still takes it.
#if (LANGUAGE == LANGUAGE_SPANISH)
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            possessives = gLocalizedWord_ItemPossessives;
            word = possessives.text[plural];
#else
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            argument &= TEXT_BUFFER_MASK;
            possessives = gLocalizedWord_ItemPossessives;
            kind = gLocalizedWord_ItemGenders[argument];
            word = possessives.text[kind & 3];
#endif
            // value counts the letters, compared unsigned: with i or j the
            // registers differ, and a signed compare changes the branch.
            for (value = 0; (u32)value < 16; value++) {
              if ((letter = *word++) == 0)
                break;
              if (value == 0 && suffix == 2)
                letter -= 'a' - 'A';
              entry[writePosition] = letter;
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            break;
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
          case MESSAGE_PLURAL_SUFFIX:
#if (LANGUAGE == LANGUAGE_GERMAN)
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            argument &= TEXT_BUFFER_MASK;
            possessives = gLocalizedWord_ItemPossessives;
            kind = gLocalizedWord_ItemGenders[argument];
            word = possessives.text[kind & 3];
#else
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            word = gLocalizedWord_ItemPronoun;
#endif
            for (i = 0; i < 16; i++) {
              if ((letter = *word++) == 0)
                break;
              if (i == 0 && suffix == 2)
                letter -= 'a' - 'A';
              entry[writePosition] = letter;
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
            suffix = 0;
            break;
#endif
          case MESSAGE_POSSESSIVE_SUFFIX:
#if (LANGUAGE == LANGUAGE_GERMAN)
            if (suffix == 1)
              entry[writePosition] = '\'';
            else
              entry[writePosition] = 's';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#elif (LANGUAGE == LANGUAGE_ENGLISH)
            entry[writePosition] = '\'';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            if (suffix == 0) {
              entry[writePosition] = 's';
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            }
#endif
            break;
          case -1:
            break;
          case MESSAGE_PAGE_BREAK:
          case MESSAGE_LINE_BREAK:
#if (LANGUAGE == LANGUAGE_ENGLISH)
            lineStart = 1;
#endif
            // Keep the break in the decoded text.
            // Fall through.
          default:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
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
    } while (running != 0 && decodedCount <= TEXT_BUFFER_MASK);

    // The code that ended the message goes in too (TEXT_END, TEXT_WAIT_KEY or
    // TEXT_END_ALT, or the last code of a message cut at the ring's length),
    // then a 0; the next message starts after both.
    entry[writePosition] = (u16)code;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    work->textBufferEnd = (u16)nextPosition;
    HeapSlot_Release(TEXT_WORK_BLOCK);
    work->lastMessageStart = (u16)start;
  }

  if (clear != 0)
    MessageArgument_ClearAll();

  return start;
}

#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)

/// The Japanese form: Japanese has no articles, so the name is copied as it is.
/// The write position wraps in the text buffer; language grammar remains
/// separate.
extern __inline__ u32 Message_AppendArticleName(s32 mode, u16 *name,
                                                u32 writePosition, u16 *entries,
                                                s32 articleForm, s32 plural,
                                                s32 *suffix) {
  u16 code;

  if (mode != 0) {
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = TEXT_LINE_SPACING;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 10;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  while (*name != 0) {
    code = *name++;
    entries[writePosition] = code;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  if (mode != 0) {
    entries[writePosition] = TEXT_LINE_SPACING;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = 8;
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    entries[writePosition] = ' ';
    writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
  }
  return writePosition;
}

/// The Japanese Message_Decode, as above with no articles or
/// plurals. A battle message (WINDOW_BATTLE_MESSAGE) has its names set off with
/// spaces and style codes and each space doubled, and in battle the units of
/// classes 122 and 125 are named Mystery Man and Mystery Woman.
/// script -1 reuses lastMessageStart; clear consumes argument slots after
/// decoding.
s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = WINDOW_WORK;
  u32 code = 0;
  s32 lineStart = 1;
  u32 start = work->textBufferEnd;
  u32 decodedCount = 0;
  s32 running = 1;
  u32 writePosition = start;
  u16 *entry = work->textBuffer;
  u8 mode = work->battleMode & WINDOW_BATTLE_MESSAGE;
  s32 (*decode)(struct MessageReader *);
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
  struct Unit *unit;
  u16 name[MESSAGE_NAME_HALFWORDS];
  u8 numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
  struct MessageReader reader;

  if (script == -1) {
    start = work->lastMessageStart;
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
              HeapSlot_Alloc(TEXT_WORK_BLOCK, size), DMA_COPY32(size >> 2),
              REG_DMA3);
    }
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(
        struct MessageReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
    Message_Find(&reader, script);
    do {
      previousCode = code;
      code = (u32)decode(&reader);
      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (code < 32) {
          switch (code) {
          case MESSAGE_ARGUMENT_CLASS_NAME:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_CLASS, clear);
            break;
          case MESSAGE_END:
          case MESSAGE_END_WAIT:
          case MESSAGE_END_NOW:
            running = 0;
            break;
          case MESSAGE_TEXT_COLOR:
          case MESSAGE_TEXT_EFFECT:
            decode(&reader);
            break;
          case MESSAGE_ARGUMENT_SIGNED_NUMBER:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_NUMBER, clear);
            break;
          case MESSAGE_ARGUMENT_ITEM_NAME:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, clear);
            break;
          case MESSAGE_ARGUMENT_ABILITY_NAME:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ABILITY, clear);
            break;
          case MESSAGE_ARGUMENT_LOCATION_NAME:
            MessageArgument_Lookup(MESSAGE_ARGUMENT_LOCATION, clear);
            break;
          case MESSAGE_ACTOR_NAME:
            decode(&reader);
            break;
          case MESSAGE_ARGUMENT_ACTOR_NAME:
            break;
          case MESSAGE_PAGE_BREAK:
            running = 0;
            code = 2;
            break;
          case MESSAGE_PROTAGONIST_NAME:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        // Halting speech pauses before each character.
        if (work->haltingSpeech != 0 && lineStart == 0 && code != 222 &&
            code != 223) {
          entry[writePosition] = 5;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }
        if (work->spacingAfter != 0 && lineStart == 0 && code != 222 &&
            code != 223 && previousCode <= 256 && previousCode > 127 &&
            previousCode != 222 && previousCode != 223 && previousCode != 32 &&
            previousCode != 165 && previousCode != 161 && previousCode != 164) {
          entry[writePosition] = 222;
          writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
        }
        // A character. Halting speech breaks off with an ellipsis at the
        // first space or after 11 characters.
        if (code > 31) {
          // A battle message doubles each space. The ellipsis here is three
          // of code 165, then code 161.
          if (mode != 0 && code == ' ') {
            entry[writePosition] = code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
          }
          if (work->haltingSpeech != 0 && (code == 32 || decodedCount > 10)) {
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
          // A control code: write out names and numbers, and keep style
          // codes with their operands.
          switch (code) {
          case MESSAGE_END:
          case MESSAGE_END_WAIT:
          case MESSAGE_END_NOW:
            running = 0;
            break;
          case MESSAGE_TEXT_COLOR:
          case MESSAGE_TEXT_EFFECT:
            entry[writePosition] = (u16)code;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            entry[writePosition] = decode(&reader) + 0xffffU;
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
            break;
          case MESSAGE_ARGUMENT_SIGNED_NUMBER:
            value = MessageArgument_Lookup(MESSAGE_ARGUMENT_NUMBER, clear);
            numberText = String_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != 16 && numberBuffer[i] != 0) {
              entry[writePosition] = numberBuffer[i];
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              i++;
            }
            break;
          case MESSAGE_ARGUMENT_CLASS_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_CLASS, clear);
            Message_DecodeRaw(argument + MsgClassName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_ITEM_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, clear);
            Message_DecodeRaw((argument & 511) + MsgItemName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_ABILITY_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ABILITY, clear);
            Message_DecodeRaw(argument + MsgAbilityName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_LOCATION_NAME:
            Message_DecodeRaw(
                Area_GetNameIndexFar(
                    MessageArgument_Lookup(MESSAGE_ARGUMENT_LOCATION, clear),
                    1) +
                    MsgDebugAreaName,
                name, MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(mode, name, writePosition,
                                                      entry, 0, 0, 0);
            break;
          case MESSAGE_PROTAGONIST_NAME:
            nameBytes = Unit_GetFar(gMainRoutineWork.leaderActorId)->name;
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
          case MESSAGE_ARGUMENT_ACTOR_NAME:
            unit = Unit_GetFar(
                MessageArgument_Lookup(MESSAGE_ARGUMENT_ACTOR, clear));
            // In battle a unit of class 122 is called Mystery Man and one of
            // class 125 Mystery Woman (MsgMysteryPersonName and the next).
            if (work->battleMode != WINDOW_FIELD &&
                (unit->id == 122 || unit->id == 125)) {
              message = MsgMysteryPersonName;
              if (unit->id == 125)
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
          case MESSAGE_ACTOR_NAME:
            argument = decode(&reader) - 1;
            nameBytes = Unit_GetFar(argument)->name;
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
          case -1:
            break;
          case MESSAGE_PAGE_BREAK:
          case MESSAGE_LINE_BREAK:
            lineStart = 1;
            // Keep the break in the decoded text.
            // Fall through.
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
    work->textBufferEnd = (u16)nextPosition;
    HeapSlot_Release(TEXT_WORK_BLOCK);
    work->lastMessageStart = (u16)start;
  }
  if (clear != 0)
    MessageArgument_ClearAll();
  return start;
}

#endif
