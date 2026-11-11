#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "input.h"
#include "scene_ids.h"
#include "event_inline.h"

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004d38(s32 scene, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004d08_(s32 actor, s32 value, s32 frames) {
  Func_080d47b4(actor, value, frames);
}

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_Ov667.
void Func_02002e00(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  {
    /* FAKEMATCH(pin): the mode pinned to r3 starts right after the 1 negated
     * for y, before x loads; as a plain constant, 2 of 39 instructions differ
     * in tla-en. */
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
  s32 seen = Event_GetFlag(0x8de);

  if (seen == 0) {
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    scale = Actor_GetById(gMainRoutineWork.currentOwner)->scaleX;
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
    Event_Wait(40);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(4, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 264, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldMessage_ShowAtActor(7, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(8, 265, 30);
      FieldEvent_ShowMessageThenWait(9, 0, 10);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Actor_ShowEmote(8, 265, 30);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
      FieldEvent_ShowMessageThenWait(9, 0, 10);
    }
    Func_02004d08_(8, 256, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Func_02004d08_(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    Func_02004d08_(8, 264, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    Actor_ShowEmote(8, 258, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 264, 30);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
    gMainRoutineWork.renderMode = 7;
    gMainRoutineWork.ownerOverride = 1;
    Func_02004db0();
    Event_SetFlag(0x8de);
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(6, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(5, 0, 0);
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = scale;
    Func_02004c28();
  }
}
