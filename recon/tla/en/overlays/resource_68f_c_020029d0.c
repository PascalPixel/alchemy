// Draft, differ 87 of 122, skeleton gap 16 (tla-en, 2026-11-01)
// Eight effects in a row across the leader, each with a random height; mode
// nonzero sends them out in a fan. Statements, fields, shifts, pool loads and
// the shared velocity.z tail (velocityZ) are the native's. What is left is
// allocation, and the skeleton gap follows from it: i in a high register adds
// its reloads, which rotates every scratch register and moves the sprite load.
// The native has leader r7, mode r8, i r10, state r9. Here it is i r7, leader
// r8, state r10, mode r9. Priorities from the .17.lreg dump (floor_log2(refs) *
// refs / live): i 3*11/186, leader 2*7/94, state 2*5/184, mode 1*3/100; i and
// state are doubled by their zero. Goto, do-while, s32 i with an unsigned
// compare, declaration order and mode written into the else arm (7 refs) leave
// i first. Dropping loop weight from refs would give leader, mode, i, state,
// the native's order exactly: one more function for the -Os lead, a compiler
// question for Pascal (K3), not tested. Permute 2 minutes reached differ 48 to
// 57 only with a dead second y and an `if (leader)` that adds refs: dead code,
// rejected. Also tried with no gain: chained scales, sprite, x and scale
// locals, argument forms. The sibling Func_02002b04 has the same i and state.
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_020032d4[];

struct Actor *Actor_GetById(s32 id);
struct FieldActor *Func_02002f98(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
u32 Func_02003150(struct FieldSprite *sprite, u32 frame);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_02002f80(struct FieldEffect *effect, s32 mode);
void Func_02002f90(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

void Func_020029d0(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Func_02002f98(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Func_02003150(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_02002f80(effect, 2);
      Func_02002f90(effect, gData_020032d4);
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
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}
