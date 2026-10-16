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
void ObjectMotion_SetPositionAndReset(s32 a, s32 b, s32 c);
void ObjectMotion_ArmCallback(s32 a, s32 b, s32 c);
void Func_02000b7c(void);
void FieldEvent_WaitFrames(s32 a);
void Func_02000b34(s32 a, s32 b, s32 c);
void ObjectMotion_SetVariantCallback(s32 a, s32 b);
void Func_02000b64(s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 a, s32 b);
void Motion_SetVarCbAndRefresh(s32 a, s32 b);
void Camera_FollowActor(s32 a, s32 b);
void Func_02000a9c(void);
extern u8 MsgAlhafraInteriors31[];
extern u8 gPartyState[];

void Func_02000084(void) {
  Call1(GameFlag_SetBit, 0x8b2);
  Func_02000a94();
  Call1(Func_02000b8c, 0);
  Call1(Func_02000b24, (s32)MsgAlhafraInteriors31);
  Call2(Func_02000b84, 0x1f, 1);
  Call3(ObjectMotion_SetPositionAndReset, 4, 0x1c4, 0x14f);
  Call3(ObjectMotion_ArmCallback, 4, 0x8000, 0);
  Func_02000b7c();
  Call3(ObjectMotion_ArmCallback, 0x1f, 0x2000, 0);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call3(Func_02000b34, 0x801f, 0, 0xa);
  Call2(ObjectMotion_SetVariantCallback, 4, 3);
  Call2(Func_02000b64, 4, 0x102);
  Call1(FieldEvent_WaitFrames, 0x32);
  Call2(Motion_SetModeAndWaitAnimation, 0x1f, 3);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_02000b34, 0x801f, 0, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 4, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Motion_SetModeAndWaitAnimation, 0x1f, 4);
  Call3(Func_02000b34, 0x801f, 0, 0xa);
  Call2(Motion_SetVarCbAndRefresh, 0x1f, 2);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_02000b34, 0x801f, 0, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x1f, 3);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_02000b34, 0x801f, 0, 0xa);
  Call3(ObjectMotion_ArmCallback, 0x1f, 0xc000, 0);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Camera_FollowActor, FIELD_AT(gPartyState, s32, 532), 1);
  Func_02000b7c();
  Func_02000a9c();
}
