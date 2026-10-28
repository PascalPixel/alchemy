#include "types.h"
#include "frame.h"
#include "actor.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  u8 unk08[16];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[8];
};

extern u32 gData_02003d50[];

s32 GameFlag_Test(s32 flag);
u32 Random16Far(void);
void Func_02003c4c(struct Actor *object, s32 value);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);

/// Every fourth frame, spawns effect 403 at the object with a random z offset
/// and z velocity.
void Func_02000d40(struct Actor *object) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct MagmaRockEffectConfig config;
    s32 dz;
    s32 velocityZ;
    s32 z;

    config.id = 403;
    config.kind = 2;
    config.data = gData_02003d50;
    if ((frame & 15) == 0 && !GameFlag_Test(608))
      Func_02003c4c(object, 136);
    dz = ((Random16Far() & 15) - 8) << 15;
    velocityZ = ((Random16Far() & 31) - 16) << 12;
    z = object->z;
    /* FAKEMATCH(barrier): the object's z loads between the velocity's shift
     * and its store, as native; as object->z + dz in the call, 2 of 55
     * instructions differ in tla-en. */
    asm("" : "+r"(z) : "r"(velocityZ));
    Func_0200015c(object->x, object->y, z + dz, 0, 0, velocityZ, 0x320001,
                  &config);
  }
}
