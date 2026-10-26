// Near miss, differ 8 of 192: The Broken Seal's Shop_SelectQuantity. Each
// Shop_FillSelector call loads its arguments in another order: the native
// finishes the count in r0 (and sets r0 before r1 for the constants) before
// r1 and r2, where this draft loads r1 and r2 first (an inline wrapper leaves
// the order unchanged). The quantity held in a temporary across the last
// two fills gives the native's registers (the result in r5, the B-button
// test, reused as the zero, in r6; 36 without it).
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "dma.h"
#include "resource.h"
#include "render_input.h"
#include "mem.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "input.h"

extern volatile struct InputState gInput;
extern const u8 gShop_QuantityTiles[];
extern u8 MsgCoins[];

struct UiSprite {
  u8 unk00[24];
  u16 tile : 10;
  u16 flags : 6;
};

s32 Func_081096f8(s32 minimum, s32 maximum, s32 price) {
  u8 *buffer;
  s32 window;
  s32 changed;
  s32 slot;
  s32 count;
  s32 result;
  struct UiSprite *sprite;
  s32 selected;

  buffer = Runtime_AllocateBlock(56, 0x400);
  changed = 1;
  maximum -= minimum;
  result = -1;
  count = 0;
  window = UiWindow_CreateFar(7, 4, 23, 3, 2);
  if (window != 0) {
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
      VramCache_Load(slot, 0x100, 0);
      RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 0,
                             0);
      sprite = (struct UiSprite *)RenderOutput_CreateFar(
          slot, 0x40004000, (struct RenderInput *)window, 32, 0);
      sprite->tile += 4;
      for (;;) {
        if (gInput.pressed & 1) {
          Audio_PlayCue(112);
          result = count + 1;
          break;
        }
        if (gInput.pressed & 2) {
          Audio_PlayCue(113);
          result = -1;
          break;
        }
        if (gInput.repeat & 0x20) {
          Audio_PlayCue(111);
          changed = 1;
          count--;
        }
        if (gInput.repeat & 0x10) {
          Audio_PlayCue(111);
          changed = 1;
          count++;
        }
        if (changed) {
          changed = 0;
          count = __modsi3(count + maximum, maximum);
          Dma_Set(gShop_QuantityTiles, buffer, 0x84000040, REG_DMA3);
          Shop_FillSelector(30, 14, buffer);
          Shop_FillSelector(minimum + maximum, 0, buffer);
          Shop_FillSelector(minimum + count + 1, 10, buffer);
          /* FAKEMATCH(temporary): the quantity in its own local before the
           * last fill; as result = count + 1 after VramCache_Load, 36 of 192
           * instructions differ in tla-en. */
          selected = count + 1;
          Shop_FillSelector(minimum, 2, buffer);
          VramCache_Load(slot, 0x100, buffer);
          result = selected;
          UiText_DrawNumberInWindowFar(result, 2, window, 72, 0);
          UiText_DrawNumberInWindowFar(price * result, 6, window, 88, 0);
          UiText_DrawCharacterAtOffsetFar((s32)MsgCoins, window, 136, 0);
        }
        WaitFrames(1);
      }
      WaitFrames(1);
      UiWork_FinalizeFar(window, 2);
    }
  }
  Runtime_ReleaseHeapBlock(56);
  return result;
}
