#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_0200120c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_02001254(s32 actor, s32 target);

static inline void GondowanCliffs_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): the call through this inline; as direct calls to
   * Actor_SetPositionAndReset, 78 of 165 instructions of Func_020007d8 differ
   * in tla-en. */
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void GondowanCliffs_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH(pin): the height fixed to r1, so the actor is loaded after the
   * height and frames, as in overlay 694's Contigo_Jump; as a plain local, the
   * order differs. */
  register s32 h __asm__("r1") = height;

  Func_080d47b4(actor, h, frames);
}

/// The scene that asks where the party intends to go, to help recover the black
/// orb.
void Func_020007d8(void) {
  Func_0200120c();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 552, 488);
  Func_02001254(5, gMainRoutineWork.currentOwner);
  FieldActor_WatchAndSetScript(5, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 568, 480);
  FieldActor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Func_02001254(6, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 584, 472);
  FieldActor_WatchAndSetScript(22, gMainRoutineWork.currentOwner);
  Func_02001254(22, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 608, 472);
  FieldEvent_SetMessageId((s32)MsgWhereDoYou);
  GondowanCliffs_Jump(22, 258, 20);
  Event_ShowMessage(22, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  FieldActor_Jump(5, 6, 15);
  FieldActor_Jump(5, 6, 23);
  Event_ShowMessage(5, 0);
  FieldActor_Stop(6);
  Actor_FaceActor(6, 5, 0);
  Event_ShowMessage(6, 0);
  FieldActor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  Event_ShowMessage(22, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 584, 472);
  Actor_SetPosition(22, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 568, 480);
  Actor_SetPosition(6, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 552, 488);
  Actor_SetPosition(5, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 528, 488);
  Event_FinishScreenTransition();
  Event_PlaySound(123);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 512, 488);
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
}
