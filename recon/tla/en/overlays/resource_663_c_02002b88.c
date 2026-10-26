// Draft, differ 15 of 70 (was 31): by the controlled character's height (y
// plus 32 pixels, in steps of 112) picks one of four levels, and when it
// changes copies that level's 21 by 3 block of cell attributes to (27, 54)
// (Func_02003b80, Map_CopyCellAttributeRect in this edition); overlay 664's
// Func_02004f3c has the same shape. The copies go through Call6, so the
// native's shared call tail and block order come out. Left: the native
// reloads gData_02004698's address for the final store, where this draft
// keeps it in r6 across the switch (and so saves r6); the y load is issued
// before the 32 is built. Measured: the switch inside an if instead of the
// early return, no change.
#include "types.h"
#include "object_runtime.h"
#include "lib/call.h"

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
  if (gData_02004698 != level) {
    switch (level) {
  case 3:
    Call6((void (*)())Func_02003b80, 64, 64, 21, 3, 27, 54);
    break;
  case 2:
    Call6((void (*)())Func_02003b80, 64, 67, 21, 3, 27, 54);
    break;
  case 1:
    Call6((void (*)())Func_02003b80, 64, 70, 21, 3, 27, 54);
    break;
  case 0:
    Call6((void (*)())Func_02003b80, 64, 73, 21, 3, 27, 54);
    break;
    }
    gData_02004698 = level;
  }
}
