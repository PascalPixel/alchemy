// Draft, differ 47 of 58: places the objects of a list ended by 0xffff: each
// entry (three halfwords) names an object, the first of its three script
// choices and the flag that picks between them; the object stops moving,
// loses its solid bit and keeps the flag as its action parameter.
#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_020006cc(struct Actor *object);

void Func_02000908(u16 *entry) {
  struct Actor *object;
  s32 flag, base;

  if (entry[0] == 0xffff)
    return;
loop:
  object = Actor_GetById(entry[0]);
  flag = ((s16 *)entry)[2];
  base = ((s16 *)entry)[1];
  Actor_SetSingleChildFlags(object, 0);
  base = (u16)base * 3 + GameFlag_Test((u16)flag);
  Func_02005d5c(object, base);
  object->motionFlags = 0;
  object->collisionFlags &= ~1;
  object->action = flag;
  entry += 3;
  Func_020006cc(object);
  if (entry[0] != 0xffff)
    goto loop;
}
