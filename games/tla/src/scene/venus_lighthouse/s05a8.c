#include "types.h"

void Actor_SetPartAttribute(void *object, s32 value);
void Import_SoundPlay(s32 cue);
s32 Random16Far(void);

extern u32 gFrameCount;

struct SceneEffectSource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct SceneEffectParams {
  s32 unk00;
  s32 mode;
  s32 color1;
  s32 color2;
  u8 unk10[0x18];
};

void Func_02000150(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

s32 Func_020005a8(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;
  phase = gFrameCount & 7;
  if (phase != 0)
    goto done;
  params.unk00 = 3 - (s32)((u32)(Random16Far() * 2) >> 16);
  params.color1 = 0x6666;
  params.color2 = 0x6666;
  params.mode = 14;
  x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
  y = object->y + ((32 - (s32)((u32)(Random16Far() * 32) >> 16)) << 16);
  speed = ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x00050000) / 10;
  Func_02000150(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
  return 0;
}
