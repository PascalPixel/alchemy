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
