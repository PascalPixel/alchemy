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
  Func_080d47b4(id, a, b);
}

void Func_0200253c(void) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaFocusEnergies);
  Event_ShowMessage(24, 0);
  Func_02005724(23, gMainRoutineWork.currentOwner);
  Func_02005724(5, gMainRoutineWork.currentOwner);
  Func_02005724(6, gMainRoutineWork.currentOwner);
  Func_02005724(7, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(23, 0xcccc, 0x6666);
  Actor_SetScript(23, gData_02005ba4);
  System_Wait(10);
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02005af4);
  Actor_SetScript(5, gData_02005b1c);
  Actor_SetScript(7, gData_02005b60);
  FieldActor_RefreshSelectorById(23);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(434), -1, PIXELS(286), 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Actor_RunScriptAndWait(23, gData_02005be8);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(23, FACING_NORTH + FACING_STEP);
  Event_ShowMessage(23, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(434), -1, PIXELS(168), 1);
  Func_020057b4();
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 40);
  Actor_Get(7)->angle = FACING_SOUTH;
  Actor_Get(gMainRoutineWork.currentOwner)->angle = 0x4000;
  Actor_Get(6)->angle = FACING_SOUTH;
  Actor_Get(5)->angle = FACING_SOUTH;
  Actor_Get(7)->actionFlags &= 0xfe;
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_Get(6)->actionFlags &= 0xfe;
  Actor_Get(5)->actionFlags &= 0xfe;
  Actor_WalkTo(7, 400, 272);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 420, 272);
  Actor_WalkTo(6, 444, 272);
  Actor_WalkTo(5, 468, 272);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(80);
  System_Wait(80);
  FieldEvent_SetExitRequest(11);
}

void Func_020027cc(void) {
  s32 speed;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySound(78);
  speed = 0x4000;
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(348),
                             PIXELS(312), speed);
  Actor_SetPositionAndFacing(7, PIXELS(330), PIXELS(312), speed);
  {
    s32 slow = 0x3000;

    Actor_SetPositionAndFacing(23, PIXELS(432), PIXELS(312), slow);
  }
  Actor_SetPositionAndFacing(6, PIXELS(448), PIXELS(312), speed);
  Actor_SetPositionAndFacing(5, PIXELS(468), PIXELS(312),
                                                 speed);
  System_Wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Event_PlaySound(220);
  System_Wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  Func_02000f6c();
  System_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 256, 0);
  Func_0200578c_ArgsFirst(5, 256, 0);
  Func_0200578c_ArgsFirst(6, 256, 0);
  Func_0200578c_ArgsFirst(7, 256, 0);
  Func_0200578c_ArgsFirst(23, 256, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(23, FACING_NORTHWEST + FACING_STEP, 60);
  Event_PlaySound(130);
  Actor_SetSpriteColor(Actor_Get(9), 7);
  Actor_SetSpriteColor(Actor_Get(10), 7);
  System_Wait(8);
  Actor_SetSpriteColor(Actor_Get(10), 3);
  Actor_SetSpriteColor(Actor_Get(9), 0);
  System_Wait(8);
  ((struct Wanderer *)Actor_Get(gMainRoutineWork.currentOwner))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(7))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(5))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(6))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(23))->timer =
      (Random16Far() * 40 >> 16) + 10;
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02005c74);
  Actor_SetScript(7, gData_02005c74);
  Actor_SetScript(5, gData_02005ca0);
  Actor_SetScript(6, gData_02005ca0);
  Actor_SetScript(23, gData_02005ca0);
  Event_PlaySound(11);
  Func_02004d48(11, 0);
  FieldEvent_Wait(80);
  Event_SetFlag(0x8f7);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(12);
}
