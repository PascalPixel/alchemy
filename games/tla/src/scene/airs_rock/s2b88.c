#include "types.h"
#include "actor.h"
#include "call.h"

extern s32 gData_02004698;
s32 Func_02003cc8(void);
struct Actor *Actor_GetById(u32 id);
void Func_02003b80(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02002b88(void) {
  struct Actor *actor = Actor_GetById(Func_02003cc8());
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
