// Draft, differ 16 of 92: overlay 6ad's copy of resource_698_c_020007a4.c, with
// this overlay's symbols, its two Iwram_MulQ16 calls through a forwarding
// inline (direct, they share 0x80000 in r6 and the call address moves to r10:
// 69). Left, all instruction order: the native copies the position's address to
// r8 before reloading the angle after the x store, computes the priority flags'
// address after the motion flags store, loads the pooled zero for unk62 and
// unk63 before the angle and builds the options' address later; and its literal
// pool lands one instruction earlier. An asm holding position.x makes it much
// worse (90).
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

void Func_020056c0(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_02005498(s32 type, s32 x, s32 y, s32 z);
void Func_02005480(struct GustObject *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_02001894(struct GustObject *object);
void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h). */
  return Iwram_MulQ16(a, b);
}

struct GustObject *Func_02001a54(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_020056c0(222);
  position.x = source->x + MulQ16(0x80000, Math_Cosine(source->angle));
  position.z = source->z + MulQ16(0x80000, Math_Sine(source->angle));
  object = Func_02005498(280, position.x, source->y, position.z);
  Func_02005480(object, 2);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001894;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_020035b8(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}
