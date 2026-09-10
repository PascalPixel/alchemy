#include "types.h"

u8 *Func_08077008(s32);

s32 Func_080ac8fc(u16 *out, s32 id, s32 group)
{
    u8 *rec;
    s32 count;
    s32 g;
    s32 b;
    s32 mask;
    s32 v;

    rec = Func_08077008(id);
    count = 0;
    if (group == -1) {
        for (g = 0; g < 4; g++) {
            mask = *(s32 *)(rec + 0x108 + g * 4);
            for (b = 0; b < 20; b++) {
                if (mask & (1 << b)) {
                    v = (g << 5) | b | 0x8000 | (id << 8);
                } else if (*(s32 *)(rec + 248 + g * 4) & (1 << b)) {
                    v = (g << 5) | b | (id << 8);
                } else {
                    continue;
                }
                out[count++] = v;
            }
        }
    } else {
        mask = *(s32 *)(rec + 0x108 + group * 4);
        for (b = 0; b < 20; b++) {
            if (mask & (1 << b)) {
                v = (group << 5) | b | 0x8000;
            } else if (*(s32 *)(rec + 248 + group * 4) & (1 << b)) {
                v = (group << 5) | b;
            } else {
                continue;
            }
            out[count++] = v;
        }
    }
    return count;
}
