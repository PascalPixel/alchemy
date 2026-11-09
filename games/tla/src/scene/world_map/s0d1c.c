#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "call.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);

void Func_02000d1c(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  if (Value3((s32 (*)())Func_02004c38, 19, 0x198, -1) == 0)
    Event_ClearFlag(0xa77);
  Func_02004c28();
}

void Func_02000d4c(void) {
  struct Actor *object;
  u8 *animation;
  s32 a, id;

  Actor_ShowItemIconById(19, 0x198);
  object = Actor_Get(19);
  animation = object->animation;
  animation[17] |= 1;
  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  a = 0x4000;
  id = 19;
  /* FAKEMATCH: the native builds the last argument, then the id, before it
   * loads the two positions; a direct call and a forwarding inline load the
   * positions first. */
  __asm__ volatile("" : "+r"(a), "+r"(id));
  Actor_SetPositionAndFacing(id, PIXELS(10168), PIXELS(9648), a);
  Event_SetFlag(0xa77);
}
