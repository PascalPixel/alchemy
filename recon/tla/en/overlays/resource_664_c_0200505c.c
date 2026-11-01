// Draft, differ 16 of 172, skeleton gap 1 (tla-en, 2026-11-01)
// Twin of The Broken Seal's VenusLighthouse_LandWithRadialDust: inside an
// event, the leader drops from height 130 after a screen transition (cue 204),
// lands in a ring of seventeen dust effects, plays cue 188 and mode 22 while
// the screen shakes twice, then gets its normal gravity back. This game adds
// random spread to the velocity and the screen effect stores. The same listing
// as overlay 68f's Func_020011e0 and overlay 655's Func_02003764.
// The loop is the twin's: for from 0, angle set first, so the count is cleared
// after the options' stores as in the native (gap 3 to 1). Differ rose from
// 10 because that form swaps the registers: the native keeps &options in r8
// and the count in r10; with the count cleared between palette and update
// they fall right (differ 10) but the clear is in the wrong place (gap 3).
// Left: that swap, and the native builds the velocity's address after the
// first call of each pass. Here it leaves the loop: loop.c moves it in the
// for form and gcse's PRE before a goto loop (TBS's hoists too).
// Measured without gain: do, while and goto loops, an options pointer set
// at the top, after the stores or after the count, a velocity pointer set
// after the first call, struct Vector3, angle and velocity declared outside
// the loop or in either order, a u16 count; permute, three minutes.
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
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
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
