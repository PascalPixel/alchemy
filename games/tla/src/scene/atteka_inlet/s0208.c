#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Func_02000ff8(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001068(s32 id, s32 x, s32 z, s32 angle);
void Func_02001098(s32 message);
void Func_020010a8(s32 id, s32 value);
void Func_020010b8(s32 id, s32 value);
s32 Func_020010c8(s32 id, s32 emote, s32 frames);
void Func_02001108(s32 value);

/// Once flag 0x9d0 is set and while flag 0x9b2 is clear, objects 9 and 10 are
/// placed, jump in and talk, four cells of the map change and flag 0x9b0 is set.
void Func_02000208(void) {
  if (GameFlag_Test(0x9d0) && !GameFlag_Test(0x9b2)) {
    Func_02001010();
    Call1(Func_02001108, 0);
    Call4(Camera_MoveWithinMapBounds, 96 << 16, -1, 308 << 16, 1);
    {
      /* FAKEMATCH: the angle is shifted from a local so that it is built first,
       * as the native builds it; as a constant it is built last (differ 2). */
      s32 angle = 208;

      Call4(Func_02001068, 9, 60 << 16, 362 << 16, angle << 8);
    }
    Call1(wait, 1);
    Call3(Actor_SetSpeedParameters, 9, 0x20000, 0x10000);
    Call3(Actor_Jump, 9, 6, 0);
    Call3(Actor_SetPositionAndCommit, 9, 64, 336);
    Call3(Actor_SetPositionAndReset, 9, 70, 308);
    Call3(Actor_ArmCallback, 9, FACING_EAST, 0);
    {
      /* FAKEMATCH: as above (differ 2). */
      s32 angle = 160;

      Call4(Func_02001068, 10, 94 << 16, 272 << 16, angle << 7);
    }
    Call1(wait, 1);
    Call3(Actor_SetSpeedParameters, 10, 0x20000, 0x10000);
    Call3(Actor_Jump, 10, 6, 0);
    Call3(Actor_SetPositionAndCommit, 10, 90, 288);
    Call3(Actor_SetPositionAndReset, 10, 80, 316);
    Call2(Func_020010b8, 10, FACING_EAST);
    Actor_GetById(9)->velocityY = 0x40000;
    Call1(FieldEvent_Wait, 10);
    Call1(Func_02001098, (s32)MsgWhereDoYouThink);
    Call2(Func_020010a8, 0x4009, 0);
    Value3(Func_020010c8, 10, 265, 0);
    Call2(Func_020010a8, 10, 0);
    Call3(Actor_ArmCallback, 9, 0x3000, 0);
    Call2(Func_020010a8, 0x4009, 0);
    Call2(Func_020010b8, 10, 0xb000);
    Call2(Actor_SetModeById, 10, 3);
    Call2(Func_020010a8, 10, 0);
    Call3(Actor_ArmCallback, 10, FACING_EAST, 0);
    Value3(Func_020010c8, 9, 264, 0);
    Call3(Actor_ArmCallback, 9, FACING_EAST, 20);
    Call2(Func_020010a8, 0x4009, 0);
    Call2(Func_020010a8, 10, 0);
    Func_02000ff8(1, 16, 1, 1, 3, 19);
    Func_02000ff8(1, 16, 1, 1, 4, 19);
    Func_02000ff8(1, 16, 1, 1, 4, 20);
    Func_02000ff8(1, 16, 1, 1, 4, 21);
    Call1(GameFlag_SetBit, 0x9b0);
    Func_02001018();
  }
}
