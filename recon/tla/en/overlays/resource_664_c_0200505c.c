// Draft, differ 10 of 172 (was 83): inside an event, the leader drops from
// height 130 after a screen transition (cue 204), lands in a ring of
// seventeen dust effects, plays cue 188 and mode 22 while the screen shakes
// twice, then gets its normal gravity back. The same listing as overlay 68f's
// Func_020011e0; the ring is written as overlay 655's Func_02003764. The
// count is cleared between the options' palette and update, so global.c
// ranks &options above it and they take r8 and r10 as the native's do (with
// the count cleared in the loop's own first clause they swap).
// Left: the native clears the count after the options' stores and builds the
// velocity's address after the first call of each pass (here once, before
// the loop). Measured: the count cleared at its declaration, before the
// options or after them (16 to 40), the loop as do, while or goto, an options
// pointer; permute found nothing better.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "call.h"

struct Actor *Actor_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005d84(void);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02005ec4(u32 id, s32 mode);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005dd4(void);

void Func_0200505c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

  Func_02005e14();
  Func_02005f2c(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02005d84();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005f94(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  i = 0;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 12);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
    Func_020002f4(
        leader->x, leader->y, leader->z, velocity[0], velocity[1], velocity[2],
        EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
        &options);
  }
  Func_02005f94(188);
  Func_02005ec4(gMainRoutineWork.currentOwner, 257);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())Func_02005dcc, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
  Func_02005dd4();
  Func_02005ec4(gMainRoutineWork.currentOwner, 256);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Func_02005e1c();
}
