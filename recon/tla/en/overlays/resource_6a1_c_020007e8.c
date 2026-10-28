// Draft, differ 10 of 92: overlay 6a1's copy of the function drafted in
// resource_698_c_020007a4.c (overlay 698's Func_020007a4, differ 10 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "call.h"
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

void Func_020022c8(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_02002178(s32 type, s32 x, s32 y, s32 z);
void Func_02002160(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_02000628(struct GustObject *object);
void Func_02001600(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_020007e8(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_020022c8(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sine(source->angle));
  object = Func_02002178(280, position.x, source->y, position.z);
  Func_02002160(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->angle = source->angle;
  object->update = Func_02000628;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02001600(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}
