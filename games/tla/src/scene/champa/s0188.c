#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02002a44();
s32 Func_02002b3c();
void Func_02002a4c(void);
extern u8 MsgChampa49[];

void Func_02000188(void) {
  s32 t24;
  s32 t26;
  s32 t29;

  GameFlag_SetBit(0x973);
  Func_02002a44();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgChampa49);
  Actor_FaceDirection(32, 0x3000, 0);
  FieldActor_Jump(32, 6, 20);
  FieldEvent_ShowMessageThenWait(0x20, 0, 5);
  FieldCamera_MoveTo(PIXELS(176), -1, PIXELS(256), 1);
  Func_02002b3c();
  Event_Wait(10);
  FieldActor_StartRepeatedMotion(32, 3);
  Func_080d489c(0x20, 0x102);
  Event_Wait(40);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(10);
  Func_080d47b4(0x21, 0x101, 0x23);
  Actor_FaceDirectionAndWait(33, 0x3000);
  Event_Wait(10);
  Func_080d47b4(0x21, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(0x21, 0, 5);
  FieldActor_Jump(32, 6, 0);
  FieldActor_Jump(33, 6, 0);
  Actor_SetSpeed(32, 0x1cccc, 0xe666);
  Actor_SetSpeedParameters(33, 0x1cccc, 0xe666);
  t24 = ((s32)Actor_GetById(32));
  FIELD_AT((t24 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t24 + 0x5a), u8, 0));
  Actor_WalkBy(32, 0, -16);
  t26 = ((s32)Actor_GetById(33));
  FIELD_AT((t26 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t26 + 0x5a), u8, 0));
  Actor_WalkByAndWait(33, 0, -16);
  Event_Wait(1);
  t29 = ((s32)Actor_GetById(33));
  FIELD_AT((t29 + 0x5a), u8, 0) = (1 | FIELD_AT((t29 + 0x5a), u8, 0));
  Event_Wait(15);
  Func_080d47b4(0x20, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x20, 0, 5);
  Actor_FaceDirection(32, 0xb000, 0);
  Actor_FaceDirectionAndWait(33, 0xd000);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(0x8021, 0, 5);
  FieldEvent_ShowMessageThenWait(0x22, 0, 5);
  FieldActor_WatchAndSetScript(32, 34);
  FieldActor_WatchAndSetScript(33, 34);
  Actor_SetSpeed(34, 0x13333, 0x9999);
  Actor_WalkByAndWait(34, 0, 32);
  Actor_FaceDirectionAndWait(34, 0x3000);
  Camera_MoveWithinMapBounds(PIXELS(208), -1, PIXELS(296), 1);
  Func_02002b3c();
  Event_Wait(15);
  FieldActor_Jump(34, 6, 15);
  FieldActor_Jump(34, 6, 23);
  FieldEvent_ShowMessageThenWait(0x22, 0, 5);
  FieldActor_Jump(34, 6, 20);
  Actor_SetSpeed(34, 0x20000, 0x10000);
  Actor_WalkByAndWait(34, 0, -48);
  Actor_FaceDirection(32, 0xb000, 0);
  Actor_FaceDirection(33, 0xd000, 0);
  Actor_SetPosition(34, 0, 0);
  Actor_SetSpeed(32, 0x20000, 0x10000);
  Actor_SetSpeed(33, 0x20000, 0x10000);
  Actor_WalkByAndWait(32, -16, -16);
  Actor_WalkBy(33, 16, -16);
  Actor_WalkByAndWait(32, 0, -16);
  Actor_SetPosition(32, 0, 0);
  Actor_WalkByAndWait(33, 0, -16);
  Actor_SetPosition(33, 0, 0);
  Func_02002a4c();
}
