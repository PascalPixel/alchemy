// Draft, differ 18 of 831, skeleton gap 9 (tla-en, 2026-11-01)
// Message_Decode, The Broken Seal's English function (decode.c) with this
// game's additions: code 28 writes the first n characters of a unit's name,
// keeping a kana mark (222 or 223) with its character, and the em dash is code
// 176, not 24. Allocation, stack frame and every branch agree.
// Left: three scheduling differences, 6 lines each. In the entry block the
// constant 0x1342 (work->count) starts later than here; the halting-speech
// store of 5 and the spacing store of 222 each put the first two instructions
// of the ring mask after the shifted index, and the strh before the increment.
// sched2 here gives those insns the priorities that place them where they are,
// and the spellings of the store and of the increment tried leave them there.
// The message-slot base must stay a separate local read after the DMA (slots):
// one local shared with the read of windowTiles gets constant-propagated, loses
// its register and shifts every reload after it.
// The earlier contents of this file live on elsewhere: Text_DrawMessage in
// games/tla/src/window/f041fa4.c, Text_DrawStringAtOffset in 080420fc.c,
// Text_DrawPrefixedNumberAtOffset in 08042274.c, Text_DrawString in
// games/tla/src/window/draw_string.c, the number writers and
// Text_DrawStringInWindow in games/tla/src/window/render.c.

#include "types.h"
#include "edition.h"
#include "dma.h"
#include "heap_release.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "ram_code.h"
#include "text_reader.h"
#include "window.h"

#define TEXT_DECODER_SLOT 200
#define TEXT_BUFFER_MASK (UI_TEXT_ENTRY_COUNT - 1)

extern const u8 Text_DecodeSymbolCode[];
extern u8 Text_DecodeSymbolCodeSize[];
extern u8 MsgClassName[], MsgItemName[], MsgAbilityNothing[], MsgDebugAreaName[];

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
  s32 lineStart = 1;
  s32 quote = 0;
  u32 start = work->count;
  u32 decodedCount = 0;
  s32 plural = 0;
  s32 running = 1;
  u32 writePosition = start;
  u32 previousCode;
  s32 suffix = 0;
  u16 *entry = work->entries;
  s32 overflow = 0;
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
            argument = Func_0803cd08(2, clear);
            Message_DecodeRaw((argument & TEXT_BUFFER_MASK) + (s32)MsgItemName, name, 24);
            writePosition = Func_0803aed8(0, name, writePosition, entry, no,
                                          plural, &suffix);
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
                Scene_GetAreaNameIndexFar(Func_0803cd08(6, clear), 1) + (s32)MsgDebugAreaName,
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
            writePosition = Func_0803aed8(0, name, writePosition, entry, 0, 0,
                                          &suffix);
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
            writePosition = Func_0803aed8(0, name, writePosition, entry, 0, 0,
                                          &suffix);
            break;
          case 28:
            argument = decode(messageReader) - 1;
            unit = Unit_Get(argument);
            length = decode(messageReader);
            nameBytes = unit;
            nameOutput = name;
            for (j = 0; j < length; j++) {
              *nameOutput++ = *nameBytes++;
              if ((u8)(*nameBytes + 34) <= 1)
                *nameOutput++ = *nameBytes++;
            }
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
