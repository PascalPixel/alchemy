#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02001be4();
s32 Func_02001cfc();
s32 Func_02001c74();
s32 Camera_MoveWithinMapBounds();
void Func_02001ccc(void);
s32 FieldEvent_Wait();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02001c84();
s32 Actor_ArmCallback();
s32 Actor_SetModeById();
s32 Func_02001d4c();
s32 Func_02001cec();
s32 Actor_Jump();
s32 Motion_SetModeAndWaitAnimation();
s32 Func_02001ca4();
s32 Camera_FollowActor();
s32 Func_02001bec();
extern u8 MsgAlhafra74[];
extern u8 gMainRoutineWork[];

void Func_0200047c(void) {
  Call1(GameFlag_SetBit, 0x8b7);
  Func_02001be4();
  Call1(Func_02001cfc, 0);
  Call1(Func_02001c74, (s32)MsgAlhafra74);
  Call4(Camera_MoveWithinMapBounds, 0x2300000, -1, 0x1f00000, 1);
  Func_02001ccc();
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetVarCbAndRefresh, 29, 2);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001c84, 0x1d, 0, 0xa);
  Call3(Actor_ArmCallback, 28, 0, 0);
  Call3(Actor_ArmCallback, 31, 0x6000, 0);
  Call3(Actor_ArmCallback, 30, 0x6000, 0);
  Call2(Actor_SetModeById, 29, 5);
  Call1(Func_02001d4c, 0x59);
  FIELD_AT(gMainRoutineWork, s32, 532) = 0x1d;
  Call2(Func_02001cec, 0, 3);
  FIELD_AT(gMainRoutineWork, s32, 532) = 4;
  Call1(FieldEvent_Wait, 50);
  Call3(Actor_Jump, 28, 6, 15);
  Call3(Actor_Jump, 28, 6, 23);
  Call3(Func_02001c84, 0x1c, 0, 0xa);
  Call3(Actor_ArmCallback, 29, 0x8000, 0);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetModeAndWaitAnimation, 29, 3);
  Call1(FieldEvent_Wait, 20);
  Func_02001ca4(0x1e, 0x108, 0x1e);
  Call3(Func_02001c84, 0x1e, 0, 0xa);
  Func_02001ca4(0x1d, 0x102, 0x28);
  Call3(Actor_ArmCallback, 29, 0x4000, 0);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetVarCbAndRefresh, 29, 2);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001c84, 0x1d, 0, 0xa);
  Call2(Motion_SetVarCbAndRefresh, 30, 2);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001c84, 0x1e, 0, 0xa);
  Call3(Actor_ArmCallback, 29, 0, 0);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetModeAndWaitAnimation, 29, 3);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_Jump, 31, 6, 15);
  Call3(Actor_Jump, 31, 6, 23);
  Call3(Func_02001c84, 0x1f, 0, 0xa);
  Call3(Actor_ArmCallback, 30, 0, 0);
  Call2(Motion_SetModeAndWaitAnimation, 30, 3);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_ArmCallback, 31, 0x4000, 0);
  Call2(Actor_SetModeById, 31, 6);
  Call3(Actor_ArmCallback, 30, 0xc000, 0);
  Call2(Actor_SetModeById, 30, 6);
  Call3(Actor_ArmCallback, 29, 0x4000, 0);
  Call3(Actor_ArmCallback, 28, 0xc000, 0);
  Call2(Camera_FollowActor, FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_02001ccc();
  Func_02001bec();
}
