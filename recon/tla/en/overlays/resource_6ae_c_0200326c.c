// Draft, differ 2 of 92 under -mthumb-slow-loads: adopted in ee174ba9e6 and
// exact under the old options; with the new option it no longer compiles
// exactly, so it is back on its listing. Overlay 69e's Func_020001a4
// (resource_69e_c_020001a4.c) is the same function and differs the same way.
#include "types.h"
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
extern u32 gData_02006afc[];

u32 __umodsi3(u32 a, u32 b);
struct FieldActor *Object_GetById(s32 id);
u32 Random16Far(void);
s32 Math_Sine(u32 angle);
void Object_SetPartAttribute(struct OrbitEffect *object, s32 value);
struct OrbitEffect *Func_02005574(s32 type, s32 x, s32 y, s32 z);
void Func_0200556c(struct OrbitEffect *object, const void *script);
void Func_02003210(struct FieldActor *self);

static inline struct OrbitEffect *CreateOrbitEffect(s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarding through the inline evaluates the position before the
   * effect type; called directly, the type's second half comes before the z
   * load (differ 2). */
  return Func_02005574(0x2a0, x, y, z);
}

/// Every third frame, or every frame once flag 0x200 is set, starts a particle
/// circling actor 23 at a random height above it, as overlay 69e's
/// Func_020001a4 does under flag 0x201.
void Func_0200326c(void) {
  struct FieldActor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;
  /* FAKEMATCH(pin): the random value in r2, where the native copies the
   * call's result; as u32 random, 18 of 92 instructions differ in tla-en. */
  register u32 random asm("r2");

  if (Flag_IsSet(0x200) == 0 && __umodsi3(gFrameCount, 3) != 0)
    return;
  center = Object_GetById(23);
  if (Flag_IsSet(0x200) != 0) {
    random = Random16Far();
    effect = CreateOrbitEffect(
        center->x, (s32)((u32)(random << 8) >> 16 << 16) + center->y - 0x1c0000,
        center->z);
  } else {
    random = Random16Far();
    effect = CreateOrbitEffect(
        center->x, (s32)((u32)(random << 6) >> 16 << 16) + center->y - 0x1c0000,
        center->z);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Func_0200556c(effect, gData_02006afc);
    Object_SetPartAttribute(effect, 1);
    effect->motionFlags = 0;
    angle = Random16Far() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Func_02003210;
    effect->radius = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 2;
  }
}
