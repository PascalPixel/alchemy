// Draft, differ 90 of 122, skeleton gap 30 (tla-en, 2026-11-01)
// Overlay 697's copy of the debris burst drafted in resource_692_c_020046e8.c.
// The statements are right; what is left is allocation. The listing has leader
// in r7, spread in r8, i in r10, frame in r9; this draft has i in r7, leader in
// r8, spread in r10. Everything else in the gap follows from that: the shared
// str to velocity.z only cross-jumps when the else's reload lands in r0 (reload
// regs rotate), and the scale store stays ahead of sprite's load only when the
// constant and the ldrb share r3, an anti-dependency for sched2. A probe with
// i ranked below leader closed both, so keep x = y = spread in the else: a z
// local gets the shared store today (gap 29) but folds spread to movs #0.
// global.c's rank is log2(refs) * refs / length: i 3*11/186 = .177 (5 refs in
// the loop, length doubled by its constant start), leader 2*7/94, spread 2*7/100.
// i needs at most 7 refs, or leader and spread 8: one more ref outside the loop.
// Nothing found gives either in plain C. No effect from: for or ++i loop, i < 8,
// i = frame = 0 and its orders, a second i = 0, declaration order, an angle,
// scale or sprite local, an early goto for effect == 0. A local for the random
// value computes x before y, which the listing does not.
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_02006254[];

struct Actor *Actor_GetById(s32 id);
struct FieldActor *Func_020057d8(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
u32 Func_020059f0(struct FieldSprite *sprite, u32 frame);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_020057c8(struct FieldEffect *effect, s32 mode);
void Func_020057d0(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02004a14(union FieldObject *object);

void Func_02004a94(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Func_020057d8(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Func_020059f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_020057c8(effect, 2);
      Func_020057d0(effect, gData_02006254);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Math_Cosine(i << 13), 0x13333);
        effect->velocity.z = Math_Sine(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16Far() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16Far() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Func_02004a14;
    }
    i++;
  } while (i <= 7);
}
