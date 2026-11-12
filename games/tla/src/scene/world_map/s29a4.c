/// @file
/// Scene code of overlay 0x64b, in ROM order, from s29a4's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "task.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"

struct EventWork {
  u8 unk000[0x1ac];
  s32 startTransition;
  u8 unk1b0[4];
  s32 transitionFrames;
};

extern const u8 gData_02004eb8[];

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020028cc(void);
void Func_02004c20(void);

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

void Func_020029a4(void) {
  struct Actor *actor;

  actor = Actor_Get(8);
  FieldEvent_Wait(60);
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  FieldCamera_FollowActor(8, 1);
  System_Wait(1);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_Get(8), 0);
  Actor_SetSpeedParameters_(8, 0x6666, 0x3333);
  actor->action = 0;
  Actor_SetScript(8, gData_02004eb8);
  Task_AddFrame(Func_020028cc, TASK_PASS_VBLANK);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x100;
  PaletteFade_ApplyTarget(0x10003, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  FieldEffect_ScheduleRatioTransition(0x16666, 300);
  FieldEvent_Wait(270);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 16;
  *(u16 *)BG_PLTT = RGB(31, 31, 31);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(77);
}

extern char MsgWeReSaved[];

void Func_020042e0(s32 mode);
void Func_02004b40(void);
void Func_02004da8(void);
void Func_02004db0(void);

struct RescueMap {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 unk11e;
};

void Func_02002ab0(void) {
  struct Actor *boat = Actor_Get(10);
  struct RescueMap *map = (struct RescueMap *)Ram_HeapSlots->mapWork;
  s32 i;

  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  WorldMap_SetWindowCellTileAndDraw(64, 176, 304, 240);
  WorldMap_SetWindowCellTileAndDraw(80, 176, 320, 240);
  boat->scaleX = 0x13333;
  boat->scaleY = 0x13333;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetSpeed(10, 0x6666, 0x3333);
  boat->action = 0;
  boat->acceleration = 0x2000;
  boat->speedLimit = 0x2000;
  FieldActor_SetMoveTarget(boat, 0x27ff0000, boat->y, 0x1ecc0000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004da8();
  FieldEvent_SetMessageId((s32)MsgWeReSaved);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  FieldActor_SetMoveTarget(boat, 0x280f0000, boat->y, 0x1ebc0000);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(78);
  Event_PlaySound(162);
  Event_ShowMessage(gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(8, 0);
  Event_PlaySound(61);
  Func_02004db0();
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(0x283f0000, -1, 0x1e8c0000, 1);
  for (i = 31; i >= 0; i--) {
    map->unk11e += 0xff80;
    map->unk11c += 128;
    Actor_Get(10)->scaleY += -0x300;
    System_Wait(1);
  }
  Func_02004da8();
  Event_SetFlag(0x200);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  Func_02004db0();
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  map->unk11e = 0;
  map->unk11c = 0xe000;
  System_Wait(1);
  FieldCamera_MoveTo(0x0e470000, -1, 0x1dd00000, 0);
  Func_02004b40();
  Actor_SetPosition(8, 0x0e6a0000, 0x1dc00000);
  Actor_SetPosition(9, 0x0e3e0000, 0x1dff0000);
  Actor_SetPosition(4, 0x0e550000, 0x1df00000);
  Actor_SetPosition(5, 0x0e490000, 0x1dc50000);
  Actor_SetPosition(6, 0x0e770000, 0x1de30000);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     1);
  System_Wait(1);
  Func_02004da8();
  Actor_SetAttachedEffect(5, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldActor_Jump(6, 6, 15);
  FieldActor_Jump(6, 6, 23);
  Actor_FaceActor(6, 4, 0);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(8, 0x101);
  Event_ShowMessage(8, 0);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkTo(8, 0xe18, 0x1e0f);
  Event_PlaySound(163);
  Func_020042e0(0);
  FieldEvent_SetExitRequest(65);
}
