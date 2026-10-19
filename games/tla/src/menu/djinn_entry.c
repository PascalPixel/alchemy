#include "types.h"

extern u8 MsgDjinnName[]; // the first djinni's name

s32 Djinn_IsReadyFar(s32 owner, s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 owner, s32 element, s32 index);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
s32 UiWork_SetParamNibbleFar(s32 color);

/// The state of a packed djinni entry (owner in bits 8-11, element in 5-7,
/// index in 0-4, bit 15 set): 2 when the djinni is neither ready nor active,
/// otherwise whether bit 15 is set. DjinnMenu_DrawPage colours each name by it.
s32 Func_08101a04(u32 entry) {
  s32 owner = (entry & 0xf00) >> 8;
  s32 element = (entry & 0xe0) >> 5;
  s32 index = entry & 31;
  s32 state;

  if (Djinn_IsReadyFar(owner, element, index) ||
      Djinn_IsActiveFar(owner, element, index)) {
    u32 set = entry & 0x8000;

    state = ((0u - set) | set) >> 31;
  } else {
    state = 2;
  }
  return state;
}

/// Draws a packed djinni entry in a window: its name 8 pixels right of (x, y)
/// and its element's icon tile at the tile under (x, y), then resets the text
/// colour. The names run 20 to an element, as ☀️'s MsgDjinnName
/// (games/tbs/src/menu/menu.c).
void Func_08101a54(s32 window, s32 x, s32 y, s32 entry) {
  s32 element = (entry & 0xe0) / 32;

  UiText_DrawCharacterAtOffsetFar(
      (s32)MsgDjinnName + element * 20 + (entry & 31), window, x + 8, y);
  UiWindow_SetTilemapEntryFar(window, element + 0x5001, x / 8, y / 8, 0);
  UiWork_SetParamNibbleFar(15);
}
