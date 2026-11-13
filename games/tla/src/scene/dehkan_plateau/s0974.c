/// @file
/// Scene code of overlay 0x655, in ROM order.

#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"

extern const s32 gData_02003e00[];

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

/// Eight puffs (type 285) in a row over the leader, flung outwards when spread
/// is set.
void Func_02000974(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Func_02003d80(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_02003ba0(effect, 2);
      Actor_StartScript(effect, gData_02003e00);
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

void Func_020006ec(struct Actor *object, struct Actor *other);

void Actor_MoveToObject(s32 id, s32 target);

extern const u8 gData_02003e30[];

void Func_02000aa8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 frame;
  u32 i;

  if (leader == NULL)
    return;
  frame = 0;
  for (i = 0; i <= 16; i++) {
    {
      struct Actor *object = ((struct Actor *)Import_ActorCreate(
          285, leader->x, leader->y, leader->z));
      struct EffectView *effect = (struct EffectView *)object;

      if (object == NULL)
        goto skip;
      frame = Func_02003d80(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      Func_02003ba0(object, 2);
      Actor_StartScript(object, gData_02003e30);
      effect->velocityX = Math_Cosine(i << 12);
      effect->velocityY = 0;
      effect->velocityZ = Math_Sine(i << 12);
      effect->velocityX += effect->velocityX >> 1;
      effect->velocityX += (((Random16Far() * 3) << 14) >> 16) + -0x6000;
      effect->velocityZ += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
      effect->spin = (((u32)Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct EffectView *)leader)->sprite->priority;
    }
  skip:
  }
}
