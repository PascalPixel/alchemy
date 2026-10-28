#include "types.h"
#include "actor.h"
#include "frame.h"
#include "field_effect.h"

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
  /* FAKEMATCH(one-pass): the speed in a once-only block, which lets the
   * object's z load ahead of the flags; as a plain statement, 2 of 60
   * instructions differ in tla-en. */
  do {
    vz = ((Random16Far() & 31) - 16) << 12;
  } while (0);
  /* FAKEMATCH(temporary): z read into its own local; as object->z in the
   * call, 4 of 60 instructions differ in tla-en. */
  z = object->z;
  Func_0200015c(object->x, object->y, z + ((dz - 8) << 15), 0, 0, vz, 0x320001,
                &options);
}
