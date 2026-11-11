#include "types.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_020028b4();
s32 Actor_SetSingleChildFlags();
s32 Func_02002994();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
void Func_020028bc(void);
extern u8 MsgAreAreYou[];

void Func_02001c30(void) {
  s32 t96;

  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(5, PIXELS(364), PIXELS(156));
  Actor_SetPosition(8, PIXELS(362), PIXELS(142));
  Actor_SetPosition(9, PIXELS(390), PIXELS(144));
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldActor_SetSpritePriority(8, 2);
  Actor_SetSpriteRotationById(5, 0x4000);
  Actor_SetSingleChildFlags(((s32)Actor_Get(5)), 0);
  Func_02002994(5, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 0x3c;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgAreAreYou);
  Actor_ShowEmote(8, 0x102, 40);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetPosition(5, PIXELS(376), PIXELS(164));
  Actor_SetSpriteRotationById(5, 0);
  Actor_SetSingleChildFlags(((s32)Actor_Get(5)), 1);
  FieldActor_Jump(5, 6, 50);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(5, -14, -2);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0, 0);
  Actor_FaceDirection(5, 0xf000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 0, 0x10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(5, 9);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkBy(9, 0, 0x78);
  FieldEvent_Wait(40);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkByAndWait(8, 0x18, 0);
  Actor_WalkByAndWait(8, 0, 0x14);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x105, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  FieldActor_Jump(8, 4, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  t96 = ((s32)Actor_Get(5));
  if (t96 != 0) {
    Actor_SetDestination(8, FIELD_AT(t96, s16, 10),
                              FIELD_AT(t96, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 8;
  Func_020028bc();
}
