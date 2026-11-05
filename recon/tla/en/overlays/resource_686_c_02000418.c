// Measured under -Os, 5 November 2026: differ 32 (tla-en); notes below may predate it.
// Matches: differ 0 (tla-en, 2026-11-02). The non-Japanese body of
// games/tla/src/scene/ankohl_ruins/s0418.c, adopted before this draft existed;
// both devices below came with it. The leader lands and 17 dust effects spread
// around it. Twin of VenusLighthouse_LandWithRadialDust in The Broken Seal
// confirmed: the same calls in the same order, the same 17 particles (i << 12),
// plus this game's 0x1ac writes and two random velocity terms.
// Without the barrier the velocity address is hoisted before the loop, and i
// and the options address take r8 and r10 the wrong way round (differ 16, gap
// 1). The barrier needs its "sfp" clobber and all three reads: sfp alone gives
// 12, effect alone 62, a bare asm or a "memory" clobber 16. Neither device
// could go. Tried, 16 at best: a wrapper around the cosine and sine, a wrapper
// around the whole body, a velocity pointer set in the loop, do/while, for.
// Entering at the bottom test gives 25, a one-pass block 26.
// tla-ja has no Func_02000418: the listing's section is never assembled.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"

struct LandingEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

struct LandingEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion();
void Func_02002e44(void);
void Func_02002edc(s32 value);
void Func_02002dec(void);
void Func_02002eec(s32 cue);
void Func_02002e9c(s32 actor, s32 value);
void Func_02002e24(s32 a, s32 b, s32 c);
void Func_02002e2c(void);
void Func_02002e4c(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct LandingEffectOptions *options);

void Func_02000418(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct LandingEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];
  s32 z;
  struct LandingEffectOptions *effect;

  Func_02002e44();
  Func_02002edc(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02002dec();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002eec(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  effect->palette = 7;
  effect = &options;
  effect->update = Effect_AdvanceMotion;
  effect->scaleX = 0xa3d7;
  effect->scaleY = 0xa3d7;
  i = 0;
loop:
  angle = i << 12;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading effect and
   * leader; as plain C with no asm, 16 of 177 instructions differ in tla-en. */
  asm("" : : "r"(effect), "r"(leader), "r"(leader) : "sfp");
  velocity[0] = Math_Cosine(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sine(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  /* FAKEMATCH(temporary): z holding the third velocity for the call; as
   * velocity[2] += with velocity[2] passed, 47 of 177 instructions differ in
   * tla-en. */
  z = velocity[2] + ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  velocity[2] = z;
  Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1], z,
                0x1090001, effect);
  i++;
  if (i <= 16)
    goto loop;
  Func_02002eec(188);
  Func_02002e9c(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  Call3((void (*)())Func_02002e24, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Func_02002e2c();
  Func_02002e9c(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Func_02002e4c();
}

