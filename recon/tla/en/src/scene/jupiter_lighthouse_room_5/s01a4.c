#include "types.h"
#include "call.h"
#include "field_event.h"
#include "flag_call.h"

struct OrbitEffect {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 radius;
  u8 unk34[0x1c];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[0x0e];
  u16 angle;
  u16 unk66;
  u8 unk68[4];
  void (*update)(struct OrbitEffect *effect);
};

extern u32 gFrameCount;
extern u32 gData_02003214[];

struct FieldActor *Actor_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(u32 angle);
void Actor_SetPartAttribute(struct OrbitEffect *object, s32 value);
struct OrbitEffect *Func_02002828(s32 type, s32 x, s32 y, s32 z);
void Func_02002820(struct OrbitEffect *object, const void *script);
void Func_02000148(struct FieldActor *self);

static inline struct OrbitEffect *CreateOrbitEffect(s32 type, s32 x, s32 y,
                                                    s32 z) {
  /* FAKEMATCH(wrapper): the effect created through an inline, which
   * evaluates the position before the type; as a direct call of
   * Func_02002828, 4 of 94 instructions differ in tla-en. */
  return Func_02002828(type, x, y, z);
}

/// Every third frame, or every frame once flag 0x201 is set, starts a particle
/// circling actor 23 at a random height above it.
void Func_020001a4(void) {
  struct FieldActor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Flag_IsSet(SPLIT_CONSTANT(0x201)) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_GetById(23);
  if (Flag_IsSet(SPLIT_CONSTANT(0x201)) != 0) {
    u32 random;

    random = Random16Far();
    effect = CreateOrbitEffect(0x2a0, center->x,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  } else {
    u32 random;

    random = Random16Far();
    effect = CreateOrbitEffect(0x2a0, center->x,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Func_02002820(effect, gData_02003214);
    Actor_SetPartAttribute(effect, 1);
    effect->motionFlags = 0;
    angle = Random16Far() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Func_02000148;
    effect->radius = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}
