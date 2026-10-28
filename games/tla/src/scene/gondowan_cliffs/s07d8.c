#include "types.h"
#include "main_routine_work.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

void Func_0200120c(void);
void Func_020012e4(s32 skip);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_02001254(s32 actor, s32 target);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void Func_0200128c(s32 message);
void Func_020012a4(s32 actor, s32 emote, s32 frames);
void Func_02001294(s32 speaker, s32 flags);
void Actor_Stop(s32 actor);
void Func_020012ac(s32 actor, s32 emote);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_SetModeById(s32 actor, s32 mode);
void Func_0200124c(s32 actor, s32 x, s32 z);
void Func_0200131c(s32 cue);
void Func_020012bc(s32 countdown);

static inline void GondowanCliffs_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as direct
   * calls the coordinates are kept in callee-saved registers (79 differ). */
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void GondowanCliffs_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH: the native loads the actor after the height and frames, as
   * overlay 694's Contigo_Jump. */
  register s32 h __asm__("r1") = height;
  /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
  register s32 f __asm__("r2") = frames;
  /* FAKEMATCH: and the actor last, in r0. */
  register s32 a __asm__("r0") = actor;

  Func_020012a4(a, h, f);
}

static inline void GondowanCliffs_ObjEmote(s32 actor, s32 emote) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as a
   * direct call its 258 is shared with the jump's (14 differ). */
  Func_020012ac(actor, emote);
}

/// The scene that asks where the party intends to go, to help recover the black
/// orb.
void Func_020007d8(void) {
  Func_0200120c();
  Func_020012e4(0);
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
  Func_0200128c((s32)MsgWhereDoYou);
  GondowanCliffs_Jump(22, 258, 20);
  Func_02001294(22, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  Actor_Jump(5, 6, 15);
  Actor_Jump(5, 6, 23);
  Func_02001294(5, 0);
  Actor_Stop(6);
  Actor_SetAngleToward(6, 5, 0);
  Func_02001294(6, 0);
  Actor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  GondowanCliffs_ObjEmote(gMainRoutineWork.currentOwner, 258);
  FieldEvent_Wait(30);
  Motion_SetModeAndWaitAnimation(22, 3);
  Func_02001294(22, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(22, 3);
  FieldEvent_Wait(30);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 584, 472);
  Func_0200124c(22, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 568, 480);
  Func_0200124c(6, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 552, 488);
  Func_0200124c(5, 0, 0);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 528, 488);
  Event_FinishScreenTransition();
  Func_0200131c(123);
  GondowanCliffs_ObjWalk(gMainRoutineWork.currentOwner, 512, 488);
  Event_WaitTransitionFrames();
  Func_020012bc(1);
}
