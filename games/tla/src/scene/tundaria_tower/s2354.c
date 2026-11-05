#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_020030f8(void);
struct Actor *Func_02003188(s32 *position, s32 mode);
void Func_02003048(void);
void Func_02003050(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_020031a8(s32 sound);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void wait(s32 frames);
void Func_02002f80(struct Actor *object, s32 mode);
void Func_020029d0(s32 value);
void Func_02003000(s32 a, s32 b, s32 c);
void Func_02002ca8(s32 offset, s32 layer);
void Func_02002b04(void);

static __inline__ void Shake(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 28 of 158
   * instructions differ in tla-en. */
  Func_02003000(a, b, c);
}

void Func_02002354(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Func_020030f8() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02003188(position, 0);
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
    Func_02003048();
    if (!GameFlag_Test(flag)) {
      Func_020031a8(125);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(2);
      Func_02002f80(trap, 0);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      Func_020029d0(0);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        leader->motionFlags = 0;
        Func_020031a8(185);
        Shake(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Func_02002f80(trap, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        Shake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Func_020031a8(188);
        Func_02002b04();
        wait(20);
        GameFlag_SetBit(second);
      }
    }
    Func_02003050();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}
