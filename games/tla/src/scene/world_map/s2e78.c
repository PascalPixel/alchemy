#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"
#include "input.h"
#include "scene_ids.h"

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 actor, s32 x, s32 z);
void Func_02004cd8(s32 message);
void Func_02004ce8(s32 actor, s32 flags, s32 frames);
s32 Func_02004d08(s32 actor, s32 value, s32 frames);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02004d20(s32 a, s32 b);
void Func_02004d38(s32 scene, s32 b);

static __inline__ void Func_02004d08_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d08(actor, value, frames);
}

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_Ov667.
void Func_02002e00(void) {
  Actor_GetById(gSequenceParentWork.currentOwner)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Func_02004d20(0x40000, 0x8000);
  {
    /* FAKEMATCH: pinned, the mode starts right after the 1 negated for y,
     * before x loads; as a plain constant, 2 instructions differ in tla-en. */
    register s32 mode asm("r3") = 1;

    Camera_MoveWithinMapBounds(11890 << 16, -1, 10090 << 16, mode);
  }
  // Entered at its test: as a while loop the test is copied ahead of it and
  // gInput hoisted (differ 17).
  goto wait;
  do {
    wait(1);
  wait:;
  } while (gInput.held == 0);
  Func_02004d38((s32)&SceneId_Ov667, 80);
}

void Func_02002e78(void) {
  s32 scale;
  s32 seen = Flag_IsSet(0x8de);

  if (seen == 0) {
    Func_02004c20();
    Func_02004db8(0);
    scale = Actor_GetById(gSequenceParentWork.currentOwner)->scaleX;
    Actor_GetById(gSequenceParentWork.currentOwner)->scaleX = 0;
    Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                       0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Func_02004cd8((s32)MsgTakeTheTiller);
    FieldEvent_Wait(40);
    Func_02004ce8(7, 0, 10);
    Func_02004d08_(8, 256, 30);
    Func_02004ce8(4, 0, 10);
    Func_02004ce8(7, 0, 10);
    Func_02004d08_(8, 257, 30);
    Func_02004ce8(6, 0, 10);
    Func_02004ce8(9, 0, 10);
    Func_02004d08_(8, 264, 30);
    Func_02004ce8(5, 0, 10);
    /* FAKEMATCH: called as returning a value, which the native's argument order
     * shows (r1 before r0). */
    ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(7, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Func_02004d08_(8, 265, 30);
      Func_02004ce8(9, 0, 10);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Func_02004d08_(8, 265, 30);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
      Func_02004ce8(9, 0, 10);
    }
    Func_02004d08_(8, 256, 30);
    Func_02004ce8(7, 0, 10);
    Func_02004ce8(9, 0, 10);
    Func_02004d08_(8, 257, 30);
    Func_02004ce8(6, 0, 10);
    Func_02004ce8(9, 0, 10);
    Func_02004d08_(8, 256, 30);
    Func_02004ce8(5, 0, 10);
    Func_02004ce8(9, 0, 10);
    Func_02004ce8(6, 0, 10);
    Func_02004d08_(8, 264, 30);
    Func_02004ce8(5, 0, 10);
    Func_02004d08_(8, 258, 30);
    Func_02004ce8(6, 0, 10);
    Func_02004ce8(5, 0, 10);
    Func_02004ce8(9, 0, 10);
    Func_02004d08_(8, 257, 30);
    Func_02004ce8(7, 0, 10);
    Func_02004d08_(8, 264, 30);
    Func_02004ce8(9, 0, 10);
    Func_02004ce8(6, 0, 10);
    Func_02004ce8(5, 0, 10);
    Func_02004ce8(7, 0, 10);
    Func_02004ce8(9, 0, 10);
    Func_02004c88(gSequenceParentWork.currentOwner, 0, 0);
    gSequenceParentWork.renderMode = 7;
    gSequenceParentWork.ownerOverride = 1;
    Func_02004db0();
    Flag_Set(0x8de);
    Func_02004c88(7, 0, 0);
    Func_02004c88(6, 0, 0);
    Func_02004c88(9, 0, 0);
    Func_02004c88(5, 0, 0);
    Actor_GetById(gSequenceParentWork.currentOwner)->scaleX = scale;
    Func_02004c28();
  }
}
