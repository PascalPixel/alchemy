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
#include "edition.h"

// The quantity window: outside the English edition the count sits further left
// and the price and coins label move with their words; the Spanish, French and
// Italian windows are wider.
#if LANGUAGE == LANGUAGE_ENGLISH
#define QUANTITY_WINDOW_X 7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X 72
#define QUANTITY_PRICE_X 88
#define QUANTITY_COINS_X 136
#elif LANGUAGE == LANGUAGE_GERMAN
#define QUANTITY_WINDOW_X 7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X 64
#define QUANTITY_PRICE_X 80
#define QUANTITY_COINS_X 128
#elif LANGUAGE == LANGUAGE_SPANISH
#define QUANTITY_WINDOW_X 5
#define QUANTITY_WINDOW_WIDTH 25
#define QUANTITY_COUNT_X 64
#define QUANTITY_PRICE_X 104
#define QUANTITY_COINS_X 160
#else
#define QUANTITY_WINDOW_X 5
#define QUANTITY_WINDOW_WIDTH 25
#define QUANTITY_COUNT_X 64
#define QUANTITY_PRICE_X 88
#define QUANTITY_COINS_X 144
#endif

extern volatile struct InputState gInput;
extern const u8 gShop_QuantityTiles[];
extern u8 MsgCoins[];

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Shop_FillSelector(s32 count, s32 selector, u8 *base);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWork_FinalizeFar(s32 window, s32 style);

struct UiSprite {
  u8 unk00[24];
  u16 tile : 10;
  u16 flags : 6;
};

/// Lets the player pick a quantity with left and right in a small window that
/// shows it and its price in coins; returns it, or -1 when B cancels. The
/// Broken Seal's Shop_SelectQuantity.
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
  window =
      UiWindow_CreateFar(QUANTITY_WINDOW_X, 4, QUANTITY_WINDOW_WIDTH, 3, 2);
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
          count = (count + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, buffer, 0x84000040, REG_DMA3);
          Shop_FillSelector(30, 14, buffer);
          Shop_FillSelector(minimum + maximum, 0, buffer);
          Shop_FillSelector(minimum + count + 1, 10, buffer);
          /* FAKEMATCH(temporary): the quantity in its own local before the
           * last fill; as result = count + 1 after VramCache_Load, 28 of 192
           * instructions differ in tla-en. */
          selected = count + 1;
          Shop_FillSelector(minimum, 2, buffer);
          VramCache_Load(slot, 0x100, buffer);
          result = selected;
          UiText_DrawNumberInWindowFar(result, 2, window, QUANTITY_COUNT_X, 0);
          UiText_DrawNumberInWindowFar(price * result, 6, window,
                                       QUANTITY_PRICE_X, 0);
          UiText_DrawCharacterAtOffsetFar((s32)MsgCoins, window,
                                          QUANTITY_COINS_X, 0);
        }
        WaitTicks(1);
      }
      WaitTicks(1);
      UiWork_FinalizeFar(window, 2);
    }
  }
  Runtime_ReleaseHeapBlock(56);
  return result;
}
