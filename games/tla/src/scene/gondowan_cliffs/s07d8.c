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
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as
   * direct calls the coordinates are kept in callee-saved registers (79
   * differ). */
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void GondowanCliffs_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH: the native loads the actor after the height and frames, as
   * overlay 694's Contigo_Jump. */
  register s32 h __asm__("r1") = height;

  Func_080d47b4(actor, h, frames);
}

/// The scene that asks where the party intends to go, to help recover the black
/// orb.
void Func_020007d8(void) {
  Func_0200120c();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 552, 488);
  Func_02001254(5, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(5, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 568, 480);
  Actor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Func_02001254(6, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 584, 472);
  Actor_WatchAndSetScript(22, gMainRoutineWork.currentOwner);
  Func_02001254(22, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 608, 472);
  Event_SetMessageId((s32)MsgWhereDoYou);
  GondowanCliffs_Jump(22, 258, 20);
  Event_ShowMessageAndWait(22, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  Actor_Jump(5, 6, 15);
  Actor_Jump(5, 6, 23);
  Event_ShowMessageAndWait(5, 0);
  Actor_Stop(6);
  Actor_SetAngleToward(6, 5, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Func_080d489c(gMainRoutineWork.currentOwner, 258);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(22, 3);
  Event_ShowMessageAndWait(22, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(22, 3);
  Event_Wait(30);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 584, 472);
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 568, 480);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 552, 488);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 528, 488);
  Event_FinishScreenTransition();
  Sound_Play(123);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 512, 488);
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(1);
}
