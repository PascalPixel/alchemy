#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "scene_ids.h"
#include "call.h"
#include "flag_call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 flags);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fc4(s32 id, s32 x, s32 z);
void Func_0200201c(const void *message);
void Func_02002034(s32 id, s32 flags);
void Func_02002044(s32 id, s32 angle);
s32 Func_0200204c(s32 id, s32 emote, s32 frames);
void Func_0200205c(s32 speed, s32 acceleration);
void Func_0200206c(void);
void Func_020020ac(s32 skip);
void Func_020020b4(s32 id, s32 dx, s32 dz, s32 angle);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Func_02001f2c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Func_02001f2c(a, b, c, d, e, f);
}

s32 Func_02000c80(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov657Room1) {
    Func_02001f2c_(2, 2, 10, 24, 18, 10);
    Func_02001f2c_(0, 62, 26, 62, 3, 8);
    Func_02001f2c_(50, 65, 25, 61, 6, 1);
  }
  return 0;
}

/// Unless flag 0x8ff is set: the crew sights the ship from the deck and the
/// leader is asked about it. Either way flag 0x980 is set.
void Func_02000ce0(void) {
  s32 answer;
  s32 owner;

  if (Flag_IsSet(SPLIT_CONSTANT(0x8ff)) != 0) {
    Flag_Set(0x980);
    return;
  }
  Func_02001f5c();
  Func_020020ac(0);
  Call3((void (*)())Actor_SetSpeedParameters, 4, SPLIT_CONSTANT(0xcccc), SPLIT_CONSTANT(0x6666));
  Actor_SetPositionAndReset(4, 264, 136);
  Func_0200201c(MsgLookItS);
  Call4((void (*)())Func_020020b4, 5, 16, -16, FACING_SOUTH);
  Call4((void (*)())Func_020020b4, 9, 32, -16, FACING_SOUTH);
  Call4((void (*)())Func_020020b4, 6, -16, -16, FACING_SOUTH);
  FieldEvent_Wait(80);
  Func_0200204c(9, 0x102, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x19999, SPLIT_CONSTANT(0xcccc));
  Actor_SetPositionAndReset(9, 300, 152);
  FieldEvent_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 9, FACING_SOUTH + FACING_STEP, 20);
  Call3((void (*)())Actor_ArmCallback, 9, FACING_SOUTHEAST + FACING_STEP, 20);
  Call3((void (*)())Actor_ArmCallback, 9, FACING_SOUTH + FACING_STEP, 0);
  Func_02002034(9, 0);
  Func_0200205c(0x4cccc, SPLIT_CONSTANT(0x9999));
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(200), -1, PIXELS(344),
        1);
  Func_0200206c();
  FieldEvent_Wait(20);
  Func_0200205c(0x13333, SPLIT_CONSTANT(0x2666));
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(600), -1, PIXELS(344),
        1);
  Func_0200206c();
  FieldEvent_Wait(40);
  Func_0200205c(0x66666, SPLIT_CONSTANT(0xcccc));
  Camera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(128), 1);
  Func_0200206c();
  Motion_SetModeAndWaitAnimation(5, 4);
  Func_02002034(SPLIT_CONSTANT(0x2005), 0);
  Func_02002044(6, FACING_EAST);
  Actor_SetModeById(6, 4);
  Func_02002034(6, 0);
  Call2((void (*)())Func_02002044, 9, FACING_NORTHWEST + FACING_STEP);
  Func_0200204c(9, 0x101, 0);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetModeById(9, 3);
    Func_02002034(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_WaitForAnimationChange(owner);
    Func_0200201c(MsgLookItS + 5); // the fifth message after it
    Func_02002034(9, 0);
  }
  Actor_SetModeById(5, 4);
  Func_02002034(SPLIT_CONSTANT(0x2005), 0);
  Func_02002044(6, FACING_EAST);
  Actor_SetModeById(6, 4);
  Func_02002034(6, 0);
  Actor_SetModeById(9, 4);
  Func_02002034(9, 0);
  Actor_SetSpeedParameters(9, SPLIT_CONSTANT(0xcccc), SPLIT_CONSTANT(0x6666));
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_SetPositionAndReset(9, 288, 144);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Func_02002034(SPLIT_CONSTANT(0x2009), 0);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(4, 3);
  Motion_SetModeAndWaitAnimation(9, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x13333, SPLIT_CONSTANT(0x9999));
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, SPLIT_CONSTANT(0x9999));
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, SPLIT_CONSTANT(0x9999));
  {
    struct Actor *target;

    Actor_SetModeById(9, 2);
    target = Actor_GetById(4);
    if (target != NULL)
      Actor_ResetAndSetPosition(9, ((s16 *)&target->x)[1],
                                ((s16 *)&target->z)[1]);
    Actor_SetModeById(5, 2);
    target = Actor_GetById(4);
    if (target != NULL)
      Actor_ResetAndSetPosition(5, ((s16 *)&target->x)[1],
                                ((s16 *)&target->z)[1]);
    Actor_SetModeById(6, 2);
    target = Actor_GetById(4);
    if (target != NULL)
      Actor_ResetAndSetPosition(6, ((s16 *)&target->x)[1],
                                ((s16 *)&target->z)[1]);
  }
  Actor_WaitForMoveById(6);
  Func_02001fc4(6, 0, 0);
  Func_02001fc4(5, 0, 0);
  Func_02001fc4(9, 0, 0);
  Func_02001f64();
  Flag_Set(0x980);
}
