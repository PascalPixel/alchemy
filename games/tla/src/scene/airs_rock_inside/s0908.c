#include "types.h"
#include "call.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_020006cc(struct Actor *object);

// Places the objects of a list ended by 0xffff: each entry (three halfwords)
// names an object, the first of its three script choices and the flag that
// picks between them; the object stops moving, loses its solid bit and keeps
// the flag as its action parameter.
void Func_02000908(u16 *list) {
  struct Actor *object;
  s32 flag, base;
  u16 *entry;

  if (*list == 0xffff)
    return;
  entry = list;
  do {
    object = Actor_GetById(*entry);
    base = ((s16 *)entry)[1];
    flag = ((s16 *)entry)[2];
    Actor_SetSingleChildFlags(object, 0);
    {
      s32 choice = GameFlag_Test((u16)flag);

      // The two shifts stay statements: as (u16)base, base loses its rank.
      base <<= 16;
      base = (u32)base >> 16;
      base += base << 1;
      base += choice;
    }
    Func_02005d5c(object, base);
    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    object->action = flag;
    /* FAKEMATCH(wrapper): Func_020006cc called through Call1, the step behind
     * it; as a direct call with the step before it, 2 of 58 instructions differ
     * in tla-en. */
    Call1((void (*)())Func_020006cc, (s32)object);
    entry += 3;
  } while (*entry != 0xffff);
}
