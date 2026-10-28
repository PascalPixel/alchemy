// Draft, differ 93 of 116: seventeen pieces of debris bursting out of the leader in
// a ring (shared with overlay 697 and the other trap maps), at the leader's
// sprite priority.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_02005668[];

struct ObjectRuntime *Object_GetById(s32 id);
u32 Random16Far(void);
u32 Func_02004e98(struct FieldSprite *sprite, u32 frame);
void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_02004c98(struct FieldEffect *effect, s32 mode);
void Func_02004ca0(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

void Func_0200481c(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Engine_ObjectCreate(285, leader->x,
                                                       leader->y, leader->z);
    if (effect != 0) {
      frame = Func_02004e98(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      ObjectDispatch_SetSingleChildFlags(effect, 0);
      Func_02004c98(effect, 2);
      Func_02004ca0(effect, gData_02005668);
      effect->velocity.y = 0;
      effect->velocity.x = Math_Cosine(i << 12);
      effect->velocity.z = Math_Sine(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->animation)->priority;
    }
    i++;
  } while (i <= 16);
}
