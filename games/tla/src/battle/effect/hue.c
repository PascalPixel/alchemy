#include "types.h"
#include "ram_buffer.h"

s32 Func_080dce20(s16 hue)
{
    s32 level = 0;
    s16 angle = hue;

    angle = (angle + 360) % 360;
    if (angle < 60) {
        level = angle * 31 / 60;
    } else if (angle < 180) {
        level = 31;
    } else if (angle < 240) {
        level = (240 - angle) * 31 / 60;
    }
    return level;
}

struct WaveFxWork {
    u8 unknown_000[0x28b];
    u8 red;
    u8 green;
    u8 blue;
    u16 hue; // 度
};

// 色相を 4 度すすめ、その色を求める
void Func_080dce60(void)
{
    struct WaveFxWork *work = Ram_HeapSlots->unknown_4c[3];
    s16 r;
    s16 g;
    s16 b;
    u32 limit;

    asm volatile(""); /* FAKEMATCH: ⚓️ loads the work block before it builds the hue's offset; without the barrier the offset's first half is scheduled ahead of the load (differ 2) */
    r = Func_080dce20(work->hue);
    g = Func_080dce20(work->hue + 120);
    b = Func_080dce20(work->hue + 240);

    // 0〜31 を 4〜11 に縮める
    work->red = (r / 4 + 4) & 31;
    work->green = (g / 4 + 4) & 31;
    work->blue = (b / 4 + 4) & 31;
    limit = 359 << 16;
    /* FAKEMATCH: ⚓️ loads the wrap limit from the pool before it reads the hue; written as work->hue += 4; if (work->hue >= 360) the same instructions come with the hue read first (differ 2). The temporary holds the limit as the compare uses it, 359 degrees in the high half. */
    asm volatile("" : "+l"(limit));
    if ((u32)((work->hue += 4) << 16) > limit)
        work->hue = 0;
}
