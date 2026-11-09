#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "flag_call.h"
#include "event_inline.h"

extern u32 gFrameCount;
extern const u8 gData_020068b0[];

u32 Random16Far(void);
void Func_02005764(struct Actor *object, const void *script);
void Func_02000090(void);

static inline struct Actor *Actor_Create(s32 type, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the effect created through an inline, which
   * evaluates the position before the type; as a direct call of
   * Import_ActorCreate, 4 of 94 instructions differ in tla-en. */
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

/// Every third frame, or every frame once flag 0x201 is set, spawns a particle
/// at a random height above object 31 that circles it (Func_02000090).
void Func_020004c8(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Flag_IsSet(0x201) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_GetById(31);
  if (Flag_IsSet(0x201) != 0) {
    u32 random;

    random = Random16Far();
    effect = Actor_Create(672, center->x,
                          (s32)((u32)(random << 8) >> 16 << 16) + center->y -
                              0x1c0000,
                          center->z);
  } else {
    u32 random;

    random = Random16Far();
    effect = Actor_Create(672, center->x,
                          (s32)((u32)(random << 6) >> 16 << 16) + center->y -
                              0x1c0000,
                          center->z);
  }
  if (effect != 0) {
    sprite = effect->animation;
    Func_02005764(effect, gData_020068b0);
    Actor_SetPartAttribute(effect, 1);
    effect->motionFlags = 0;
    angle = Random16Far() & 0xffff000;
    effect->action = angle;
    effect->unk66 = 0;
    effect->update = Func_02000090;
    effect->speedLimit =
        Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}
