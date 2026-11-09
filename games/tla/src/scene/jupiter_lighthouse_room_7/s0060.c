#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"

void Func_02000060(s32 mode) {
  if (mode == 1) {
    Sound_Play(133);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    wait(6);
  }
  Func_0802b450(81, 42, 4, 3, 76, 35);
  Map_CopyCellAttributeRect(12, 41, 4, 1, 12, 35);
}

void Func_020000b4(void) {
  Sound_Play(134);
  Func_0802b450(76, 42, 4, 3, 76, 35);
  wait(6);
  Func_0802b450(71, 42, 4, 3, 76, 35);
  Map_CopyCellAttributeRect(12, 36, 4, 1, 12, 35);
}

void Func_02000100(void) {
  struct Actor *object = Actor_GetById(16);

  if (!Event_GetFlag(0xa1b) && object->x >> 20 == 8 && object->z >> 20 == 37) {
    GameFlag_SetBit(0xa1b);
    Func_02000060(1);
  }
}
