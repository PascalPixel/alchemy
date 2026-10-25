#include "types.h"

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

void Func_02000eac(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

s32 Func_0200087c(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.unk00 = 3;
    params.color1 = 0xb333;
    params.color2 = 0xb333;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
    speed = ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x50000) / 10;
    Func_02000eac(x, y, object->z, 0, speed, phase, 0xb0000, &params);
  }
  return 0;
}
