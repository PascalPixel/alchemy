#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "input.h"
#include "system.h"
#include "uiflow.h"
#include "text_render.h"

extern s16 gPortrait_CharacterMap[];
extern s16 gPortrait_SpriteMap[];

struct RenderInput *Func_0803d3c0(s32 x, s32 y, s32 width, s32 height);
struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void WindowSprite_PrepareForRedraw(struct RenderInput *window);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);
void Text_DrawNumberInWindow(s32 value, s32 width, struct RenderInput *output,
                               s32 x, s32 y);

extern u8 MsgTestName[];

/// A debug browser of the portraits: left and right step through
/// gPortrait_CharacterMap and then gPortrait_SpriteMap, L and R by ten, each
/// drawn with its number and its name from MsgTestName; A or B leaves.
s32 Func_0804e3f4(void) {
  struct WindowWork *work;
  struct RenderInput *portrait;
  s32 redraw;
  struct RenderInput *window;
  s32 count;
  s32 total;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancel;
  u32 slotOffset;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  redraw = 1;
  portrait = Func_0803d3c0(0, 0, 10, 5);
  window = Window_Create(10, 10, 14, 3, 2);
  index = 0;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  count = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  total = count + i;
  /* FAKEMATCH(dead): a zero from the count in the cancel mask, as in The
   * Broken Seal's twin; as cancel = 2, 30 of 180 instructions differ in
   * tla-en. */
  cancel = ((u32)(u16)count >> 16) | 2;
  /* FAKEMATCH(temporary): the slot's offset defined before the loop; as the
   * offset written at its load, 10 of 180 instructions differ in tla-en. */
  slotOffset = offsetof(struct WindowWork, outputIds[1]);
  while (1) {
    if (gInput.repeat & 0x20) {
      redraw = 1;
      index--;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      index++;
    }
    if (gInput.repeat & 0x200) {
      redraw = 1;
      index -= 10;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      index += 10;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & cancel)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + total) % total;
      WindowSprite_PrepareForRedraw(window);
      if (index < count)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - count) * 2 + 1] + 128;
      {
        /* FAKEMATCH(temporary): a word temporary for the slot and the
         * palette in a variable; as the slot assigned directly and 15
         * passed, 8 of 180 instructions differ in tla-en. */
        s32 pal;
        s32 loaded = *(u16 *)((u8 *)work + slotOffset);
        pal = 15;
        slot = loaded;
        UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
      }
      Text_DrawNumberInWindow(index, 2, window, 0, 0);
      Text_DrawCharacterAtOffset(index + (s32)MsgTestName, window, 24, 0);
    }
    wait(1);
  }
  Window_Close((struct Window *)window, 2);
  Window_Close((struct Window *)portrait, 2);
  wait(1);
  return 0;
}
