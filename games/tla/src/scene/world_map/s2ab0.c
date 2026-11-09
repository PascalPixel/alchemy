#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

extern char MsgWeReSaved[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020042e0(s32 mode);
void Func_02004b40(void);
void Func_02004c20(void);
void Func_02004da8(void);
void Func_02004db0(void);

struct RescueMap {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 unk11e;
};

void Func_02002ab0(void) {
  struct Actor *boat = Actor_GetById(10);
  struct RescueMap *map = (struct RescueMap *)Ram_HeapSlots->mapWork;
  s32 i;

  Func_02004c20();
  Event_ResolvePendingActions(0);
  WorldMap_SetWindowCellTileAndDraw(64, 176, 304, 240);
  WorldMap_SetWindowCellTileAndDraw(80, 176, 320, 240);
  boat->scaleX = 0x13333;
  boat->scaleY = 0x13333;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Actor_SetSpeed(10, 0x6666, 0x3333);
  boat->action = 0;
  boat->acceleration = 0x2000;
  boat->speedLimit = 0x2000;
  FieldActor_SetMoveTarget(boat, 0x27ff0000, boat->y, 0x1ecc0000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004da8();
  Event_SetMessageId((s32)MsgWeReSaved);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMoveTarget(boat, 0x280f0000, boat->y, 0x1ebc0000);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(5, 0);
  Sound_Play(78);
  Sound_Play(162);
  Event_ShowMessageAndWait(gMainRoutineWork.currentOwner, 0);
  Event_ShowMessageAndWait(8, 0);
  Sound_Play(61);
  Func_02004db0();
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(0x283f0000, -1, 0x1e8c0000, 1);
  for (i = 31; i >= 0; i--) {
    map->unk11e += 0xff80;
    map->unk11c += 128;
    Actor_GetById(10)->scaleY += -0x300;
    wait(1);
  }
  Func_02004da8();
  GameFlag_SetBit(0x200);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(8, 0);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  Func_02004db0();
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  map->unk11e = 0;
  map->unk11c = 0xe000;
  wait(1);
  FieldCamera_MoveTo(0x0e470000, -1, 0x1dd00000, 0);
  Func_02004b40();
  Actor_SetPosition(8, 0x0e6a0000, 0x1dc00000);
  Actor_SetPosition(9, 0x0e3e0000, 0x1dff0000);
  Actor_SetPosition(4, 0x0e550000, 0x1df00000);
  Actor_SetPosition(5, 0x0e490000, 0x1dc50000);
  Actor_SetPosition(6, 0x0e770000, 0x1de30000);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  wait(1);
  Func_02004da8();
  Actor_SetAttachedEffect(5, 0x102);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Actor_Jump(6, 6, 15);
  Actor_Jump(6, 6, 23);
  Actor_SetAngleToward(6, 4, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAttachedEffect(8, 0x101);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkTo(8, 0xe18, 0x1e0f);
  Sound_Play(163);
  Func_020042e0(0);
  Import_EventSetExitRequest(65);
}
