#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 Func_02003b8c(void);
struct Actor *Func_02003c2c(s32 *position, s32 flags);
void Func_02003aec(void);
s32 Func_02003af4(void);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void Func_02003a54(struct Actor *object, s32 mode);
void Func_02003224(struct Actor *object, struct Actor *target);
void Func_020034ac(s32 mode);
void Func_02003784(s32 level, s32 layer);
void Func_020035e0(void);

void Func_02003240(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target == 0)
    return;
  /* FAKEMATCH(dead): a test no u16 can pass enters the loop a second way, so it
   * is no natural loop and its references are not weighted; as if (target != 0)
   * goto test; return;, 75 of 191 instructions differ in tla-en. */
  if (list[0] & 0x10000)
    goto body;
  goto test;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02003aec();
      if (!GameFlag_Test(flag)) {
        Func_02003a54(actor8, 1);
        Func_02003224(actor8, target);
        wait(1);
        Sound_Play(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(2);
        Func_02003a54(actor8, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_020034ac(0);
        Func_02003224(target, actor8);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02003a54(actor8, 0);
        Func_02003224(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(185);
        /* FAKEMATCH(wrapper): forwarded through Call3, which builds each
         * 0x10000 apart; as Map_SetScreenShake(0x10000, 0x40000, 0x10000), 21
         * of 191 instructions differ in tla-en. */
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(actor8, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        /* FAKEMATCH(wrapper): forwarded through Call3, which builds each -1
         * apart; as Map_SetScreenShake(-1, -1, 0xe666), 23 of 191 instructions
         * differ in tla-en. */
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02003af4();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
