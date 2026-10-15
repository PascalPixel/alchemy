/* Draft of overlay 698's Func_0200087c (0x0200887c), the twin of ☀️'s
 * SceneEffect_SpawnRandomEveryEightFramesB and of overlay 64e's
 * Func_020005a8: differ 4 of 72 (two reordered rows). After the first random
 * call the native copies the object to r2 and loads its x between the two
 * halves of the multiply by 9; here both halves come first (sched2 gives
 * lsls 116 and the copy 115). The x load split off behind a one-pass block or
 * a barrier differs by 12; four minutes of alchemy permute found nothing.
 * Copied in fourteen more overlays of the 698 family (copy group
 * 698:Func_0200087c). */
#include "types.h"

s32 Random16Far(void);

extern u32 gFrameCount;

struct SceneEffectSource {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct SceneEffectParams {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

void Func_02000eac(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

s32 Func_0200087c(struct SceneEffectSource *object)
{
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
