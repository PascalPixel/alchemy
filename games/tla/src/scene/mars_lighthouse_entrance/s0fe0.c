#include "room.h"
#include "io_reg.h"
#include "scene_tables.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02000df0(void);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);

void Func_02000fe0(const u8 *event) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  // The leader's map column and, later, the frame count share one variable,
  // as the native's register shows.
  s32 i = leader->x >> 20;

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, z >> 20);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  leader->update = 0;
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Func_02004bc0(leader, 27);
  Actor_SetAnimationSpeed(leader, 48);
  System_Wait(30);
  leader->angle = FACING_SOUTH;
  leader->motionFlags = 0;
  if ((event[3] & 64) == 0) {
    {
      /* FAKEMATCH(temporary): the window mask is built in a word register, as
       * the native builds it; as REG_WINOUT = 0xf1f, 18 of 130 instructions
       * differ in tla-en. */
      u16 winout = 0xf1f;

      REG_WINOUT = winout;
    }
    REG_DISPCNT |= 0x8000;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(204);
  leader->scaleX -= 0x400;
  leader->scaleY -= 0x300;
  leader->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}
