// Draft, differ 8 of 69, skeleton gap 3 (tla-en, 2026-11-01)
// Twin of ☀️'s Text_DrawMessageAsTiles (window/window/text_tiles.c): same locals,
// statements, Message_Decode(character, 1), terminator, cell formula and 32 * 20
// bound, reading RenderInput's u16 x and y as s16 where ☀️ reads Window's.
// Left: scheduling only. The listing loads the 0x1ff mask before reloading the
// count and uses the shift of offsetY as the load's filler; here the mask load
// is the filler and the shift falls after the first ldrsh. The mid-function pool
// branch then sits two instructions later.
// Tried, none better: dropping the u16 cast, operand order of the mask, shifting
// offsetY in place around the count update, a temporary for the count, reading
// and writing canvas->count directly, *(p + n) and cast spellings of the store,
// u32 and u16 zero, cell's terms reordered. A named mask local is split into
// movs/lsls/adds by -mthumb-split-constants (differ 11), so the listing's pool
// load comes from the & itself.
#include "text_render.h"
#include "window.h"
#include "ram_buffer.h"

s32 Message_Decode(s32 message, s32 mode);

void Text_DrawCharacterAtOffset(s32 character, struct RenderInput *position,
                                  u32 offsetX, u32 offsetY) {
  s32 byteOffset;
  s32 vramAddress;
  u16 *text;
  s32 zero;
  u32 cell;
  struct WindowWork *canvas;
  u16 *counter;

  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  counter = &canvas->count;
  zero = 0;
  *counter = zero;
  Message_Decode(character, 1);

  canvas->entries[*counter] = zero;
  *counter = (u16)((*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1));

  cell = (((s16)position->y + (offsetY >> 3) + 1) << 5) +
         ((s16)position->x + (offsetX >> 3)) + 1;
  if (cell < 0x280U) {
    byteOffset = cell * 2;
    vramAddress = byteOffset + 0x06002000;
    text = canvas->entries;
    Text_DrawStringAsTiles(text, (s32)((u8 *)canvas->tilemap + byteOffset),
                             vramAddress, 7 & offsetX);
  }
}
