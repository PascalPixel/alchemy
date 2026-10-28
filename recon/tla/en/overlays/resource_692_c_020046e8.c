// Draft, differ 92 of 122: eight pieces of debris in a row across the leader
// (shared with overlay 697 and the other trap maps), shrinking and spinning as
// they fall; with spread set they also fly out along eight angles.
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_02005638[];

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
u32 Func_02004e98(struct FieldSprite *sprite, u32 frame);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_02004c98(struct FieldEffect *effect, s32 mode);
void Func_02004ca0(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

void Func_020046e8(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    effect = (struct FieldEffect *)Import_ActorCreate(
        285, leader->x + (i << 17) - 0x80000,
        leader->y - (((Random16Far() * 14) >> 16) << 16) - 0x20000, leader->z);
    if (effect != 0) {
      frame = Func_02004e98(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_02004c98(effect, 2);
      Func_02004ca0(effect, gData_02005638);
      effect->scaleX = effect->scaleY = 0xb333;
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
