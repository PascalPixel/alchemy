// Draft, differ 81 of 122, skeleton gap 13 (tla-en, 2026-11-01)
// Overlay 691's copy of the debris burst in resource_697_c_02004a94.c and
// resource_692_c_020046e8.c: eight pieces of actor type 285 around the leader.
// The statements are right; what is left is allocation. The listing has leader
// r7, spread r8, i r10, frame r9; this draft has i r7, leader r8, spread r10.
// Ranks from the lreg dump (log2(refs) * refs / length): i 3*11/190, leader
// 2*7/96, spread 2*7/102. i needs at most 7 refs, or leader and spread 8.
// Proved: one more ref each outside the loop gives the listing's registers.
// __asm__("" : : "r"(leader), "r"(spread)); after the loop gives differ 6, gap
// 2. That is a barrier, which comes out because it does not reach differ 0.
// Under it the y local (computed before the x sum) lands the x and y constants
// in r0 and r3 as the listing does; without y, differ 26 to 30.
// The 6 left under the barrier are the -13 mask store. The listing has ldrb r3,
// mask r2, ands r3, r2, then 4 in r2, with the sprite load after both scale
// stores. As written here (u8 value) combine puts the mask first in the and, so
// the result ties to the mask: ldrb r2, mask r3, and the load goes early.
// s8 value: ldrb r3, mask r2, but the result is still tied to the mask (8). No
// value local: the result ties to the ldrb, but the QI mask and the 4 outrank
// it in local-alloc and take r3 (14). The bit-field gives 18, or 30 without y.
// A one-constant mask local loses the reload through r0 and rotates the spills
// (56 to 64). To finish: keep the and's operands in load-then-mask order with
// value, or rank the load's quantity above the mask's; both need a barrier
// accepted first.
// Tried under the barrier: goto loop (58), scale store orders (32), x + i << 17
// (83), raw u8 masks (69), wrappers (14 to 64), one-pass around the and (8), a
// constant local for the type (no effect). Without the barrier: for loop (96).
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_02003e0c[];

struct Actor *Actor_GetById(s32 id);
struct FieldActor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
u32 Func_02003bd4(struct FieldSprite *sprite, u32 frame);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_02003a54(struct FieldEffect *effect, s32 mode);
void Func_02003a5c(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

void Func_020034ac(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    s32 y;

    y = leader->y - (((Random16Far() * 14) >> 16) << 16);
    effect = (struct FieldEffect *)Import_ActorCreate(
        285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    if (effect != 0) {
      frame = Func_02003bd4(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_02003a54(effect, 2);
      Func_02003a5c(effect, gData_02003e0c);
      effect->scaleY = effect->scaleX = 0xb333;
      { s8 *p = (s8 *)effect->sprite; u8 value = p[9] & -13; value |= 4; p[9] = value; }
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
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}
