#include "types.h"

extern u8 MsgDjinnName[]; // the first djinni's name

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
s32 UiWork_SetParamNibbleFar(s32 color);

// Draws a packed djinni entry in a window: its name 8 pixels right of (x, y)
// and its element's icon tile at the tile under (x, y), then resets the text
// colour. The names run 20 to an element, as ☀️'s MsgDjinnName
// (games/tbs/src/menu/menu.c).
void Func_08101a54(s32 window, s32 x, s32 y, s32 entry) {
  s32 element = (entry & 0xe0) / 32;

  UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnName + element * 20 +
                                      (entry & 31),
                                  window, x + 8, y);
  UiWindow_SetTilemapEntryFar(window, element + 0x5001, x / 8, y / 8, 0);
  UiWork_SetParamNibbleFar(15);
}
