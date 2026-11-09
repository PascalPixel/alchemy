#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 Func_020030f8(void);
struct Actor *Func_02003188(s32 *position, s32 mode);
void Func_02003048(void);
void Func_02003050(void);
void Func_02002f80(struct Actor *object, s32 mode);
void Func_02002748(struct Actor *object, struct Actor *to);
void ObjectVisual_CopyAttributes(s32 id, s32 from);
void Func_020029d0(s32 value);
void Func_02002ca8(s32 offset, s32 layer);
void Func_02002b04(void);

static __inline__ void Shake(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_02002764(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Func_020030f8() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02003188(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Func_02003048();
    if (!GameFlag_Test(flag)) {
      Func_02002f80(partner, 1);
      Func_02002748(partner, trap);
      wait(1);
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(2);
      Func_02002f80(partner, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_020029d0(0);
      Func_02002748(trap, partner);
      wait(1);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        Func_02002f80(partner, 0);
        Func_02002748(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(185);
        Shake(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Func_02002f80(partner, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        Shake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02002b04();
        wait(20);
        GameFlag_SetBit(second);
      }
    }
    Func_02003050();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}
