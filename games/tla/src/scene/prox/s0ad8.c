#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004464();
s32 Func_0200457c();
s32 Func_0200446c();
extern u8 MsgProx62[];

void Func_02000ad8(void) {
  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(160), -1, PIXELS(160), 1);
  Func_0200457c();
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(0xa, 1);
  Actor_FaceDirectionAndWait(10, 0xb000);
  FieldEvent_SetMessageId((s32)MsgProx62);
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(0xc, 0x102);
  Actor_SetAttachedEffect(0xd, 0x102);
  Actor_SetAttachedEffect(0xe, 0x102);
  Actor_SetAttachedEffect(0xf, 0x102);
  Actor_SetAttachedEffect(0x10, 0x102);
  Actor_SetAttachedEffect(0x11, 0x102);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Event_ShowMessage(10, 0);
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(0xc, 0x102);
  Actor_SetAttachedEffect(0xd, 0x102);
  Actor_SetAttachedEffect(0xe, 0x102);
  Actor_SetAttachedEffect(0xf, 0x102);
  Actor_SetAttachedEffect(0x10, 0x102);
  Actor_SetAttachedEffect(0x11, 0x102);
  FieldEvent_Wait(20);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(12, 0);
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_ShowEmote(10, 0x100, 0);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessage(13, 0);
  FieldActor_Jump(15, 4, 10);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_ShowEmote(10, 0x105, 40);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(16, 0x107, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(14, 2, 0);
  Actor_SetAnimation(14, 3);
  Event_ShowMessage(14, 0);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_ShowEmote(10, 0x108, 40);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(11, 0);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, 0x100, 20);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(11, 0);
  Actor_SetAttachedEffect(0xa, 0x102);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, 0, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, 0, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, 0, 40);
  Actor_FaceDirection(12, 0xb000, 0);
  Actor_FaceDirection(13, 0xb000, 0);
  Actor_FaceDirection(14, 0xd000, 0);
  Actor_FaceDirection(15, 0xd000, 0);
  Actor_FaceDirection(16, 0xd000, 0);
  Actor_FaceDirection(17, 0xd000, 20);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, 0x109, 40);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(10, 0xb000, 80);
  Actor_SetAnimationAndWait(10, 4);
  Event_ShowMessage(10, 0);
  Event_SetFlag(0x9c1);
  Func_0200446c();
}
