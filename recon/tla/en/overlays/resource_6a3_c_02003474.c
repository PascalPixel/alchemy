// Draft, differ 10 of 92 (was 69): overlay 6a3's copy of overlay 698's
// Func_020007a4. The two polar offsets go through inline helpers, as the
// native rebuilds the 0x80000 radius for each call. Left: scheduling; the
// native loads the pool zero for unk62 and unk63 before the angle and
// builds the options address between the angle's load and store. Tried:
// every order of the five stores (10 is best), a chained zero, one-pass
// blocks around them (16 to 18), permute 10 minutes.
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

void Func_020049c4(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_0200470c(s32 type, s32 x, s32 y, s32 z);
void Func_020046f4(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_020032b4(struct GustObject *object);
void Func_02003b7c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ s32 Gust_PolarX(u16 angle, s32 radius) {
  return Iwram_MulQ16(radius, Math_Cosine(angle));
}

static __inline__ s32 Gust_PolarZ(u16 angle, s32 radius) {
  return Iwram_MulQ16(radius, Math_Sine(angle));
}

struct GustObject *Func_02003474(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_020049c4(222);
  position.x = source->x + Gust_PolarX(source->angle, 0x80000);
  position.z = source->z + Gust_PolarZ(source->angle, 0x80000);
  object = Func_0200470c(280, position.x, source->y, position.z);
  Func_020046f4(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->angle = source->angle;
  object->update = Func_020032b4;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02003b7c(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}
