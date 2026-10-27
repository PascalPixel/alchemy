// Draft, differ 59 of 284: ☀️'s EV08_TALK (debug/level.c) for overlay 6B7, the
// debug room's item giver, with ⚓️'s 512 item slots (the modulus a variable
// as in ☀️, so it stays a __modsi3 call), message 607 as MsgItemName (pooled,
// so a symbol, as 146 is MsgItemPlainName; not yet named in the catalogs) and
// talk.c's Input_PressedFenced for the A test (without it the B test reuses
// the A test's gInput address: differ 64). Left: global.c gives the window r8
// and the KEY_L mask, which CSE reuses as the first modulus of the Left branch,
// r7; the native swaps them (lreg: the mask 6 refs over 30 insns ranks above
// the window's 19 over 214). Tried: the windows declared the other way, a
// goto main loop (144), one-pass blocks around the redraw block or its item
// branch (84, 86), the window pinned to r7 (169), four minutes of alchemy
// permute (its best moves calls between branches).
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
extern char MsgItemName[];

void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawResource(s32 msg, void *window, s32 x, s32 y);
void RenderOutput_RedrawSavedRect(void *window);
void RenderOutput_ClearList(void *window);
void UiText_DrawStringInWindow(const u8 *text, void *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, void *window, s32 x,
                               s32 y);
void UiText_DrawCharacterAtOffset(s32 character, void *window, s32 x, s32 y);
void UiWork_Finalize(void *window, s32 mode);
void WaitFrames(s32 frames);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);
struct ItemDefinition *Func_020006bc(s32 item);
void Func_0200073c(s32 cue);

static inline u32 Input_PressedFenced(u32 mask) {
  u32 keys = gInput.pressed & mask;

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
  window = UiWindow_Create(0, 0, 30, 7, 2);
#if !EDITION_INTERNATIONAL
  details = UiWindow_Create(0, 8, 13, 10, 2);
#else
  details = UiWindow_Create(0, 9, 18, 10, 2);
#endif
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
          (volatile u32 *)0x040000d4);
  WaitFrames(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      item = (item + ITEM_COUNT) % count;
      RenderOutput_RedrawSavedRect(window);
      RenderOutput_ClearList(window);
      UiText_DrawStringInWindow(gData_02000748, window, 0, 0);
      UiText_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        UiText_DrawStringInWindow(gData_02000754, window, 0, 32);
        index = item & 0x1ff;
        Func_020006bc(index);
        UiText_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
        UiText_DrawResource(index + (s32)MsgItemPlainName, window, 0, 16);
        RenderOutput_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        UiText_DrawStringInWindow(gData_0200076c, window, 0, 32);
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
    WaitFrames(1);
  }
  RenderOutput_RedrawSavedRect(window);
  WaitFrames(1);
  UiWork_Finalize(window, 1);
  UiWork_Finalize(details, 1);
}
