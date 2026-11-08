#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
void GameFlag_SetBit(s32 a);
void Func_02000a94(void);
void Func_02000b8c(s32 a);
void Func_02000b24(s32 a);
void Func_02000b84(s32 a, s32 b);
void Actor_SetPositionAndReset(s32 a, s32 b, s32 c);
void Actor_ArmCallback(s32 a, s32 b, s32 c);
void Func_02000b7c(void);
void FieldEvent_Wait(s32 a);
void Func_02000b34(s32 a, s32 b, s32 c);
void Actor_StartRepeatedMotion(s32 a, s32 b);
void Func_02000b64(s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 a, s32 b);
void Motion_SetVarCbAndRefresh(s32 a, s32 b);
void Camera_FollowActor(s32 a, s32 b);
void Func_02000a9c(void);
extern u8 MsgAlhafraInteriors31[];
extern u8 gMainRoutineWork[];

void Func_02000084(void) {
  GameFlag_SetBit(SPLIT_CONSTANT(0x800, 178));
  Func_02000a94();
  Func_02000b8c(0);
  Func_02000b24((s32)MsgAlhafraInteriors31);
  Func_02000b84(0x1f, 1);
  Call3(Actor_SetPositionAndReset, 4, 452, 335);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Func_02000b7c();
  Actor_ArmCallback(31, 0x2000, 0);
  FieldEvent_Wait(20);
  Call3(Func_02000b34, SPLIT_CONSTANT(0x8000, 31), 0, 0xa);
  Actor_StartRepeatedMotion(4, 3);
  Func_02000b64(4, 0x102);
  FieldEvent_Wait(50);
  Motion_SetModeAndWaitAnimation(31, 3);
  FieldEvent_Wait(10);
  Call3(Func_02000b34, SPLIT_CONSTANT(0x8000, 31), 0, 0xa);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(31, 4);
  Call3(Func_02000b34, SPLIT_CONSTANT(0x8000, 31), 0, 0xa);
  Motion_SetVarCbAndRefresh(31, 2);
  FieldEvent_Wait(10);
  Call3(Func_02000b34, SPLIT_CONSTANT(0x8000, 31), 0, 0xa);
  Motion_SetModeAndWaitAnimation(31, 3);
  FieldEvent_Wait(10);
  Func_02000b34(SPLIT_CONSTANT(0x8000, 31), 0, 0xa);
  Actor_ArmCallback(31, 0xc000, 0);
  FieldEvent_Wait(20);
  Camera_FollowActor(FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_02000b7c();
  Func_02000a9c();
}
