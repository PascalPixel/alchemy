/// @file
/// The party meets person 11 on the ledge: they talk, the ground shakes, the screen blends out and back while the ledge changes, the party gains the item and person 11 joins the leader (flag 0x9ba).
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system/io_reg.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02004e5c(void);
void Func_02004ec4(s32 id, s32 target);
void Func_02004ddc(void);
void Func_020015d0(s32 a, s32 b);
void Func_02004dd4(void);
void Party_LearnAbility(s32 unit, s32 ability);
s32 Func_02004e64(void);

s32 Func_020019cc(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(volatile): the blend level rereads through its slot; as a plain local,
   * 112 of 250 instructions differ in tla-en. */
  volatile u16 level;
  s32 message;

  /* FAKEMATCH(wrapper): the calls that repeat an argument are forwarded through
   * call.h, so each builds its own; as direct calls, 32 of 250 instructions differ
   * in tla-en. */
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  message = (s32)MsgGaiaRockInside09;
  ((s32 (*)())FieldMessage_Show)(message, 1);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  if (leader->z >> 19 <= 39) {
    Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1b8, 0x148);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  }
  Func_02004ec4(11, gMainRoutineWork.currentOwner);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1d6, 0x162);
  Actor_WalkTo(11, 0x1be, 0x162);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(11);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(11, 0, 0);
  Event_SetMessageId(message + 1);
  Event_ShowMessage(11, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(11, 0xc000, 0);
  FieldEvent_Wait(10);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1d2, 0x156);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Message_ShowCentered(message + 2, 1);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Map_WaitForShakeToSettle();
  Func_02004ddc();
  level = REG_BLDALPHA & 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    wait(1);
  }
  REG_DISPCNT &= ~0x200;
  Call2((void (*)())Func_020015d0, 12, 4);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  while (level != 8) {
    REG_BLDALPHA = level | 0x1000;
    level++;
    wait(1);
  }
  Func_02004dd4();
  Actor_SetPosition(12, 0, 0);
  Party_LearnAbility(4, 139);
  Event_SetFlag(0x9ba);
  Actor_SetSpeed(11, 0x13333, 0x9999);
  Actor_SetAnimation(11, 2);
  {
    struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);

    if (owner != 0)
      Actor_SetDestination(11, ((s16 *)&owner->x)[1], ((s16 *)&owner->z)[1]);
  }
  FieldActor_WaitForMoveById(11);
  Actor_SetPosition(11, 0, 0);
  FieldEvent_Wait(20);
  return Func_02004e64();
}
