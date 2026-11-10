#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02003b8c(void);
struct Actor *Func_02003c2c(s32 *position, s32 flags);
void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003a54(struct Actor *object, s32 mode);
void Func_020034ac(s32 mode);
void Func_02003784(s32 level, s32 layer);
void Func_020035e0(void);

void Func_02002e30(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target != NULL)
    goto test;
  return;
body:
  {
    s32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02003aec();
      if (!GameFlag_Test(flag)) {
        Sound_Play(125);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        wait(2);
        Func_02003a54(target, 0);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        Func_020034ac(0);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        leader->motionFlags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(target, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
    }
    Func_02003af4();
    return;
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
