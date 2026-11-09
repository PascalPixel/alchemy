#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 MsgByReachingThis[];
extern const u8 gData_020012a4[];
void Func_0200119c(void);
void Func_020011a4(void);
void Func_02001154(struct Actor *object, const u8 *script);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020009bc(void);
void Func_020011d4(s32 actor, s32 target);
s32 Func_02001264(void);
void Func_02001214(s32 actor, s32 a);

union EventLine {
  s32 word;
  u16 line;
};

#define TALK()                                                                 \
  FieldMessage_ShowCentered((s16)((union EventLine *)work)->line++, 0, -3)

void Func_020009d4(void) {
  struct Actor *follower = Actor_GetById(9);
  struct Actor *statue = Actor_GetById(0x40);
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *glow, *leader;
  s32 speed;

  Func_0200119c();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x403108, 1);
  BattleEffect_StartBufferInterpolation(60);
  FieldCamera_MoveTo(0xa80000, -1, 0x9e0000, 1);
  Sound_Play(26);
  Event_Wait(60);
  Actor_SetModeById(8, 2);
  Sound_Play(0x240);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  work += 0x1c4;
  TALK();
  Actor_SetPosition(0x40, 0xa80000, 0x980000);
  statue->motionFlags = 6;
  statue->terrainHeight += 0x100000;
  Sound_Play(220);
  glow = ((struct Actor *)Import_ActorCreate(412, 0, 0, 0));
  Func_02001154(glow, gData_020012a4);
  Animation_ApplyChildValues(glow, 1);
  glow->motionFlags = 0;
  glow->x = statue->x;
  glow->y = statue->y + 0x80000;
  glow->z = statue->z;
  glow->terrainHeight = statue->terrainHeight;
  glow->update = Func_020009bc;
  Event_Wait(15);
  glow->x = 0;
  glow->y = 0;
  glow->z = 0;
  glow->update = 0;
  TALK();
  Actor_ResetAndSetPosition(gMainRoutineWork.currentOwner, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_020011d4(9, gMainRoutineWork.currentOwner);
  speed = 0x10000;
  follower->speedLimit = speed;
  Actor_ResetAndSetPosition(9, 184, 188);
  Actor_WaitForMoveById(9);
  Actor_FaceDirection(9, 0x8000, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  TALK();
  Func_080d47b4(9, 258, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ShowEmote(9, 261, 0);
  Event_Wait(30);
  TALK();
  Func_080d47b4(9, 265, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Func_080d47b4(9, 257, 0);
  Event_Wait(60);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 261, 0);
  Func_080d47b4(9, 261, 0);
  Event_Wait(60);
  TALK();
  Event_Wait(40);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(8, 2);
  TALK();
  Event_Wait(30);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_WaitForMoveById(9);
  GameFlag_SetBit(0x9ce);
  Func_02001264();
  PaletteFade_ApplyTarget(speed, 0);
  BattleEffect_StartBufferInterpolation(60);
  Func_02001214(gMainRoutineWork.currentOwner, 1);
  Event_Wait(60);
  Func_020011a4();
}
