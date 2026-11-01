// Draft, differ 55 of 284, skeleton gap 13 (tla-en, 2026-11-01)
// ☀️'s EV08_TALK (debug/level.c) for overlay 6B7, the debug room's item giver,
// with ⚓️'s 512 item slots (the modulus a variable as in ☀️, so it stays a
// __modsi3 call), message 607 as MsgItemName (pooled, so a symbol, as 146 is
// MsgItemPlainName; both declared by msg_ids.h) and talk.c's
// Input_PressedFenced for the A test.
// Left is one allocation pair, and the whole skeleton gap follows from it:
// window and the KEY_L mask, which CSE reuses as the first modulus of the Left
// branch, swap r7 and r8. The native gives window r7 and the mask r8 (the
// mov r8, r2 after ands is reload filling a high register). lreg ranks the
// mask 6 refs over 30 insns (0.40) above window's 19 over 214 (0.36); window
// outranks it from about 217 against 36, three more insns between the KEY_L
// test and the first modulus that survive combine and leave at allocation.
// The fence is needed only because PRE (GCSE) otherwise drops the B test's
// load of gInput's address, inserting it on the edge from the A block.
// Tried: the windows declared the other way, a goto loop (140), one-pass
// blocks around the redraw block or its item branch (84, 86), while (redraw)
// (80), the window pinned to r7 (169), step as s32 or read through its 24-bit
// shift (55, 55), a local for the sign-extended step (85), an index temporary
// at the modulus (188), the Left body reordered (59), the fence without its
// operand (63) or before the B test (55), plain or inline A tests (60), four
// minutes of alchemy permute (its best moves calls between branches).
#include "types.h"
#include "input.h"
#include "item.h"
#include "dma.h"
#include "edition.h"
#include "text/msg_ids.h"

#define ITEM_COUNT 512

extern const u8 gData_02000748[];
extern const u8 gData_02000754[];
extern const u8 gData_0200076c[];

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void Text_DrawMessage(s32 msg, void *window, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(void *window);
void WindowSprite_ClearList(void *window);
void Text_DrawStringInWindow(const u8 *text, void *window, s32 x, s32 y);
void Text_DrawNumberAtOffset(s32 value, s32 format, void *window, s32 x,
                               s32 y);
void Text_DrawCharacterAtOffset(s32 character, void *window, s32 x, s32 y);
void Window_Close(void *window, s32 mode);
void wait(s32 frames);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);
struct ItemDefinition *Func_020006bc(s32 item);
void Func_0200073c(s32 cue);

static inline u32 Input_PressedFenced(u32 mask) {
  u32 keys = gInput.pressed & mask;

  /* FAKEMATCH(barrier): the memory clobber makes the next test load gInput's
   * address again; as a plain test, 60 of 284 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(keys) : : "memory");
  return keys;
}

void Func_020002cc(void) {
  void *window;
  void *details;
  s32 item;
  s32 redraw;
  s32 index;
  s32 count;
  s8 step;

  count = ITEM_COUNT;
  step = 0;
  Func_0200073c(112);
  item = 1;
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, 2);
#if !EDITION_INTERNATIONAL
  details = Window_Create(0, 8, 13, 10, 2);
#else
  details = Window_Create(0, 9, 18, 10, 2);
#endif
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
          (volatile u32 *)0x040000d4);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      item = (item + ITEM_COUNT) % count;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gData_02000748, window, 0, 0);
      Text_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gData_02000754, window, 0, 32);
        index = item & 0x1ff;
        Func_020006bc(index);
        Text_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
        Text_DrawMessage(index + (s32)MsgItemPlainName, window, 0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        Text_DrawStringInWindow(gData_0200076c, window, 0, 32);
      }
    }
    if (Input_PressedFenced(1)) {
      if (PartyInventory_Add(item) == -1) {
        Func_0200073c(113);
        break;
      }
      Func_0200073c(175);
    }
    if (gInput.pressed & 2) {
      Func_0200073c(113);
      break;
    }
    if (gInput.repeat & 64) {
      step = -1;
      item--;
      redraw = 1;
      Func_0200073c(111);
    }
    if (gInput.repeat & 128) {
      step = 1;
      item++;
      redraw = 1;
      Func_0200073c(111);
    }
    if (gInput.repeat & 16) {
      step = 1;
      item += 10;
      redraw = 1;
      Func_0200073c(111);
    }
    if (gInput.repeat & 32) {
      step = -1;
      item -= 10;
      redraw = 1;
      Func_0200073c(111);
    }
    if (gInput.repeat & 256) {
      step = 1;
      item += 30;
      redraw = 1;
      Func_0200073c(111);
    }
    if (gInput.repeat & 512) {
      step = -1;
      item -= 30;
      redraw = 1;
      Func_0200073c(111);
    }
    if (step == -1) {
      item = (item + ITEM_COUNT) % count;
      goto check_back;
    back:
      item = (item + ITEM_COUNT - 1) % count;
    check_back:
      if (Func_020006bc(item & 0x1ff)->icon == 0)
        goto back;
    }
    if (step == 1) {
      item = (item + ITEM_COUNT) % count;
      goto check_ahead;
    ahead:
      item = (item + ITEM_COUNT + 1) % count;
    check_ahead:
      if (Func_020006bc(item & 0x1ff)->icon == 0)
        goto ahead;
    }
    step = 0;
    wait(1);
  }
  WindowSprite_RedrawSavedRect(window);
  wait(1);
  Window_Close(window, 1);
  Window_Close(details, 1);
}
