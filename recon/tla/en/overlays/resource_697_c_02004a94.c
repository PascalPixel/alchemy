// Draft, differ 92 of 122: overlay 697's copy of the function drafted in
// resource_692_c_020046e8.c (overlay 692's Func_020046e8, differ 92 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_sprite.h"
#include "field_effect.h"

extern const s32 gData_02006254[];

struct ObjectRuntime *Object_GetById(s32 id);
struct FieldActor *Func_020057d8(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
u32 Func_020059f0(struct FieldSprite *sprite, u32 frame);
void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_020057c8(struct FieldEffect *effect, s32 mode);
void Func_020057d0(struct FieldEffect *effect, const s32 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02004a14(union FieldObject *object);

void Func_02004a94(s32 spread) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    effect = (struct FieldEffect *)Func_020057d8(
        285, leader->x + (i << 17) - 0x80000,
        leader->y - (((Random16Far() * 14) >> 16) << 16) - 0x20000, leader->z);
    if (effect != 0) {
      frame = Func_020059f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      ObjectDispatch_SetSingleChildFlags(effect, 0);
      Func_020057c8(effect, 2);
      Func_020057d0(effect, gData_02006254);
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
      effect->update = Func_02004a14;
    }
    i++;
  } while (i <= 7);
}
