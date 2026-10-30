// Near miss, differ 2 of 94 (tla-en, 2026-10-30): as overlay 69e's
// Func_020001a4 (resource_69e_c_020001a4.c), the same function, the sprite's
// priority byte loads before the radius store, where the native loads it after
// the store and the zero's copy out of r8: -mthumb-slow-loads's
// read-modify-write exception gives the load no wait after the store (that
// draft's header has the sched2 reading and what was tried).
// History: adopted in 90246a275d and exact under the old options; with
// -mthumb-slow-loads (2026-10-26) it no longer compiles exactly, so it is back
// on its listing.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "flag_call.h"

extern u32 gFrameCount;
extern const u8 gData_020068b0[];

u32 Random16Far(void);
s32 Math_Sine(u32 angle);
struct Actor *Actor_GetById(s32 id);
struct Actor *Func_0200576c(s32 type, s32 x, s32 y, s32 z);
void Func_02005764(struct Actor *object, const void *script);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02000090(void);

static inline struct Actor *Actor_Create(s32 type, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the effect created through an inline, which
   * evaluates the position before the type; as a direct call of
   * Func_0200576c, 4 of 94 instructions differ in tla-en. */
  return Func_0200576c(type, x, y, z);
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
