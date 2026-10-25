// Draft, differ 31 of 70: by the controlled character's height (y plus 32
// pixels, in steps of 112) picks one of four levels, and when it changes
// copies that level's 21 by 3 block of cell attributes to (27, 54)
// (Func_02003b80, Map_CopyCellAttributeRect in this edition); overlay
// 664's Func_02004f3c has the same shape. Left: the native shares the call
// of levels 3, 2 and 1 and orders the switch's tests and blocks otherwise.
#include "types.h"
#include "object_runtime.h"

extern s32 gData_02004698;
s32 Func_02003cc8(void);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02003b80(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02002b88(void) {
  struct ObjectRuntime *actor = Object_GetById(Func_02003cc8());
  s32 level;

  actor->priorityFlags |= 32;
  level = ((actor->y + 0x200000) >> 16) / 112;
  if (gData_02004698 == level)
    return;
  switch (level) {
  case 3:
    Func_02003b80(64, 64, 21, 3, 27, 54);
    break;
  case 2:
    Func_02003b80(64, 67, 21, 3, 27, 54);
    break;
  case 1:
    Func_02003b80(64, 70, 21, 3, 27, 54);
    break;
  case 0:
    Func_02003b80(64, 73, 21, 3, 27, 54);
    break;
  }
  gData_02004698 = level;
}
