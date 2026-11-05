// Draft, differ 94 (tla-en, 2026-11-05). Func_02000848 of overlay 6ba.
// A rise from 70: the barrier is gone, the inner loops plain, step a u8
// stored as 255 and read (s8), which gives the listing's shifts. Left: rank
// only: item r5, gInput r6, redraw r8, step sl, against the native gInput r5,
// item r6, step r8, redraw sl.
#include "types.h"
#include "call.h"
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

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void WindowSprite_PrepareForRedraw(void *window);
void Text_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void Text_DrawNumberAtOffset(s32 value, s32 digits, void *window, s32 x,
                               s32 y);
void Text_DrawCharacterAtOffset(s32 message, void *window, s32 x, s32 y);
void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);
void Window_Close(void *window, s32 release);
void wait(s32 frames);
struct ItemSummary *Func_02000d48(s32 item);
void Func_02000e78(s32 cue);

#define ITEM_COUNT 500

void Func_02000848(void) {
  void *window;
  void *details;
  s32 item;
  s32 redraw;
  s32 entryCount;
  s8 step;

  entryCount = ITEM_COUNT;
  step = 0;
  window = Window_Create(0, 0, 30, 7, 2);
  details = Window_Create(0, 8, 13, 10, 2);
  item = 1;
  redraw = 1;
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
  Func_02000e78(112);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      WindowSprite_PrepareForRedraw(window);
      WindowSprite_PrepareForRedraw(details);
      Text_DrawStringInWindow(gData_02000e84, window, 0, 0);
      Text_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gData_02000e90, window, 0, 32);
        Text_DrawCharacterAtOffset((s32)(item + MsgItemName), window, 120, 0);
        Text_DrawMessage((s32)(item + MsgItemPlainName), window, 0, 16);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        Text_DrawStringInWindow(gData_02000ea8, window, 0, 32);
      }
    }
    wait(1);
    if (gInput.pressed & 1) {
      if (PartyInventory_Add(item) == -1) {
        Func_02000e78(113);
        break;
      }
      Func_02000e78(175);
    }
    if (gInput.pressed & 2) {
      Func_02000e78(113);
      break;
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
      for (item = (item + ITEM_COUNT) % entryCount;
           Func_02000d48(item & 0x1ff)->icon == 0;
           item = (item + ITEM_COUNT - 1) % entryCount)
        ;
    }
    if (step == 1) {
      for (item = (item + ITEM_COUNT) % entryCount;
           Func_02000d48(item & 0x1ff)->icon == 0;
           item = (item + ITEM_COUNT + 1) % entryCount)
        ;
    }
    step = 0;
  }
  Window_Close(window, 2);
  Window_Close(details, 2);
}
