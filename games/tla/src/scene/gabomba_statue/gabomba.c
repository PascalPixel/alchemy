#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];
extern u32 gData_02005d8c[];
u8 *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
void Func_020056b4(void);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_0200564c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057b4(void);
void Func_02004f40(s32 *x_out, s32 *z_out);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02005624(struct Actor *object, s32 mode);
s32 ArcTan2(s32 y, s32 x);
void Func_02005644(struct Actor *object);

void Func_02002f70(void) {
  s32 index = Func_02003604();
  s32 x;
  s32 z;
  struct Actor *spark;
  struct Actor *actor;
  s32 fast;
  s32 slow;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_020048a4(19, 11, gData_02005d8c[index], 16);
  Actor_SetAnimation(15, 5);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(18, 0, 0);
  Actor_SetPosition(19, 0, 0);
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(21, 0, 0);
  Actor_SetPosition(22, 0, 0);
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(182), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  fast = 0x4000;
  Actor_SetPositionAndFacing(5, PIXELS(472), PIXELS(216), fast);
  slow = 0x3000;
  Actor_SetPositionAndFacing(24, PIXELS(472), PIXELS(200), slow);
  System_Wait(1);
  Func_0200564c();
  System_Wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(312), 1);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(24, 0x10000, 0x8000);
  Actor_SetScript(5, gData_02005ccc);
  Actor_RunScriptAndWait(24, gData_02005d24);
  FieldEvent_SetMessageId(MsgGabombaSawEnergyFlow);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(24, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_WalkToAndWait(5, 440, 296);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  FieldCamera_MoveTo(PIXELS(414), -1, PIXELS(272), 1);
  Func_020057b4();
  Event_ShowMessage(5, 0);
  Func_02004f40(&x, &z);
  x = (x << 20) + (156 << 17);
  z = (z << 20) + (184 << 16);
  spark = (FieldActor_Create(434, x, 0, z));
  spark->motionFlags |= 4;
  Actor_SetSingleChildFlags(spark, 0);
  Func_02005624(spark, 1);
  System_Wait(1);
  actor = Actor_Get(5);
  actor->angle = ArcTan2(spark->z - actor->z, spark->x - actor->x);
  /* FAKEMATCH(barrier): an empty asm after the angle is stored, so the store
   * comes before the camera call's constants are started; without it, the -1
   * is started first and 2 instructions differ (with x and z read into locals
   * first, 16). */
  asm volatile("");
  FieldCamera_MoveTo(spark->x, -1, spark->z, 1);
  Func_020057b4();
  Event_ShowMessage(5, 0);
  Func_02005644(spark);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(304), 1);
  Func_020057b4();
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(24, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(24, 0);
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(472),
                             PIXELS(204), fast);
  Actor_SetPositionAndFacing(7, PIXELS(440), PIXELS(204), fast);
  Actor_SetPositionAndFacing(23, PIXELS(456), PIXELS(204), slow);
  Actor_SetPositionAndFacing(6, PIXELS(488), PIXELS(204),
                                                 fast);
  System_Wait(1);
  Event_ShowMessage(23, 0);
  Actor_FaceDirection(24, FACING_NORTH + FACING_STEP, 0);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(264), 1);
  Func_020057b4();
  FieldActor_Jump(24, 4, 10);
  Event_ShowMessage(0x4018, 0);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x8007, 0);
  Actor_ShowEmote(24, 257, 40);
  Event_ShowMessage(0x8006, 0);
  Actor_SetAnimation(23, 4);
  Event_ShowMessage(0x9017, 0);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_ClearFlag(265);
  System_Wait(60);
  FieldEvent_SetExitRequest(18);
}

void Func_02003338(void) {
  Func_0200533c();
  Func_02005724(23, gMainRoutineWork.currentOwner);
  Actor_SetSpriteColor(Actor_Get(23), 15);
  System_Wait(10);
  FieldEvent_SetMessageId(MsgGabombaChannelTiming);
  Event_ShowMessage(23, 0);
  Actor_SetPosition(23, 0, 0);
  System_Wait(1);
  Event_SetFlag(0x9ec);
  Func_02005364();
}
