#include "types.h"

extern s32 Func_02001638(const u8 *, const u8 *);
extern s32 Func_08000100(s32, s32);
extern void Func_08009080(u8 *, s32);

s32 Func_02001674(u8 *self, const u8 *other, s32 distance_limit, s32 force)
{
    if (Func_02001638(self + 8, other + 8) < distance_limit || force != 0) {
        u32 angle = (u16)Func_08000100(*(const s32 *)(other + 16) - *(s32 *)(self + 16),
                                       *(const s32 *)(other + 8) - *(s32 *)(self + 8));
        u32 facing = *(u16 *)(self + 6) & 0xf000;

        if ((angle & 0xf000) == facing ||
            ((angle + 0x1000) & 0xf000) == facing ||
            ((angle + 0xfb90f002u) & 0xf000) == facing || force != 0) {
            self[91] = 1;
            Func_08009080(self, 1);
            return 1;
        }
    }

    self[91] = 0;
    Func_08009080(self, 2);
    return 0;
}
