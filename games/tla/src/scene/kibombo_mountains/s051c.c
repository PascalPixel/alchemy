#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_0200015c(s32 id);
void Func_020004f4(s32 id, s32 x, s32 z);

void Func_0200051c(s32 unused, s32 cell) {
  if (cell == 20) {
    struct Actor *object = Actor_Get(20);

    if (!Event_GetFlag(0x8e7)) {
      s32 x = object->x >> 20;

      if (x == 4) {
        Func_0200015c(20);
        MapGrid_CopyCellAttributes(3, 28, 1, 1, x, 28);
        Event_SetFlag(0x8e7);
      }
    }
  }
  if (cell == 13) {
    struct Actor *object = Actor_Get(13);

    if (!Event_GetFlag(0x8e8)) {
      s32 x = object->x >> 20;

      if (x == 28) {
        s32 z = object->z >> 20;

        if (z == 30) {
          Func_0200015c(13);
          MapGrid_CopyCellAttributes(32, 30, 1, 2, x, z);
          Event_SetFlag(0x8e8);
        }
      }
    }
  }
  if (cell == 14 || cell == 15) {
    Func_020004f4(14, 8, 17);
    Func_020004f4(15, 8, 17);
  }
  if (cell == 16 || cell == 17) {
    Func_020004f4(16, 22, 33);
    Func_020004f4(17, 22, 33);
  }
}
