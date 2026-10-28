// Draft, differ 16 of 177 (was 49): the leader drops in with a burst of 17
// effects. The two camera shakes go through Call3, as the native builds each
// call's constants afresh. Left: register choice in the burst loop (the
// native keeps the options' address in r8 and the counter in r10, here the
// other way round) and the velocity's address, which the native forms after
// the cosine call in each pass; Func_02001474 in overlay 654 has the same
// pair. Five permuter minutes found nothing closer.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "field_effect.h"
#include "event_work.h"
#include "call.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02005158(void);
void Func_02005198(s32 a, s32 b, s32 c);
void Func_020051a0(void);
void Func_020053d8(s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02002ec4(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct EffectOptions options;
  u32 i;

  Func_020051c0();
  Func_02005370(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_02005158();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020053d8(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 12);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
    Func_020000b8(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                  velocity[2], 0x01090001, &options);
  }
  Func_020053d8(188);
  Func_020052d0(gAdventure.currentOwner, 0x101);
  Actor_SetModeById(gAdventure.currentOwner, 49);
  Call3((void (*)())Func_02005198, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02005198, -1, -1, 0xe666);
  Func_020051a0();
  Func_020052d0(gAdventure.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gAdventure.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_020051c8();
}
