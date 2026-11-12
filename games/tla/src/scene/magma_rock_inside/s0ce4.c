#include "local.h"
#include "types.h"
#include "actor.h"
#include "frame.h"
#include "field_effect.h"

void Func_02004e70(s32 a, s32 b);
void Func_02004e78(s32 owner, u16 value);
s32 Field_DispatchTypeHandler(s32 type);
s32 Func_02004e80(void);
s32 Func_02004e88(void);

void Func_02000ce4(s16 value) {
  struct MainRoutineWork *party;

  Func_02004e70(151, 1);
  party = &gMainRoutineWork;
  Func_02004e78(party->currentOwner, value);
  Field_DispatchTypeHandler(1);
  Func_02004e80();
  Func_02004e88();
}

extern const s32 gData_0200502c[];

void Func_02004f28(struct Actor *object, s32 sound);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

/// An object's update: every fourth frame it spawns an effect of type 403 with
/// its own script at a random point within 4 pixels of the object along z,
/// moving along z at a random speed; every sixteenth frame it also plays sound
/// 136 at the object.
void Func_02000d1c(struct Actor *object) {
  struct EffectOptions options;
  u32 frame = gFrameCount;
  s32 dz;
  s32 vz;
  s32 z;

  if ((frame & 3) != 0)
    return;
  options.type = 403;
  options.priority = 2;
  options.script = gData_0200502c;
  if ((frame & 15) == 0)
    Func_02004f28(object, 136);
  dz = Random16Far() & 15;
  vz = ((Random16Far() & 31) - 16) << 12;
  /* FAKEMATCH(temporary): z read into its own local; as object->z in the
   * call, 4 of 60 instructions differ in tla-en. */
  z = object->z;
  Func_0200015c(object->x, object->y, z + ((dz - 8) << 15), 0, 0, vz, 0x320001,
                &options);
}

void Func_02000da4(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetSpriteColor(object, 10);
  else
    Actor_SetSpriteColor(object, 0);
}

// The cell watch's table lookup over (id, cell, value) rows ended by 0xffff:
// for the row of the cell under the watch, sets the watch's object to the row's
// object and returns its value; otherwise returns the value of the last row
// passed (0 for none). The loop is spelled with gotos in the native's rotated
// layout, which rebuilds 0xffff on every pass, where loop.c would hoist it out
// of a C loop.

struct CellWatch {
  u8 unk00[4];
  s32 x;
  u8 unk08[4];
  s32 z;
  u8 unk10[4];
  struct Actor *object;
};

s32 Func_02000658(u32 layer, s32 x, s32 y);

s32 Func_02000dcc(u16 *table) {
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 cell = Func_02000658(0, watch->x >> 20, watch->z >> 20) >> 8;
  u32 id;

  goto test;
again:
  result = *table++;
test:
  if (*table != 0xffff) {
    id = *table++;
    if (cell != *table++)
      goto again;
    watch->object = Actor_Get(id);
    result = *table;
  }
  return result;
}

void Func_02000e14(void) { Event_ResolvePendingActions(0); }
