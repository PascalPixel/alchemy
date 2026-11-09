#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "event_inline.h"

extern const u8 gData_02003e3c[];

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_02003bd4(struct FieldSprite *sprite, s32 state);
void Func_02003a54(struct FieldEffect *object, s32 mode);
void Func_02003a5c(struct FieldEffect *object, const void *script);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

void Func_020035e0(void) {
  struct Actor *leader;
  struct FieldEffect *effect;
  s32 state;
  u32 i;
  s32 angle;
  u8 zero;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader == 0)
    return;
  // Default: without it loop.c lifts the set of zero out of the loop.
  zero = 0;
  state = 0;
  for (i = 0; i <= 16; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                      leader->z);
    if (effect != 0) {
      state = Func_02003bd4(effect->sprite, state);
      zero = 0;
      effect->motionFlags = zero;
      effect->collisionFlags = zero;
      Actor_SetSingleChildFlags((struct Actor *)effect, 0);
      Func_02003a54(effect, 2);
      Func_02003a5c(effect, gData_02003e3c);
      angle = i << 12;
      effect->velocity.x = Math_Cosine(angle);
      effect->velocity.y = zero;
      effect->velocity.z = Math_Sin(angle);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += (Random16Far() * 0xc000 >> 16) - 0x6000;
      effect->velocity.z += (Random16Far() * 0x6000 >> 16) - 0x3000;
      effect->spin = (Random16Far() * 0x1000 >> 16) - 0x800;
      effect->scaleRateX = zero;
      effect->scaleRateY = zero;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->animation)->priority;
    }
  }
}
