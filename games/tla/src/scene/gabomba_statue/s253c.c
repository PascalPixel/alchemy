#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "event_inline.h"

struct Wanderer {
  u8 unk00[98];
  u8 timer;
};

extern u32 gData_02005af4[];
extern u32 gData_02005b1c[];
extern u32 gData_02005b60[];
extern u32 gData_02005ba4[];
extern u32 gData_02005be8[];
extern s32 gData_02005c74[];
extern s32 gData_02005ca0[];
void Func_020056b4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005724(s32 id, s32 target);
void Func_020057b4(void);
void Func_02000f6c(void);
u32 Random16Far(void);
s32 Func_02004d48(s32 a, s32 b);
void Event_FinishScreenTransition(void);

static __inline__ void Func_0200578c_ArgsFirst(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: the second and third arguments pinned to r1 and r2 are set
   * before the id at four of Func_020027cc's five calls; as direct calls, 24
   * instructions differ in tla-en. */
  register s32 a_ asm("r1") = a;
  register s32 b_ asm("r2") = b;

  Func_080d47b4(id, a_, b_);
}

void Func_0200253c(void) {
  Func_020056b4();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId(MsgGabombaFocusEnergies);
  Event_ShowMessageAndWait(24, 0);
  Func_02005724(23, gMainRoutineWork.currentOwner);
  Func_02005724(5, gMainRoutineWork.currentOwner);
  Func_02005724(6, gMainRoutineWork.currentOwner);
  Func_02005724(7, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(23, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(23, gData_02005ba4);
  wait(10);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005af4);
  Actor_SetMovementTypeById(5, gData_02005b1c);
  Actor_SetMovementTypeById(7, gData_02005b60);
  Actor_RefreshSelectorById(23);
  Event_Wait(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(434), -1, PIXELS(286), 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Actor_SetMovementTypeAndWaitById(23, gData_02005be8);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(23, FACING_NORTH + FACING_STEP);
  Event_ShowMessageAndWait(23, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(434), -1, PIXELS(168), 1);
  Func_020057b4();
  Event_Wait(20);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_ArmCallback(7, FACING_SOUTHEAST, 40);
  Actor_GetById(7)->angle = FACING_SOUTH;
  Actor_GetById(gMainRoutineWork.currentOwner)->angle = 0x4000;
  Actor_GetById(6)->angle = FACING_SOUTH;
  Actor_GetById(5)->angle = FACING_SOUTH;
  Actor_GetById(7)->actionFlags &= 0xfe;
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_GetById(6)->actionFlags &= 0xfe;
  Actor_GetById(5)->actionFlags &= 0xfe;
  Actor_WalkTo(7, 400, 272);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 420, 272);
  Actor_WalkTo(6, 444, 272);
  Actor_ResetAndSetPositionInMode2(5, 468, 272);
  Event_Wait(20);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  BattleEffect_StartBufferInterpolation(80);
  wait(80);
  Import_EventSetExitRequest(11);
}

void Func_020027cc(void) {
  s32 speed;

  Func_020056b4();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Sound_Play(78);
  speed = 0x4000;
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(348),
                             PIXELS(312), speed);
  Actor_SetPositionAndFacing(7, PIXELS(330), PIXELS(312), speed);
  {
    s32 slow = 0x3000;

    Actor_SetPositionAndFacing(23, PIXELS(432), PIXELS(312), slow);
  }
  Actor_SetPositionAndFacing(6, PIXELS(448), PIXELS(312), speed);
  Actor_SetHorizontalPositionAndAngleWithTerrain(5, PIXELS(468), PIXELS(312),
                                                 speed);
  wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(220);
  wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(80);
  Func_02000f6c();
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(10);
  wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 256, 0);
  Func_0200578c_ArgsFirst(5, 256, 0);
  Func_0200578c_ArgsFirst(6, 256, 0);
  Func_0200578c_ArgsFirst(7, 256, 0);
  Func_0200578c_ArgsFirst(23, 256, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_ArmCallback(23, FACING_NORTHWEST + FACING_STEP, 60);
  Sound_Play(130);
  Actor_SetPartAttribute(Actor_GetById(9), 7);
  Actor_SetPartAttribute(Actor_GetById(10), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(10), 3);
  Actor_SetPartAttribute(Actor_GetById(9), 0);
  wait(8);
  ((struct Wanderer *)Actor_GetById(gMainRoutineWork.currentOwner))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(7))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(5))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(6))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(23))->timer =
      (Random16Far() * 40 >> 16) + 10;
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005c74);
  Actor_SetMovementTypeById(7, gData_02005c74);
  Actor_SetMovementTypeById(5, gData_02005ca0);
  Actor_SetMovementTypeById(6, gData_02005ca0);
  Actor_SetMovementTypeById(23, gData_02005ca0);
  Sound_Play(11);
  Func_02004d48(11, 0);
  Event_Wait(80);
  GameFlag_SetBit(0x8f7);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(12);
}
