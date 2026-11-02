// Matches: differ 0 (tla-en, 2026-11-02)
// Also differ 0 in tla-ja, de, es, fr and it by the same diff (not a build).
// Seventeen effects (type 285) burst from the leader, one 1/16 turn apart, the
// sibling of Func_020029d0. The names are guesses: nothing here is from the
// game's text.
// The one thing to rule on is `zero`. Its default is S2's `// Default:` form,
// but it is what makes the match: with it the loop's `zero = 0` is the local's
// second set, so loop.c leaves it in the body, and the unread default is then
// deleted. Without the default, or without the set in the loop, the constants
// are hoisted, differ 95 of 116, skeleton gap 8/7. The review has this kind of
// steering default open (6b8_0ad0), so adoption should ask whether it counts as
// FAKEMATCH(dead). As a goto loop with one set, differ 45: that also keeps zero
// in the body, but loses the hoisted constants that reload rebuilds, such as
// the -13 mask and 285.
#include "types.h"

union FieldObject;

#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "field_sprite.h"

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct FieldEffect *object, s32 flags);
struct FieldEffect *Func_02002f98(s32 type, s32 x, s32 y, s32 z);
void Func_02002f80(struct FieldEffect *object, s32 mode);
void Func_02002f90(struct FieldEffect *object, const void *script);
s32 Func_02003150(struct FieldSprite *sprite, s32 state);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
extern const u8 gData_02003304[];

void Func_02002b04(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 state;
  u32 i;
  // Default: without it the set in the loop is hoisted.
  s32 zero = 0;

  if (leader != NULL) {
    state = 0;
    for (i = 0; i < 17; i++) {
      struct FieldEffect *effect =
          Func_02002f98(285, leader->x, leader->y, leader->z);

      if (effect != NULL) {
        s32 angle;

        state = Func_02003150(effect->sprite, state);
        zero = 0;
        effect->motionFlags = zero;
        effect->collisionFlags = zero;
        Actor_SetSingleChildFlags(effect, 0);
        Func_02002f80(effect, 2);
        Func_02002f90(effect, gData_02003304);
        angle = i << 12;
        effect->velocity.x = Math_Cosine(angle);
        effect->velocity.y = zero;
        effect->velocity.z = Math_Sine(angle);
        effect->velocity.x += effect->velocity.x >> 1;
        effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
        effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
        effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
        effect->scaleRateX = zero;
        effect->scaleRateY = zero;
        effect->update = Effect_AdvanceMotionAtLeaderPriority;
        effect->sprite->priority =
            ((struct FieldSprite *)leader->animation)->priority;
      }
    }
  }
}
