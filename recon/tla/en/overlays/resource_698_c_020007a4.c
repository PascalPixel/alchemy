// Near miss, differ 4 of 92 (tla-en, 2026-10-31): two rows sched2 orders by a
// cost the C cannot move. After the x store the native copies the position's
// address to r8 before reading the angle again; here the angle's ldrh, an
// extended load that -mthumb-slow-loads lets follow a store at once, is ready
// a cycle after the store and outranks the copy (priority 212 to 211). After
// the motion flags' store the native finishes it before copying the object to
// r1 for the priority flags; here their ldrb opens a read-modify-write, so it
// does not wait for the store either, and the copy outranks the store (95 to
// 94). Were each load to wait a cycle after its store, both rows would fall in
// the native's order.
// Overlay 698's Func_020007a4, in 15 overlays of the 698 family. Plays sound
// 222, creates object 280 eight pixels from the source along its angle and
// spawns an effect under it. Both products go through the call layer (Value2),
// which keeps IwramMulQ16's address in r6 and builds 0x80000 again for each,
// as the native does (directly, gcse keeps 0x80000 in r6: differ 69; through a
// local function pointer: 72). The angle is stored first of the four fields
// (after the three cleared ones: differ 10). Tried: permute 3 minutes and
// every one-line move (10, before the angle moved); a one-pass block or an
// empty asm after the x store or after the motion flags' store, alone or both
// (4 each: the address copy then precedes the x store, or the flags' address
// is finished before the zero is made); permute 8 minutes from differ 4
// (17,586 candidates, none better).
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

void Func_02001b44(s32 sound);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct GustObject *Func_02001a14(s32 type, s32 x, s32 y, s32 z);
void Func_020019fc(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_020005e4(struct GustObject *object);
void Func_02000eac(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_020007a4(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Func_02001b44(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sine(source->angle));
  object = Func_02001a14(280, position.x, source->y, position.z);
  Func_020019fc(object, 2);
  Actor_SetSingleChildFlags(object, 0);
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
