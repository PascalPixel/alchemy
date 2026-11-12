#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "../trial_road/trial_road.h"
#include "local.h"

s32 Func_02004e38(void);
struct Actor *Func_02004ef8(s32 *position, s32 mode);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_020046e8(s32 value);
void Func_020049c0(s32 offset, s32 layer);
void Func_0200481c(void);

static __inline__ void Shake(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 28 of 158
   * instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

void Func_0200406c(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Func_02004e38() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02004ef8(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Func_02004d40();
    if (!GameFlag_Test(flag)) {
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(2);
      Func_02004c98(trap, 0);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      Func_020046e8(0);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        leader->motionFlags = 0;
        Sound_Play(185);
        Shake(0x10000, 0x40000, 0x10000);
        Func_020046e8(0);
        wait(8);
        Func_02004c98(trap, 2);
        Func_020049c0(-2, trap->terrainId);
        Func_020046e8(1);
        wait(16);
        Func_020049c0(-2, trap->terrainId);
        wait(4);
        Shake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_0200481c();
        wait(20);
        GameFlag_SetBit(second);
      }
    }
    Func_02004d48();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

struct TrapEntry {
  u16 actor;
  u16 flag;
};

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

void Func_020042e0(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Func_02004df8(id);
}
