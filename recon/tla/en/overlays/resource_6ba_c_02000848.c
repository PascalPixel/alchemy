// Draft, differ 141 of 258: the debug room's item picker, from ☀️'s debug
// level.c EV08_TALK. The native has ⚓️'s 500 items, keys tested down before up,
// no wrap at redraw, and message 607 and 146 as pool symbols (named MsgItemName
// and MsgItemPlainName after ☀️; the ⚓️ catalogs leave them unnamed so far).
// Measured: as a while loop loop.c hoists 500 into r9 (differ 199); the goto
// loop keeps it in place. Left: the native loads gInput's address once for the
// A test and again after it (r3, then r5 for the rest); here CSE keeps the
// first in r6 for all, so item, redraw and step take r5, r8 and r10 for the
// native's r6, r10 and r8. A one-pass block around the A test: 167.
#include "types.h"
#include "input.h"
#include "dma.h"
#include "io_reg.h"

extern const u8 gData_02000e84[];
extern const u8 gData_02000e90[];
extern const u8 gData_02000ea8[];
extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];

struct ItemSummary {
  u8 unk00[6];
  u16 icon;
};

void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void RenderOutput_PrepareForRedraw(void *window);
void UiText_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 digits, void *window, s32 x,
                               s32 y);
void UiText_DrawCharacterAtOffset(s32 message, void *window, s32 x, s32 y);
void UiText_DrawResource(s32 message, void *window, s32 x, s32 y);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);
void UiWork_Finalize(void *window, s32 release);
void WaitTicks(s32 ticks);
struct ItemSummary *Func_02000d48(s32 item);
void Func_02000e78(s32 cue);

#define ITEM_COUNT 500

void Func_02000848(void) {
  void *window;
  void *details;
  s32 item;
  s32 redraw;
  s8 step;

  step = 0;
  window = UiWindow_Create(0, 0, 30, 7, 2);
  details = UiWindow_Create(0, 8, 13, 10, 2);
  item = 1;
  redraw = 1;
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
  Func_02000e78(112);
  WaitTicks(1);
loop:
  {
    if (redraw) {
      redraw = 0;
      RenderOutput_PrepareForRedraw(window);
      RenderOutput_PrepareForRedraw(details);
      UiText_DrawStringInWindow(gData_02000e84, window, 0, 0);
      UiText_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        UiText_DrawStringInWindow(gData_02000e90, window, 0, 32);
        UiText_DrawCharacterAtOffset((s32)(item + MsgItemName), window, 120, 0);
        UiText_DrawResource((s32)(item + MsgItemPlainName), window, 0, 16);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        UiText_DrawStringInWindow(gData_02000ea8, window, 0, 32);
      }
    }
    WaitTicks(1);
    if (gInput.pressed & 1) {
      if (PartyInventory_Add(item) == -1) {
        Func_02000e78(113);
        goto done;
      }
      Func_02000e78(175);
    }
    if (gInput.pressed & 2) {
      Func_02000e78(113);
      goto done;
    }
    if (gInput.repeat & 0x80) {
      step = 1;
      item++;
      redraw = 1;
      Func_02000e78(111);
    }
    if (gInput.repeat & 0x40) {
      step = -1;
      item--;
      redraw = 1;
      Func_02000e78(111);
    }
    if (gInput.repeat & 0x10) {
      step = 1;
      item += 10;
      redraw = 1;
      Func_02000e78(111);
    }
    if (gInput.repeat & 0x20) {
      step = -1;
      item -= 10;
      redraw = 1;
      Func_02000e78(111);
    }
    if (gInput.repeat & 0x100) {
      step = 1;
      item += 30;
      redraw = 1;
      Func_02000e78(111);
    }
    if (gInput.repeat & 0x200) {
      step = -1;
      item -= 30;
      redraw = 1;
      Func_02000e78(111);
    }
    if (step == -1) {
      item = (item + ITEM_COUNT) % ITEM_COUNT;
      goto check_back;
    back:
      item = (item + ITEM_COUNT - 1) % ITEM_COUNT;
    check_back:
      if (Func_02000d48(item & 0x1ff)->icon == 0)
        goto back;
    }
    if (step == 1) {
      item = (item + ITEM_COUNT) % ITEM_COUNT;
      goto check_ahead;
    ahead:
      item = (item + ITEM_COUNT + 1) % ITEM_COUNT;
    check_ahead:
      if (Func_02000d48(item & 0x1ff)->icon == 0)
        goto ahead;
    }
    step = 0;
    goto loop;
  }
done:
  UiWork_Finalize(window, 2);
  UiWork_Finalize(details, 2);
}
