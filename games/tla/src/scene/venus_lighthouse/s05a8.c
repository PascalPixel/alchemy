#include "types.h"
#include "event_inline.h"

s32 Random16Far(void);

extern u32 gFrameCount;

struct SceneEffectSource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct SceneEffectParams {
  s32 priority;    ///< the sprite's priority
  s32 palette;     ///< the sprite's palette
  s32 startScaleX; ///< 16.16 fixed point, 0x10000 full size
  s32 startScaleY; ///< 16.16 fixed point, 0x10000 full size
  u8 unk10[0x18];
};

void Func_02000150(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

/// Emits a rising particle near the actor every eight frames.
s32 VenusLighthouse_EmitRisingParticles(struct SceneEffectSource *actor) {
  struct SceneEffectParams options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    goto done;
  options.priority = 3 - (s32)((u32)(Random16Far() * 2) >> 16);
  options.startScaleX = 0x6666;
  options.startScaleY = 0x6666;
  options.palette = 14;
  particleX = actor->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
  particleY = actor->y + ((32 - (s32)((u32)(Random16Far() * 32) >> 16)) << 16);
  riseSpeed =
      ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x00050000) / 10;
  Func_02000150(particleX, particleY, actor->z, 0, riseSpeed, framePhase,
                0x000b0000, &options);
done:
  return 0;
}
