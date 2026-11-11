#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "event_inline.h"

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void Func_02004c98(struct Actor *object, s32 a);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_020046e8(s32 value);
void Func_0200481c(void);
void Func_02004934(s32 a, s32 b, s32 c);

static __inline__ void Func_02005838_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 20 of 91
   * instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

void Func_02004200(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_Get(id);
  s32 flag;

  if (entry->actor != 0xffff) {
  next:
    if (entry->actor != id) {
      entry++;
      goto test;
    }
    flag = entry->flag;
    Func_02004d40();
    if (!Event_GetFlag(flag + 1)) {
      Event_PlaySound(185);
      Func_02005838_(0x10000, 0x40000, 0x10000);
      Func_020046e8(0);
      System_Wait(8);
      Func_02004c98(object, 2);
      Func_02004934(-4, OBJECT_BYTE(object, 34), id);
      Func_020046e8(1);
      System_Wait(16);
      Func_02005838_(-1, -1, 0xe666);
      System_Wait(8);
      FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
      System_Wait(2);
      Event_PlaySound(188);
      Func_0200481c();
      System_Wait(20);
      Event_SetFlag(flag);
      Event_SetFlag(flag + 1);
    }
    Func_02004d48();
    return;
  test:
    if (entry->actor != 0xffff)
      goto next;
  }
}
