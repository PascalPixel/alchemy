#include "types.h"
#include "edition.h"
#include "version.h"
#include "dma.h"
#include "heap_release.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "ram_code.h"
#include "text_reader.h"
#include "window.h"

#define TEXT_DECODER_SLOT 200
#define TEXT_BUFFER_MASK (UI_TEXT_ENTRY_COUNT - 1)

// The European editions keep a second item name table after the first,
// read for the plural forms; its distance from MsgItemName is a message id.
#if LANGUAGE == LANGUAGE_SPANISH
#define ITEM_PLURAL_NAME_OFFSET 469
#else
#define ITEM_PLURAL_NAME_OFFSET 461
#endif

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
// The possessive copied whole, {singular, plural} (articles.c).
struct PossessiveTable {
  s8 *text[2];
};
extern const struct PossessiveTable gLocalizedWord_ItemPossessives;
#endif
#endif

u32 Func_0803cd08(u32 name, u32 clear);
u32 Func_0803aed8(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural,
                  s32 *suffix);
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);
void MessageArgument_ClearAll(void);
u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);
u8 *Unit_Get(s32 unitId);
s32 Scene_GetAreaNameIndexFar(s32 scene, s32 entrance);

s32 Message_Decode(s32 script, s32 clear) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 code = 0;
#if !TLA_PATCH_2
  s32 lineStart = 1;
#endif
  s32 quote = 0;
  u32 start = work->count;
  u32 decodedCount = 0;
  s32 plural = 0;
  s32 running = 1;
  u32 writePosition = start;
  u32 previousCode;
  s32 suffix = SUFFIX_START;
  u16 *entry = work->entries;
  s32 overflow = 0;
#if TLA_PATCH_2
  s32 afterItem;
  s32 lastChar;
  s32 form;
  u16 k;
  s8 *word;
  s8 c;
  s32 cnt;
#endif
  s32 (*decode)(struct TextReader *);
  u32 nextPosition;
  s32 no;
  u32 j;
  u32 i;
  s32 value;
  s32 argument;
  s32 magnitude;
  u32 length;
  u8 *unit;
  u8 *numberText;
  u8 *nameBytes;
  u16 *nameOutput;
  u16 *p;
#if TLA_PATCH_2 && LANGUAGE != LANGUAGE_FRENCH
  struct PossessiveTable possessives;
#endif
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
            Func_0803cd08(3, clear);
            break;
          case 0:
          case 2:
          case 30:
            running = 0;
            break;
          case 22:
            Func_0803cd08(5, clear);
            break;
          case 20:
            decode(messageReader);
            Func_0803cd08(2, clear);
            break;
          case 21:
            Func_0803cd08(4, clear);
            break;
          case 23:
            Func_0803cd08(6, clear);
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
            value = (s32)Func_0803cd08(5, clear);
            magnitude = value;
            if (value < 0)
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
            argument = Func_0803cd08(3, clear);
            Message_DecodeRaw(argument + (s32)MsgClassName, name, 24);
            writePosition = Func_0803aed8(0, name, writePosition, entry, no,
                                          plural, &suffix);
            break;
          case 20:
            no = decode(messageReader) - 1;
#if TLA_PATCH_2
#if LANGUAGE == LANGUAGE_SPANISH
            form = no & 7;
#else
            form = no;
#endif
            argument = Func_0803cd08(2, clear) & TEXT_BUFFER_MASK;
            if (form == 2 || (form == 3 && plural != 0) || form == 4)
              Message_DecodeRaw(
                  argument + (s32)(MsgItemName + ITEM_PLURAL_NAME_OFFSET), name,
                  24);
            else
              Message_DecodeRaw(argument + (s32)MsgItemName, name, 24);
            writePosition = Func_0803aed8(0, name, writePosition, entry, no,
                                          plural, &suffix);
            for (k = 0; name[k] != 0; k++)
              lastChar = (s16)name[k];
            afterItem = (u16)lastChar != '.';
            afterItem = 1 - afterItem;
#else
            argument = Func_0803cd08(2, clear);
            Message_DecodeRaw((argument & TEXT_BUFFER_MASK) + (s32)MsgItemName,
                              name, 24);
            writePosition = Func_0803aed8(0, name, writePosition, entry, no,
                                          plural, &suffix);
#endif
            break;
          case 21:
            argument = Func_0803cd08(4, clear);
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
                Scene_GetAreaNameIndexFar(Func_0803cd08(6, clear), 1) +
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
            nameBytes = Unit_Get(gMainRoutineWork.currentOwner);
            nameOutput = name;
            i = 0;
            do {
              *nameOutput = *nameBytes;
              i++;
              nameBytes++;
              nameOutput++;
            } while (i <= 14);
            writePosition =
                Func_0803aed8(0, name, writePosition, entry, 0, 0, &suffix);
            break;
          case 18:
            no = decode(messageReader) - 1;
            nameBytes = Unit_Get(Func_0803cd08(1, clear));
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition = Func_0803aed8(0, name, writePosition, entry, no,
                                          plural, &suffix);
            break;
          case 17:
            argument = decode(messageReader) - 1;
            nameBytes = Unit_Get(argument);
            nameOutput = name;
            j = 0;
            do {
              *nameOutput = *nameBytes;
              j++;
              nameBytes++;
              nameOutput++;
            } while (j <= 14);
            writePosition =
                Func_0803aed8(0, name, writePosition, entry, 0, 0, &suffix);
            break;
          case 28:
            argument = decode(messageReader) - 1;
            unit = Unit_Get(argument);
            length = decode(messageReader);
            nameBytes = unit;
            nameOutput = name;
#if TLA_PATCH_2
            for (j = 0; j < length; j++)
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
          case 25:
            // DEAD: the item argument is read and dropped.
            Func_0803cd08(2, 0);
#if LANGUAGE == LANGUAGE_FRENCH
            word = (s8 *)gLocalizedWord_ItemPronoun;
#else
            possessives = gLocalizedWord_ItemPossessives;
            word = possessives.text[plural];
#endif
            // The word's first letter is a capital at a sentence's start.
            for (cnt = 0; (c = *word++) != 0;) {
              if (cnt == 0 && suffix == 2)
                c -= 32;
              entry[writePosition] = c;
              writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
              cnt++;
              if (cnt > 15)
                break;
            }
#if LANGUAGE == LANGUAGE_FRENCH
            suffix = 0;
#endif
            break;
          case 27:
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
