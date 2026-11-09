#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 Func_020059c0(void);
struct Actor *Func_02005a28(s32 *position, s32 mode);
void Func_02005880(void);
void Func_02005888(void);
void Func_020057c8(struct Actor *object, s32 mode);
void Func_0200480c(struct Actor *object, struct Actor *to);
void ObjectVisual_CopyAttributes(s32 id, s32 from);
void Func_02004a94(s32 value);
void Func_02004d6c(s32 offset, s32 layer);
void Func_02004bc8(void);

void Func_02004828(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Func_020059c0() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02005a28(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Func_02005880();
    if (!GameFlag_Test(flag)) {
      Func_020057c8(partner, 1);
      Func_0200480c(partner, trap);
      wait(1);
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(2);
      Func_020057c8(partner, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_02004a94(0);
      Func_0200480c(trap, partner);
      wait(1);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        Func_020057c8(partner, 0);
        Func_0200480c(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        wait(8);
        Func_020057c8(partner, 2);
        Func_02004d6c(-2, trap->terrainId);
        Func_02004a94(1);
        wait(16);
        Func_02004d6c(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02004bc8();
        wait(20);
        GameFlag_SetBit(second);
      }
    }
    Func_02005888();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}
