/// @file
/// The message decoder: articles, names and numbers written out.
// Part of window.c.

#include "window_internal.h"

/// The most letters of an article Message_AppendArticleName writes, and of a
/// possessive or pronoun before an item's name.
#define ARTICLE_MAX_LETTERS 8
#define POSSESSIVE_MAX_LETTERS 16
/// The highest character code outside Japanese: those fonts hold a byte's
/// worth.
#define CHARACTER_CODE_MAX 255
/// The Japanese font's first kana (charmap_ja.tsv): its kana run to
/// TEXT_LAST_FONT_CODE, and its kanji start at KANJI_FIRST_CODE (window.h).
#define KANA_FIRST_CODE 128
/// Halting speech breaks off with an ellipsis at its first space, or after
/// this many codes.
#define HALTING_SPEECH_MAX_CODES 10
/// MESSAGE_BUTTON_ICON's first character code.
#define BUTTON_ICON_FIRST_CODE 128
/// The value of an operand the message stores one above it, taken one down as
/// a halfword.
#define OPERAND_VALUE(stored) ((stored) + (u32)(u16)-1)

#if LANGUAGE != LANGUAGE_JAPANESE
/// The article classes a name can take in each European language, and the bits
/// of its class.
#if LANGUAGE == LANGUAGE_ITALIAN
#define ARTICLE_KINDS 11
#define ARTICLE_KIND_MASK 15
#elif LANGUAGE == LANGUAGE_SPANISH
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

/// The control codes of a message as stored (the PO catalogs' names), which
/// Message_Decode expands into decoded text.
enum {
  MESSAGE_END = 0,        ///< kept as TEXT_END: the window stays open
  MESSAGE_PAGE_BREAK = 1, ///< kept as TEXT_PAGE
  MESSAGE_END_WAIT = 2,   ///< kept as TEXT_WAIT_KEY: a button closes the window
  MESSAGE_LINE_BREAK = 3, ///< kept as TEXT_NEWLINE
  MESSAGE_TEXT_COLOR = 8, ///< kept with its operand as TEXT_COLOR
  /// Kept with its operand as TEXT_STYLE: the style of sprite text, 1
  /// outlined (Text_DrawGlyphPair), 2 to 5 a motion (Glyph_Draw).
  MESSAGE_TEXT_EFFECT = 9,
  MESSAGE_LINE_SPACING = 10, ///< kept with its operand as TEXT_LINE_SPACING
  MESSAGE_UNUSED_11 = 11,    ///< kept with its operand as TEXT_UNUSED_11
  MESSAGE_UNUSED_12 = 12,    ///< kept with its operand as TEXT_UNUSED_12
  /// German: a counted noun; Italian: a gender ending; Spanish and French: the
  /// plural suffix
  MESSAGE_LOCALIZED_WORD = 13,
  MESSAGE_UNUSED_14 = 14, ///< kept with its two operands as TEXT_UNUSED_14
  /// Kept with its operand as TEXT_WINDOW_STATE: the window animation to run.
  MESSAGE_WINDOW_EFFECT = 15,
  /// The name of the unit gMainRoutineWork.leaderActorId names.
  MESSAGE_PROTAGONIST_NAME = 16,
  MESSAGE_ACTOR_NAME = 17, ///< the name of the unit its operand names
  // The argument codes write out the next message argument of their kind
  // (MESSAGE_ARGUMENT_).
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

/// How a name argument is written ({argument_item_name:n} and the like, the
/// catalogs showing the form one above): with or without its article, and
/// singular or plural.
enum ArticleForm {
  ARTICLE_FORM_BARE = 0,    ///< the name alone
  ARTICLE_FORM_ARTICLE = 1, ///< the name after its article ("a Herb")
  /// English: the name with its plural ending ("Herbs"); the other European
  /// languages: the plural name after its article.
  ARTICLE_FORM_PLURAL = 2,
  /// ARTICLE_FORM_ARTICLE or ARTICLE_FORM_PLURAL, by the number argument
  /// written before it.
  ARTICLE_FORM_BY_NUMBER = 3,
  /// Outside English: the plural name alone.
  ARTICLE_FORM_BARE_PLURAL = 4,
};
#if LANGUAGE == LANGUAGE_SPANISH
/// In Spanish, the form's bit asking for the indefinite article rather than
/// the definite one, above the form.
#define ARTICLE_FORM_INDEFINITE 8
#define ARTICLE_FORM_MASK 7
#endif

/// The state of the text just written, which an article or ending after it
/// follows: a sentence's end capitalises the next article, and a name ending
/// in s (in German, s or z) takes a different possessive or plural ending.
enum {
  LAST_LETTER_OTHER = 0,
  LAST_LETTER_S = 1,
  LAST_LETTER_SENTENCE_END = 2,
};

// Message_AppendArticleName copies a name into the decoded text at
// writePosition, as articleForm asks, and returns the position after it. In a
// Japanese battle message the name is set off by a space on each side, typed
// at the default line spacing with the spacing 8 after it.

#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
/// The German, Spanish, French and Italian form: the bare forms take no
/// article, the others the article of the name's class, capitalised at the
/// start of a sentence.
u32 Message_AppendArticleName(s32 inBattleMessage, u16 *name, u32 writePosition,
                              u16 *entries, s32 articleForm, s32 plural,
                              s32 *lastLetter) {
  struct ArticleTable table;
#if LANGUAGE == LANGUAGE_SPANISH
  struct ArticleTable indefiniteTable;
  u16 form;
  s16 indefinite;
#endif
  s32 kind;
  u16 head;
  u16 code;
  s8 *p;
  s8 letter;
  s32 letterIndex;

#if LANGUAGE == LANGUAGE_SPANISH
  form = articleForm & ARTICLE_FORM_MASK;
  indefinite = articleForm & ARTICLE_FORM_INDEFINITE;
#endif
  if (inBattleMessage != 0) {
    TEXT_APPEND(entries, writePosition, ' ');
    TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
    TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
  }
  // A name's leading article class (MESSAGE_ARTICLE_CLASS and its
  // operand) is never copied.
#if LANGUAGE == LANGUAGE_SPANISH
  if (form == ARTICLE_FORM_BARE || form == ARTICLE_FORM_BARE_PLURAL) {
#else
  if (articleForm == ARTICLE_FORM_BARE ||
      articleForm == ARTICLE_FORM_BARE_PLURAL) {
#endif
    head = name[0];
    if (head == MESSAGE_ARTICLE_CLASS)
      name += 2;
  } else {
    kind = 0;
    table = gArticle_Table;
#if LANGUAGE == LANGUAGE_SPANISH
    indefiniteTable = gArticle_Indefinite;
#endif
    if (name[0] == MESSAGE_ARTICLE_CLASS) {
      kind = name[1];
      name += 2;
    }
#if LANGUAGE == LANGUAGE_SPANISH
    if (indefinite == 0)
      p = table.text[kind & ARTICLE_KIND_MASK];
    else
      p = indefiniteTable.text[kind & ARTICLE_KIND_MASK];
#else
    p = table.text[kind & ARTICLE_KIND_MASK];
#endif
    // The first letter is capitalised at the start of a sentence.
    for (letterIndex = 0; letterIndex < ARTICLE_MAX_LETTERS; letterIndex++) {
      if ((letter = *p++) == 0)
        break;
      if (letterIndex == 0 && *lastLetter == LAST_LETTER_SENTENCE_END)
        letter -= 'a' - 'A';
      TEXT_APPEND(entries, writePosition, letter);
    }
  }
  while (*name != 0) {
    code = *name++;
    TEXT_APPEND(entries, writePosition, code);
#if LANGUAGE == LANGUAGE_GERMAN
    if (code == 's' || code == 'S' || code == 'z' || code == 'Z')
      *lastLetter = LAST_LETTER_S;
    else
      *lastLetter = LAST_LETTER_OTHER;
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
    if (code == 's' || code == 'S')
      *lastLetter = LAST_LETTER_S;
    else
      *lastLetter = LAST_LETTER_OTHER;
#endif
  }
  if (inBattleMessage != 0) {
    TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
    TEXT_APPEND(entries, writePosition, 8);
    TEXT_APPEND(entries, writePosition, ' ');
  }
  return writePosition;
}
#endif

#if LANGUAGE == LANGUAGE_ENGLISH
/// The English article classes Message_AppendArticleName chooses between
/// (gArticle_Table) when a name's class does not name one.
enum {
  ARTICLE_CLASS_NONE = 0,
  ARTICLE_CLASS_A = 1,
  ARTICLE_CLASS_AN = 2,
};

/// The English form: the singular takes "a" or "an" (by the name's first
/// letter, unless its class names an article), and the plural adds "s", or "es"
/// after an s.
u32 Message_AppendArticleName(s32 inBattleMessage, u16 *name, u32 writePosition,
                              u16 *entries, s32 articleForm, s32 plural,
                              s32 *lastLetter) {
  struct ArticleTable table;
  s32 kind;
  u16 head;
  u16 code;
  u8 *p;
  s8 letter;
  s32 letterIndex;

  if (inBattleMessage != 0) {
    TEXT_APPEND(entries, writePosition, ' ');
    TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
    TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
  }
  if (articleForm == ARTICLE_FORM_ARTICLE ||
      (articleForm == ARTICLE_FORM_BY_NUMBER && plural == 0)) {
    kind = 0;
    table = gArticle_Table;
    head = name[0];
    // The class operand is stored one above its value.
    if (head == MESSAGE_ARTICLE_CLASS) {
      kind = name[1] - 1;
      name += 2;
    }
    if (kind == ARTICLE_CLASS_NONE) {
      if (head == 'A' || head == 'I' || head == 'U' || head == 'E' ||
          head == 'O')
        kind = ARTICLE_CLASS_AN;
      else
        kind = ARTICLE_CLASS_A;
    }
    p = table.text[kind & 7];
    for (letterIndex = 0; letterIndex < ARTICLE_MAX_LETTERS; letterIndex++) {
      if ((letter = *p++) == 0)
        break;
      TEXT_APPEND(entries, writePosition, letter);
    }
  } else {
    head = name[0];
    if (head == MESSAGE_ARTICLE_CLASS)
      name += 2;
  }
  while (*name != 0) {
    code = *name++;
    TEXT_APPEND(entries, writePosition, code);
    if (code == 'S' || code == 's')
      *lastLetter = LAST_LETTER_S;
    else
      *lastLetter = LAST_LETTER_OTHER;
  }
  if (articleForm == ARTICLE_FORM_PLURAL ||
      (articleForm == ARTICLE_FORM_BY_NUMBER && plural != 0)) {
    if (*lastLetter != LAST_LETTER_OTHER) {
      TEXT_APPEND(entries, writePosition, 'e');
    }
    TEXT_APPEND(entries, writePosition, 's');
  }
  if (inBattleMessage != 0) {
    TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
    TEXT_APPEND(entries, writePosition, 8);
    TEXT_APPEND(entries, writePosition, ' ');
  }
  return writePosition;
}
#endif

/// What the decode flags of Message_SetDecodeFlags put between characters.
#define ENTRY_NARROW_GAP TEXT_PAUSE_SHORT
/// With spacingAfter (English and Japanese): TEXT_MARK_VOICED after a kana or
/// the first kanji (KANA_FIRST_CODE to KANJI_FIRST_CODE), other than the kana
/// marks and the Japanese full stop, comma and middle dot, unless the code
/// after it is a kana mark.
#define ENTRY_WIDE_GAP TEXT_MARK_VOICED

#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN ||             \
    LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN

#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#if LANGUAGE == LANGUAGE_FRENCH
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
/// again.
s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = WINDOW_WORK;
  u32 code = 0;
#if LANGUAGE == LANGUAGE_ENGLISH
  s32 lineStart = 1;
#endif
  s32 quote = 0;
  u32 start = work->textBufferEnd;
  u32 decodedCount = 0;
  s32 plural = 0;
#if LANGUAGE != LANGUAGE_GERMAN && LANGUAGE != LANGUAGE_ITALIAN
  s32 running = 1;
#endif
#if LANGUAGE == LANGUAGE_ENGLISH
  u32 writePosition = start;
  u32 previousCode;
  s32 lastLetter = LAST_LETTER_OTHER;
#else
#if LANGUAGE == LANGUAGE_SPANISH
  struct ItemPossessiveTable possessives;
  s32 form;
#endif
  s32 lastLetter = LAST_LETTER_SENTENCE_END;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
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
  s32 articleForm;
  u32 j;
  u32 i;
  s32 value;
  s32 argument;
#if LANGUAGE == LANGUAGE_ITALIAN
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
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
  struct ItemPossessiveTable possessives;
  s32 kind;
#endif

  if (script == -1) {
    start = work->lastMessageStart;
  } else {
    RamCode_LoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbol,
                     Text_DecodeSymbolCodeSize);
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(struct MessageReader *))HEAP_SLOT_BLOCK(
        HEAP_SLOT_MESSAGE_DECODER);
    Message_Find(&reader, script);

    for (; running != 0 && decodedCount <= TEXT_BUFFER_MASK; decodedCount++) {
#if LANGUAGE == LANGUAGE_ENGLISH
      previousCode = code;
#endif
      code = (u32)decode(&reader);
      // A symbol past the font's characters becomes '@'.
      if (code > CHARACTER_CODE_MAX)
        code = '@';

      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (code < TEXT_SPACE) {
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
            code = TEXT_WAIT_KEY;
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
#if LANGUAGE == LANGUAGE_ENGLISH
            && lineStart == 0 && code != TEXT_MARK_VOICED &&
            code != TEXT_MARK_SEMIVOICED
#endif
        ) {
          TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
        }

#if LANGUAGE == LANGUAGE_ENGLISH
        if (work->spacingAfter != 0 && lineStart == 0 &&
            code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
            previousCode <= KANJI_FIRST_CODE &&
            previousCode >= KANA_FIRST_CODE &&
            previousCode != TEXT_MARK_VOICED &&
            previousCode != TEXT_MARK_SEMIVOICED &&
            previousCode != TEXT_SPACE &&
            previousCode != TEXT_JAPANESE_MIDDLE_DOT &&
            previousCode != TEXT_JAPANESE_FULL_STOP &&
            previousCode != TEXT_JAPANESE_COMMA) {
          TEXT_APPEND(entry, writePosition, ENTRY_WIDE_GAP);
        }

#endif
        // A character.
        if (code >= TEXT_SPACE) {
          if (work->haltingSpeech != 0 &&
              (code == TEXT_SPACE || decodedCount > HALTING_SPEECH_MAX_CODES)) {
            TEXT_APPEND(entry, writePosition, '.');
            TEXT_APPEND(entry, writePosition, '.');
            TEXT_APPEND(entry, writePosition, '.');
            overflow = 1;
            if (decodedCount > HALTING_SPEECH_MAX_CODES)
              code = TEXT_SPACE;
          }
          // Every other double quote, the opening one, becomes
          // TEXT_OPENING_QUOTE.
          if (code == '"') {
            quote ^= 1;
            if (quote != 0)
              code = TEXT_OPENING_QUOTE;
          }
          TEXT_APPEND(entry, writePosition, (u16)code);
#if LANGUAGE == LANGUAGE_ENGLISH
          lineStart = 0;
#else
          if (code != ' '
#if LANGUAGE == LANGUAGE_SPANISH
              && code != TEXT_INVERTED_EXCLAMATION_MARK &&
              code != TEXT_INVERTED_QUESTION_MARK
#endif
          ) {
            lastLetter = LAST_LETTER_OTHER;
            if (code == '.' || code == '!' || code == '?')
              lastLetter = LAST_LETTER_SENTENCE_END;
            if (code == 's' || code == 'S'
#if LANGUAGE == LANGUAGE_GERMAN
                || code == 'z' || code == 'Z'
#endif
            )
              lastLetter = LAST_LETTER_S;
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
            TEXT_APPEND(entry, writePosition, (u16)code);
            TEXT_APPEND(entry, writePosition, OPERAND_VALUE(decode(&reader)));
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
            for (i = numberText - numberBuffer;
                 i != TEXT_NUMBER_BUFFER_SIZE && numberBuffer[i] != 0; i++) {
              TEXT_APPEND(entry, writePosition, numberBuffer[i]);
            }
            break;
          case MESSAGE_ARGUMENT_CLASS_NAME:
            articleForm = decode(&reader) - 1;
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_CLASS, clear);
            Message_DecodeRaw(argument + MsgClassName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition =
                Message_AppendArticleName(0, name, writePosition, entry,
                                          articleForm, plural, &lastLetter);
            break;
          case MESSAGE_ARGUMENT_ITEM_NAME:
            articleForm = decode(&reader) - 1;
#if LANGUAGE == LANGUAGE_SPANISH
            form = articleForm & ARTICLE_FORM_MASK;
#endif
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, clear);
            // The argument is an inventory code, the item in its low bits.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
            argument &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE == LANGUAGE_SPANISH
            if (form == ARTICLE_FORM_PLURAL ||
                (form == ARTICLE_FORM_BY_NUMBER && plural != 0) ||
                form == ARTICLE_FORM_BARE_PLURAL)
#else
            if (articleForm == ARTICLE_FORM_PLURAL ||
                (articleForm == ARTICLE_FORM_BY_NUMBER && plural != 0) ||
                articleForm == ARTICLE_FORM_BARE_PLURAL)
#endif
              Message_DecodeRaw(argument + MsgItemPluralName, name,
                                MESSAGE_NAME_HALFWORDS);
            else
              Message_DecodeRaw(argument + MsgItemName, name,
                                MESSAGE_NAME_HALFWORDS);
#else
            Message_DecodeRaw((argument & INVENTORY_ITEM_ID_MASK) + MsgItemName,
                              name, MESSAGE_NAME_HALFWORDS);
#endif
            writePosition =
                Message_AppendArticleName(0, name, writePosition, entry,
                                          articleForm, plural, &lastLetter);
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
            // A unit's name, widened here to codes.
            nameBytes = Unit_GetFar(gMainRoutineWork.leaderActorId)->name;
            nameOutput = name;
            i = 0;
            do {
              *nameOutput = *nameBytes;
              i++;
              nameBytes++;
              nameOutput++;
            } while (i < UNIT_NAME_SIZE);
            writePosition = Message_AppendArticleName(0, name, writePosition,
                                                      entry, 0, 0, &lastLetter);
            break;
          case MESSAGE_ARGUMENT_ACTOR_NAME:
            articleForm = decode(&reader) - 1;
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
            } while (j < UNIT_NAME_SIZE);
            writePosition =
                Message_AppendArticleName(0, name, writePosition, entry,
                                          articleForm, plural, &lastLetter);
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
            } while (j < UNIT_NAME_SIZE);
            writePosition = Message_AppendArticleName(0, name, writePosition,
                                                      entry, 0, 0, &lastLetter);
            break;
          case MESSAGE_BUTTON_ICON:
            argument = ((decode(&reader) - 1) << 1) + BUTTON_ICON_FIRST_CODE;
            TEXT_APPEND(entry, writePosition, argument);
            TEXT_APPEND(entry, writePosition, argument + 1);
            break;
          case MESSAGE_EM_DASH:
            TEXT_APPEND(entry, writePosition, TEXT_ARGUMENT_SPACE);
            TEXT_APPEND(entry, writePosition, '-');
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
            TEXT_APPEND(entry, writePosition, ' ');
#endif
            break;
#if LANGUAGE == LANGUAGE_GERMAN
          case MESSAGE_LOCALIZED_WORD:
            // The operand picks the noun here, not an article form.
            articleForm = decode(&reader) - 1;
            nameBytes =
                gLocalizedWord_CountedNouns[(articleForm & 7) * 2 + plural];
            while (*nameBytes != 0) {
              TEXT_APPEND(entry, writePosition, *nameBytes++);
            }
            break;
#endif
#if LANGUAGE == LANGUAGE_ITALIAN
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
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
          case MESSAGE_LOCALIZED_WORD:
#elif LANGUAGE == LANGUAGE_ENGLISH
          case MESSAGE_PLURAL_SUFFIX:
#endif
#if LANGUAGE != LANGUAGE_GERMAN && LANGUAGE != LANGUAGE_ITALIAN
            if (plural != 0) {
              if (lastLetter != LAST_LETTER_OTHER) {
                TEXT_APPEND(entry, writePosition, 'e');
              }
              TEXT_APPEND(entry, writePosition, 's');
            }
            break;
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
          case MESSAGE_PLURAL_SUFFIX:
            // The item argument is only looked at (clear 0), so the item's
            // name code after the possessive still takes it.
#if LANGUAGE == LANGUAGE_SPANISH
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            possessives = gLocalizedWord_ItemPossessives;
            word = possessives.text[plural];
#else
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            argument &= INVENTORY_ITEM_ID_MASK;
            possessives = gLocalizedWord_ItemPossessives;
            kind = gLocalizedWord_ItemGenders[argument];
            word = possessives.text[kind & 3];
#endif
            for (value = 0; (u32)value < POSSESSIVE_MAX_LETTERS; value++) {
              if ((letter = *word++) == 0)
                break;
              if (value == 0 && lastLetter == LAST_LETTER_SENTENCE_END)
                letter -= 'a' - 'A';
              TEXT_APPEND(entry, writePosition, letter);
            }
            break;
#endif
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
          case MESSAGE_PLURAL_SUFFIX:
#if LANGUAGE == LANGUAGE_GERMAN
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            argument &= INVENTORY_ITEM_ID_MASK;
            possessives = gLocalizedWord_ItemPossessives;
            kind = gLocalizedWord_ItemGenders[argument];
            word = possessives.text[kind & 3];
#else
            MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, 0);
            word = gLocalizedWord_ItemPronoun;
#endif
            for (i = 0; i < POSSESSIVE_MAX_LETTERS; i++) {
              if ((letter = *word++) == 0)
                break;
              if (i == 0 && lastLetter == LAST_LETTER_SENTENCE_END)
                letter -= 'a' - 'A';
              TEXT_APPEND(entry, writePosition, letter);
            }
            lastLetter = LAST_LETTER_OTHER;
            break;
#endif
          case MESSAGE_POSSESSIVE_SUFFIX:
#if LANGUAGE == LANGUAGE_GERMAN
            if (lastLetter == LAST_LETTER_S)
              entry[writePosition] = '\'';
            else
              entry[writePosition] = 's';
            writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#elif LANGUAGE == LANGUAGE_ENGLISH
            TEXT_APPEND(entry, writePosition, '\'');
            if (lastLetter == LAST_LETTER_OTHER) {
              TEXT_APPEND(entry, writePosition, 's');
            }
#endif
            break;
          case -1:
            break;
          case MESSAGE_PAGE_BREAK:
          case MESSAGE_LINE_BREAK:
#if LANGUAGE == LANGUAGE_ENGLISH
            lineStart = 1;
#endif
            // Keep the break in the decoded text.
            // Fall through.
          default:
            TEXT_APPEND(entry, writePosition, (u16)code);
#if LANGUAGE == LANGUAGE_ENGLISH
            if (code == 's' || code == 'S')
              lastLetter = LAST_LETTER_S;
            else
              lastLetter = LAST_LETTER_OTHER;
#endif
            break;
          }
        }
      }
    }

    // The code that ended the message goes in too (TEXT_END, TEXT_WAIT_KEY or
    // TEXT_END_ALT, or the last code of a message cut at the ring's length),
    // then a 0; the next message starts after both.
    TEXT_APPEND(entry, writePosition, (u16)code);
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    work->textBufferEnd = (u16)nextPosition;
    HeapSlot_Release(HEAP_SLOT_MESSAGE_DECODER);
    work->lastMessageStart = (u16)start;
  }

  if (clear != 0)
    MessageArgument_ClearAll();

  return start;
}

#endif

#if LANGUAGE == LANGUAGE_JAPANESE

/// The most codes the Japanese decoder takes of the name it gives the Mystery
/// Man or the Mystery Woman in battle.
#define MYSTERY_PERSON_NAME_CODES 10

/// The Japanese form: Japanese has no articles, so the name is copied as it is.
extern __inline__ u32 Message_AppendArticleName(s32 inBattleMessage, u16 *name,
                                                u32 writePosition, u16 *entries,
                                                s32 articleForm, s32 plural,
                                                s32 *lastLetter) {
  u16 code;

  if (inBattleMessage != 0) {
    TEXT_APPEND(entries, writePosition, ' ');
    TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
    TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
  }
  while (*name != 0) {
    code = *name++;
    TEXT_APPEND(entries, writePosition, code);
  }
  if (inBattleMessage != 0) {
    TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
    TEXT_APPEND(entries, writePosition, 8);
    TEXT_APPEND(entries, writePosition, ' ');
  }
  return writePosition;
}

/// The Japanese Message_Decode, as above with no articles or plurals.
s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = WINDOW_WORK;
  u32 code = 0;
  s32 lineStart = 1;
  u32 start = work->textBufferEnd;
  u32 decodedCount = 0;
  s32 running = 1;
  u32 writePosition = start;
  u16 *entry = work->textBuffer;
  u8 inBattleMessage = work->battleMode & WINDOW_BATTLE_MESSAGE;
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
    RamCode_LoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbol,
                     Text_DecodeSymbolCodeSize);
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(struct MessageReader *))HEAP_SLOT_BLOCK(
        HEAP_SLOT_MESSAGE_DECODER);
    Message_Find(&reader, script);
    for (; running != 0 && decodedCount <= TEXT_BUFFER_MASK; decodedCount++) {
      previousCode = code;
      code = (u32)decode(&reader);
      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (code < TEXT_SPACE) {
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
            code = TEXT_WAIT_KEY;
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
        if (work->haltingSpeech != 0 && lineStart == 0 &&
            code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
          TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
        }
        if (work->spacingAfter != 0 && lineStart == 0 &&
            code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
            previousCode <= KANJI_FIRST_CODE &&
            previousCode >= KANA_FIRST_CODE &&
            previousCode != TEXT_MARK_VOICED &&
            previousCode != TEXT_MARK_SEMIVOICED &&
            previousCode != TEXT_SPACE &&
            previousCode != TEXT_JAPANESE_MIDDLE_DOT &&
            previousCode != TEXT_JAPANESE_FULL_STOP &&
            previousCode != TEXT_JAPANESE_COMMA) {
          TEXT_APPEND(entry, writePosition, ENTRY_WIDE_GAP);
        }
        // A character.
        if (code >= TEXT_SPACE) {
          // A battle message doubles each space. The ellipsis here is
          // ・・・。.
          if (inBattleMessage != 0 && code == ' ') {
            TEXT_APPEND(entry, writePosition, code);
          }
          if (work->haltingSpeech != 0 &&
              (code == TEXT_SPACE || decodedCount > HALTING_SPEECH_MAX_CODES)) {
            TEXT_APPEND(entry, writePosition, TEXT_JAPANESE_MIDDLE_DOT);
            TEXT_APPEND(entry, writePosition, TEXT_JAPANESE_MIDDLE_DOT);
            TEXT_APPEND(entry, writePosition, TEXT_JAPANESE_MIDDLE_DOT);
            TEXT_APPEND(entry, writePosition, TEXT_JAPANESE_FULL_STOP);
            overflow = 1;
            if (decodedCount > HALTING_SPEECH_MAX_CODES)
              code = TEXT_SPACE;
          }
          TEXT_APPEND(entry, writePosition, (u16)code);
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
            TEXT_APPEND(entry, writePosition, (u16)code);
            TEXT_APPEND(entry, writePosition, OPERAND_VALUE(decode(&reader)));
            break;
          case MESSAGE_ARGUMENT_SIGNED_NUMBER:
            value = MessageArgument_Lookup(MESSAGE_ARGUMENT_NUMBER, clear);
            numberText = String_FormatNumber(numberBuffer, value, 0);
            i = numberText - numberBuffer;
            while (i != TEXT_NUMBER_BUFFER_SIZE && numberBuffer[i] != 0) {
              TEXT_APPEND(entry, writePosition, numberBuffer[i]);
              i++;
            }
            break;
          case MESSAGE_ARGUMENT_CLASS_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_CLASS, clear);
            Message_DecodeRaw(argument + MsgClassName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(
                inBattleMessage, name, writePosition, entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_ITEM_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ITEM, clear);
            Message_DecodeRaw((argument & INVENTORY_ITEM_ID_MASK) + MsgItemName,
                              name, MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(
                inBattleMessage, name, writePosition, entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_ABILITY_NAME:
            argument = MessageArgument_Lookup(MESSAGE_ARGUMENT_ABILITY, clear);
            Message_DecodeRaw(argument + MsgAbilityName, name,
                              MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(
                inBattleMessage, name, writePosition, entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_LOCATION_NAME:
            Message_DecodeRaw(
                Area_GetNameIndexFar(
                    MessageArgument_Lookup(MESSAGE_ARGUMENT_LOCATION, clear),
                    1) +
                    MsgDebugAreaName,
                name, MESSAGE_NAME_HALFWORDS);
            writePosition = Message_AppendArticleName(
                inBattleMessage, name, writePosition, entry, 0, 0, 0);
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
            } while (i < UNIT_NAME_SIZE);
            writePosition = Message_AppendArticleName(
                inBattleMessage, name, writePosition, entry, 0, 0, 0);
            break;
          case MESSAGE_ARGUMENT_ACTOR_NAME:
            unit = Unit_GetFar(
                MessageArgument_Lookup(MESSAGE_ARGUMENT_ACTOR, clear));
            // In battle ENEMY_MYSTERY_MAN is called Mystery Man and
            // ENEMY_MYSTERY_WOMAN Mystery Woman (MsgMysteryPersonName and the
            // next).
            if (work->battleMode != WINDOW_FIELD &&
                (unit->id == ENEMY_MYSTERY_MAN ||
                 unit->id == ENEMY_MYSTERY_WOMAN)) {
              message = MsgMysteryPersonName;
              if (unit->id == ENEMY_MYSTERY_WOMAN)
                message++;
              Message_DecodeRaw(message, name, MYSTERY_PERSON_NAME_CODES);
              writePosition = Message_AppendArticleName(
                  inBattleMessage, name, writePosition, entry, 0, 0, 0);
            } else {
              nameBytes = unit->name;
              nameOutput = name;
              j = 0;
              do {
                *nameOutput = *nameBytes;
                j++;
                nameBytes++;
                nameOutput++;
              } while (j < UNIT_NAME_SIZE);
              writePosition = Message_AppendArticleName(
                  inBattleMessage, name, writePosition, entry, 0, 0, 0);
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
            } while (j < UNIT_NAME_SIZE);
            writePosition = Message_AppendArticleName(
                inBattleMessage, name, writePosition, entry, 0, 0, 0);
            break;
          case -1:
            break;
          case MESSAGE_PAGE_BREAK:
          case MESSAGE_LINE_BREAK:
            lineStart = 1;
            // Keep the break in the decoded text.
            // Fall through.
          default:
            TEXT_APPEND(entry, writePosition, (u16)code);
            break;
          }
        }
      }
    }
    TEXT_APPEND(entry, writePosition, (u16)code);
    entry[writePosition] = 0;
    nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
    work->textBufferEnd = (u16)nextPosition;
    HeapSlot_Release(HEAP_SLOT_MESSAGE_DECODER);
    work->lastMessageStart = (u16)start;
  }
  if (clear != 0)
    MessageArgument_ClearAll();
  return start;
}

#undef MYSTERY_PERSON_NAME_CODES
#endif

#undef ARTICLE_MAX_LETTERS
#undef POSSESSIVE_MAX_LETTERS
#undef CHARACTER_CODE_MAX
#undef KANA_FIRST_CODE
#undef HALTING_SPEECH_MAX_CODES
#undef BUTTON_ICON_FIRST_CODE
#undef OPERAND_VALUE
