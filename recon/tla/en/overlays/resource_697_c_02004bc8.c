// Draft, differ 93 of 116: overlay 697's copy of the function drafted in
// resource_692_c_0200481c.c (overlay 692's Func_0200481c, differ 93 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_02006284[];

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

void Func_02004bc8(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Func_020057d8(285, leader->x,
                                                       leader->y, leader->z);
    if (effect != 0) {
      frame = Func_020059f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_020057c8(effect, 2);
      Func_020057d0(effect, gData_02006284);
      effect->velocity.y = 0;
      effect->velocity.x = Math_Cosine(i << 12);
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
