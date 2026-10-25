// Draft of overlay 6a5's Func_020044cc, the twin of overlay 698's
// Func_020007a4: its draft (resource_698_c_020007a4.c) with this overlay's
// symbols: differ 69 of 92. Left: what that draft's header records.
#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
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

void Func_020059f4(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_0200576c(s32 type, s32 x, s32 y, s32 z);
void Func_02005754(struct GustObject *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_0200430c(struct GustObject *object);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_020044cc(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_020059f4(222);
  position.x = source->x + Iwram_MulQ16(0x80000, Math_Cosine(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sine(source->angle));
  object = Func_0200576c(280, position.x, source->y, position.z);
  Func_02005754(object, 2);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_0200430c;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02004bd4(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}
