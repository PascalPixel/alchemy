#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02002a44();
s32 Func_02002b64();
s32 Func_02002adc();
s32 Actor_ArmCallback();
s32 Actor_Jump();
s32 Func_02002aec();
s32 Camera_MoveWithinMapBounds();
s32 Func_02002b3c();
s32 FieldEvent_Wait();
s32 Actor_StartRepeatedMotion();
s32 Func_02002b24();
s32 Func_02002b04();
s32 Func_02002b1c();
s32 Actor_SetSpeedParameters();
s32 Actor_GetById();
s32 Actor_OffsetPositionAndReset();
s32 Actor_CommitPositionAndActivate();
s32 Actor_WatchAndSetScript();
s32 Func_02002a9c();
void Func_02002a4c(void);
extern u8 MsgChampa49[];

void Func_02000188(void) {
  s32 t24;
  s32 t26;
  s32 t29;

  Call1(GameFlag_SetBit, 0x973);
  Func_02002a44();
  Call1(Func_02002b64, 0);
  Call1(Func_02002adc, (s32)MsgChampa49);
  Call3(Actor_ArmCallback, 32, 0x3000, 0);
  Call3(Actor_Jump, 32, 6, 20);
  Call3(Func_02002aec, 0x20, 0, 5);
  Call4(Camera_MoveWithinMapBounds, PIXELS(176), -1, PIXELS(256), 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_StartRepeatedMotion, 32, 3);
  Call2(Func_02002b24, 0x20, 0x102);
  Call1(FieldEvent_Wait, 40);
  Call2(Func_02002b04, 33, FACING_EAST);
  Call1(FieldEvent_Wait, 10);
  Func_02002b1c(0x21, 0x101, 0x23);
  Call2(Func_02002b04, 33, 0x3000);
  Call1(FieldEvent_Wait, 10);
  Func_02002b1c(0x21, 0x100, 0x1e);
  Call3(Func_02002aec, 0x21, 0, 5);
  Call3(Actor_Jump, 32, 6, 0);
  Call3(Actor_Jump, 33, 6, 0);
  Call3(Actor_SetSpeedParameters, 32, 0x1cccc, 0xe666);
  Call3(Actor_SetSpeedParameters, 33, 0x1cccc, 0xe666);
  t24 = Value1(Actor_GetById, 32);
  FIELD_AT((t24 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t24 + 0x5a), u8, 0));
  Call3(Actor_OffsetPositionAndReset, 32, 0, -16);
  t26 = Value1(Actor_GetById, 33);
  FIELD_AT((t26 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t26 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 33, 0, -16);
  Call1(FieldEvent_Wait, 1);
  t29 = Value1(Actor_GetById, 33);
  FIELD_AT((t29 + 0x5a), u8, 0) = (1 | FIELD_AT((t29 + 0x5a), u8, 0));
  Call1(FieldEvent_Wait, 15);
  Func_02002b1c(0x20, 0x109, 0x1e);
  Call3(Func_02002aec, 0x20, 0, 5);
  Call3(Actor_ArmCallback, 32, 0xb000, 0);
  Call2(Func_02002b04, 33, 0xd000);
  Call1(FieldEvent_Wait, 5);
  Call3(Func_02002aec, 0x8021, 0, 5);
  Call3(Func_02002aec, 0x22, 0, 5);
  Call2(Actor_WatchAndSetScript, 32, 34);
  Call2(Actor_WatchAndSetScript, 33, 34);
  Call3(Actor_SetSpeedParameters, 34, 0x13333, 0x9999);
  Call3(Actor_CommitPositionAndActivate, 34, 0, 32);
  Call2(Func_02002b04, 34, 0x3000);
  Call4(Camera_MoveWithinMapBounds, PIXELS(208), -1, PIXELS(296), 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 15);
  Call3(Actor_Jump, 34, 6, 15);
  Call3(Actor_Jump, 34, 6, 23);
  Call3(Func_02002aec, 0x22, 0, 5);
  Call3(Actor_Jump, 34, 6, 20);
  Call3(Actor_SetSpeedParameters, 34, 0x20000, 0x10000);
  Call3(Actor_CommitPositionAndActivate, 34, 0, -48);
  Call3(Actor_ArmCallback, 32, 0xb000, 0);
  Call3(Actor_ArmCallback, 33, 0xd000, 0);
  Call3(Func_02002a9c, 34, 0, 0);
  Call3(Actor_SetSpeedParameters, 32, 0x20000, 0x10000);
  Call3(Actor_SetSpeedParameters, 33, 0x20000, 0x10000);
  Call3(Actor_CommitPositionAndActivate, 32, -16, -16);
  Call3(Actor_OffsetPositionAndReset, 33, 16, -16);
  Call3(Actor_CommitPositionAndActivate, 32, 0, -16);
  Call3(Func_02002a9c, 32, 0, 0);
  Call3(Actor_CommitPositionAndActivate, 33, 0, -16);
  Call3(Func_02002a9c, 33, 0, 0);
  Func_02002a4c();
}
