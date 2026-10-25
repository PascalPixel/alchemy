// Draft, differ 21 of 77: by the controlled character's height (y plus 32
// pixels past 312 or past 216) picks one of three levels, and when it
// changes copies that level's two 6 by 4 cell attribute blocks to (7, 39)
// and (26, 39) (Func_02005dac, Map_CopyCellAttributeRect in this edition).
// Left: the native shares the second copy of levels 2 and 1 and tests the
// switch cases in another order.
#include "types.h"
#include "object_runtime.h"

extern s32 gData_020073d8;

s32 Func_02005f84(void);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005dac(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02004f3c(void) {
  struct ObjectRuntime *actor = Object_GetById(Func_02005f84());
  s32 level;

  actor->priorityFlags |= 32;
  if (actor->y + 0x200000 >= 0x1380000)
    level = 2;
  else if (actor->y + 0x200000 >= 0xd80000)
    level = 1;
  else
    level = 0;
  if (gData_020073d8 == level)
    return;
  switch (level) {
  case 2:
    Func_02005dac(64, 64, 6, 4, 7, 39);
    Func_02005dac(70, 64, 6, 4, 26, 39);
    break;
  case 1:
    Func_02005dac(64, 68, 6, 4, 7, 39);
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  case 0:
    Func_02005dac(64, 72, 6, 4, 7, 39);
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  }
  gData_020073d8 = level;
}
