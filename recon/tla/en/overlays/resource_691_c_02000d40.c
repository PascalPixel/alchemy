// Near miss, differ 2 of 55: a frame-timed spawn of effect 403 at the object,
// as ☀️'s SceneEffect_SpawnPeriodicEffect. The native loads the object's z
// before it stores the z velocity to the stack; here the store goes first (in
// sched2 the store has priority 5, the load 4). Measured: alchemy permute for
// 3 minutes (2), a "+r" asm on the velocity (2), the z load and the velocity
// before an empty volatile asm (4: the offset's subs and lsls must then come
// ahead of it too), a one-pass block around them (17).
#include "types.h"
#include "frame.h"
#include "object_runtime.h"

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
void Func_02003c4c(struct ObjectRuntime *object, s32 value);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);

void Func_02000d40(struct ObjectRuntime *object) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct MagmaRockEffectConfig config;
    s32 dz;
    s32 velocityZ;

    config.id = 403;
    config.kind = 2;
    config.data = gData_02003d50;
    if ((frame & 15) == 0 && !GameFlag_Test(608))
      Func_02003c4c(object, 136);
    dz = ((Random16Far() & 15) - 8) << 15;
    velocityZ = ((Random16Far() & 31) - 16) << 12;
    Func_0200015c(object->x, object->y, object->z + dz, 0, 0, velocityZ,
                  0x320001, &config);
  }
}
