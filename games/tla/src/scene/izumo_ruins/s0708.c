#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 Func_020014d8(void);
struct Actor *Func_02001530(s32 *position, s32 flags);
void Func_02001488(void);
s32 Func_02001490(void);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void Func_02001428(struct Actor *object, s32 mode);
void Func_020006ec(struct Actor *object, struct Actor *target);
void Func_02000974(s32 mode);
void Func_02000c4c(s32 level, s32 layer);
void Func_02000aa8(void);

void Func_02000708(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *pillar;
  s32 position[3];

  if (Func_020014d8() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  pillar = Func_02001530(position, 0);
  if (pillar != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02001488();
      if (!GameFlag_Test(flag)) {
        Func_02001428(helper, 1);
        Func_020006ec(helper, pillar);
        wait(1);
        Sound_Play(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(2);
        Func_02001428(helper, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_02000974(0);
        Func_020006ec(pillar, helper);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02001428(helper, 0);
        Func_020006ec(helper, pillar);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02001428(helper, 2);
        Func_02000c4c(-2, pillar->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, pillar->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02001490();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
