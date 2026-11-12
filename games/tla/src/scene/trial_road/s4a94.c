#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"

extern const s32 gData_02006254[];

u32 Func_020059f0(struct FieldSprite *sprite, u32 frame);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_020057c8(struct FieldEffect *effect, s32 mode);
void Func_020057d0(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
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

      effect = (struct FieldEffect *)Import_ActorCreate(
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

extern const s32 gData_02006284[];

void Func_02004bc8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Import_ActorCreate(285, leader->x, leader->y,
                                                      leader->z);
    if (effect != 0) {
      frame = Func_020059f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_020057c8(effect, 2);
      Func_020057d0(effect, gData_02006284);
      effect->velocity.x = Math_Cosine(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sine(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Func_02004a14;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->animation)->priority;
    }
    i++;
  } while (i <= 16);
}
