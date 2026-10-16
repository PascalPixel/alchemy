#include "scene.h"

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
  u8 unk1b0[4];
  s32 frames;
};

void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Camera_FollowActor(s32 actor, s32 target);
void Event_StartScreenTransition(void);
void Func_020034d4(s32 actor, s32 x, s32 z);
s32 Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);
void Func_0200356c(s32 scene, s32 entrance);

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020034d4(actor, x, z);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

void Func_02001774(void) {
  struct EventTransition *work;

  Func_0200345c();
  Func_020035b4(0);
  Func_0200350c((s32)MsgHaveYouLearned);
  Func_020034d4_(gPartyState.currentOwner, 568 << 16, 248 << 16);
  Func_020034d4_(8, 584 << 16, 224 << 16);
  Func_020034d4_(13, 600 << 16, 240 << 16);
  Func_020034d4_(11, 584 << 16, 264 << 16);
  ObjectMotion_ArmCallback_(8, 0x5000, 0);
  ObjectMotion_ArmCallback_(11, 0xb000, 0);
  Object_SetModeById(gPartyState.currentOwner, 41);
  Camera_FollowActor(gPartyState.currentOwner, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(10);
  Func_02003544(11, 264, 30);
  Func_0200351c(11, 0, 5);
  Motion_SetModeAndWaitAnimation(8, 4);
  Func_0200351c(8, 0, 20);
  work = Ram_HeapSlots->eventWork;
  work->frames = 40;
  work->type = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200356c((s16)gPartyState.savedScene, (s16)gPartyState.savedEntrance);
}
