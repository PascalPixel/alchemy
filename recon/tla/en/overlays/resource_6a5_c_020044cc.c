// Draft, differ 4 of 92, skeleton gap 2 (tla-en, 2026-11-01)
// Overlay 6a5's copy of overlay 698's Func_020007a4, in the same form. Plays
// sound 222, creates object 280 eight pixels from the source along its angle
// and spawns an effect under it. Both products go through the call layer
// (Value2), which keeps IwramMulQ16's address in r6 and builds 0x80000 again
// for each; the angle is stored first of the four fields.
// Left: two rows sched2 orders, both a load following a store. The native
// copies the position's address to r8 before the ldrh of the angle (here 212
// to 211 for the ldrh) and finishes the motion flags' strb before copying the
// object to r1 (here 95 to 94 for the copy). -mthumb-slow-loads delays a load
// only when it reads the address the store wrote (arm_adjust_cost); a cycle's
// wait after these stores to other addresses would give the native's order,
// so this is the compiler's approximation (K2), not something in the C.
// Tried, no gain: u16 parameters on Math_Sine and Math_Cosine, a union or
// array for the position, a pointer to it, a local for the product, a pointer
// to either flags byte, a bit-field for the priority flag, a signed motion
// flags byte, a swapped sum, storing position.y.
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

void Func_020059f4(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_0200576c(s32 type, s32 x, s32 y, s32 z);
void Func_02005754(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_0200430c(struct GustObject *object);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_020044cc(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_020059f4(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sine(source->angle));
  object = Func_0200576c(280, position.x, source->y, position.z);
  Func_02005754(object, 2);
  Actor_SetSingleChildFlags(object, 0);
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
