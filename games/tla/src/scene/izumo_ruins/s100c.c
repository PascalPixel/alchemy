#include "izumo_ruins.h"

void Func_0200100c(u16 *list) {
  struct Actor *object;
  s32 flag;

  while (list[0] != 0xffff) {
    object = Actor_Get(list[0]);
    flag = (s16)list[2];
    Actor_SetSingleChildFlags(object, 0);
    Func_02001428(object, Event_GetFlag((u16)flag) + 1);
    object->motionFlags = 0;
    object->collisionFlags &= 0xfe;
    object->action = flag;
    Func_02000f30(object);
    list += 3;
  }
}
