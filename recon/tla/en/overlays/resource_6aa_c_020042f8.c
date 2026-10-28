// Draft, differ 16 of 169 (was 63), rewritten from the listing: the leader is
// dropped from 0x82 pixels with the screen transition, 17 sparks
// (Func_02003b30, update Effect_AdvanceMotion) burst around it and it lands
// with the screen shaken. The camera and Func_02004c50 calls go through inline
// wrappers (FAKEMATCH(wrapper) on adoption). Left: gcse's PRE hoists the
// velocity's frame address out of the loop into r6 and the counter and options
// address swap (r8, sl); the native rebuilds sp+16 after each Math_Cosine.
// Tried: a goto loop and the velocity declared in the loop body (no change).
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "field_effect.h"

void Effect_AdvanceMotion(union FieldObject *object);
void Event_StartScreenTransition(void);
void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02004bf0(void);
void Func_02004c50(s32 a, s32 b, s32 c);
void Func_02004c58(void);
void Func_02004d08(s32 id, s32 value);

static __inline__ void Func_02004c50_(s32 a, s32 b, s32 c) {
  Func_02004c50(a, b, c);
}

void Func_020042f8(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Func_02004ca8();
  Func_02004da0(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_02004bf0();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  SCENE_WORK->unk1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004de0(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
next:
  {
    s32 angle = i << 12;

    velocity.x = Math_Cosine(angle);
    velocity.y = 0;
    velocity.z = Math_Sine(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
    Func_02003b30(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                  velocity.z, 0x1090001, &options);
  }
  if (++i <= 16)
    goto next;
  Func_02004de0(188);
  Func_02004d08(gSequenceParentWork.currentOwner, 0x101);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 49);
  Func_02004c50_(0x50000, 0x50000, 0x10000);
  Func_02004c50_(-1, -1, 0xe666);
  Func_02004c58();
  Func_02004d08(gSequenceParentWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 1);
  Func_02004cb0();
}
