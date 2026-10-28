// Draft, differ 77 of 191: overlay 697's copy of the function drafted in
// resource_692_c_0200447c.c (overlay 692's Func_0200447c, differ 77 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_020059c0(void);
struct Actor *Func_02005a28(s32 *position, s32 mode);
void Func_02005880(void);
void Func_02005888(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02005a68(s32 sound);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Task_Wait(s32 frames);
void Func_020057c8(struct Actor *object, s32 mode);
void Func_0200480c(struct Actor *object, struct Actor *to);
void ObjectVisual_CopyAttributes(s32 id, s32 from);
void Func_02004a94(s32 value);
void Func_02005838(s32 a, s32 b, s32 c);
void Func_02004d6c(s32 offset, s32 layer);
void Func_02004bc8(void);

static __inline__ void Shake(s32 a, s32 b, s32 c) { Func_02005838(a, b, c); }

void Func_02004828(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Func_020059c0() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02005a28(position, 0);
  if (trap == 0)
    return;
  goto test;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Func_02005880();
    if (!GameFlag_Test(flag)) {
      Func_020057c8(partner, 1);
      Func_0200480c(partner, trap);
      Task_Wait(1);
      Func_02005a68(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      Task_Wait(2);
      Func_020057c8(partner, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Task_Wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      Task_Wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_02004a94(0);
      Func_0200480c(trap, partner);
      Task_Wait(1);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        Func_020057c8(partner, 0);
        Func_0200480c(partner, trap);
        Task_Wait(1);
        leader->motionFlags = 0;
        Func_02005a68(185);
        Shake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        Task_Wait(8);
        Func_020057c8(partner, 2);
        Func_02004d6c(-2, trap->terrainId);
        Func_02004a94(1);
        Task_Wait(16);
        Func_02004d6c(-2, trap->terrainId);
        Task_Wait(4);
        Shake(-1, -1, 0xe666);
        Task_Wait(8);
        leader->motionFlags = 3;
        Task_Wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        Task_Wait(2);
        Func_02005a68(188);
        Func_02004bc8();
        Task_Wait(20);
        GameFlag_SetBit(second);
      }
    }
    Func_02005888();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}
