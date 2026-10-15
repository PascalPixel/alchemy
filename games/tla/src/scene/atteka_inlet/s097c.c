#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"
#include "call.h"

void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001060(s32 id, s32 x, s32 z);
void Func_02001068(s32 id, s32 x, s32 z, s32 angle);
void Func_02001098(s32 message);
void Func_020010a8(s32 id, s32 value);
void Func_020010b8(s32 id, s32 value);
s32 Func_020010c8(s32 id, s32 emote, s32 frames);
void Func_020010d0(s32 a, s32 b);
struct ObjectRuntime *Func_020010e8(void);
void Func_02001108(s32 value);

// Object 24 calls out to the party, jumps down to it and talks with object
// 23; flag 0x9ad is set.
void Func_0200097c(void) {
  s32 angle;

  Func_02001010();
  Call1(Func_02001108, 0);
  angle = 0xd000;
  Func_02001068(24, 0x1f80000, 0x1cc0000, angle);
  Call1(WaitFrames, 1);
  Call1(Func_02001098, (s32)MsgHeyWait2);
  Call2(Func_020010a8, 24, 0);
  Call3(ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 23, 0x3000, 0);
  Call2(Func_020010d0, 0x19999, 0x3333);
  Func_020010e8()->motionFlags = 0;
  Call4(Motion_CamBounds, 0x2000000, -1, 0x17c0000, 1);
  Call3(ObjectMotion_SetSpeedParameters, 24, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetPositionAndReset, 24, 520, 432);
  Call3(ObjectMotion_SetPositionAndReset, 24, 520, 388);
  Call3(ObjectMotion_ArmCallback, 23, 0x3000, 0);
  Call2(Func_020010a8, 24, 0);
  Value3(Func_020010c8, 23, 256, 10);
  Call2(Func_020010a8, 23, 0);
  Value3(Func_020010c8, 24, 265, 0);
  Call2(Func_020010a8, 24, 0);
  Call2(Motion_SetModeAndWaitAnimation, 23, 3);
  Func_020010b8(24, angle);
  Object_GetById(24)->actionFlags &= ~1;
  Call3(ObjectMotion_SetPositionAndReset, 24, 520, 384);
  Call1(Battle_WaitMode0, 1);
  Object_GetById(24)->actionFlags |= 1;
  Call2(Motion_SetVarCbAndRefresh, 24, 2);
  Call3(Func_02001060, 65, 0x2080000, 0x17c0000);
  Call1(WaitFrames, 1);
  Object_GetById(24)->actionFlags &= ~1;
  Call3(ObjectMotion_SetPositionAndReset, 24, 520, 388);
  Call1(Battle_WaitMode0, 1);
  Object_GetById(24)->actionFlags |= 1;
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_ArmCallback, 24, 0xb000, 10);
  Call2(Motion_SetModeAndWaitAnimation, 24, 3);
  Call3(ObjectMotion_SetPositionAndReset, 24, 520, 434);
  Call4(Motion_CamBounds, 0x2000000, -1, 0x15c0000, 1);
  Call3(ObjectMotion_SetPositionAndReset, 24, 504, 460);
  Call3(Func_02001060, 24, 0, 0);
  Func_020010b8(23, angle);
  Call2(Func_020010a8, 23, 0);
  Value3(Func_020010c8, 23, 265, 0);
  Call2(Func_020010a8, 23, 0);
  Call2(Object_SetModeById, 23, 3);
  Call2(Func_020010a8, 23, 0);
  Call2(Motion_SetModeAndWaitAnimation, 23, 3);
  Value3(Func_020010c8, 23, 264, 0);
  Call2(Func_020010a8, 23, 0);
  Call1(GameFlag_SetBit, 0x9ad);
  Func_02001018();
}
