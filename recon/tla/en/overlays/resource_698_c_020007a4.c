// Draft of overlay 698's Func_020007a4, in 15 overlays of the 698 family:
// differ 69 of 92. Plays sound 222, creates object 280 eight pixels from the
// source along its angle and spawns an effect under it. Left: the native keeps
// IwramMulQ16's address in r6 and builds 0x80000 again for the second product,
// and the new object takes r7; here gcse keeps 0x80000 in r6 and the object
// takes r10. Tried: the object pinned to r7 (40), the position's pointer
// pinned as well (46), a function pointer for the product (72), a one-pass
// block or barrier between the products (69).
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

void Func_02001b44(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_02001a14(s32 type, s32 x, s32 y, s32 z);
void Func_020019fc(struct GustObject *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_020005e4(struct GustObject *object);
void Func_02000eac(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_020007a4(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_02001b44(222);
  position.x = source->x + Iwram_MulQ16(0x80000, Math_Cosine(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sine(source->angle));
  object = Func_02001a14(280, position.x, source->y, position.z);
  Func_020019fc(object, 2);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_020005e4;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02000eac(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}
