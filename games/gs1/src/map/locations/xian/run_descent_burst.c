#include "types.h"

extern u8 *Func_020051c6(s32 id);
extern void Func_02005118(s32 arg0);
extern void Func_02005370(s32 sound_id);
extern void Func_02000f4c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02000f62(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02000f86(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02000fa2(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);

void FieldScene_RunDescentBurst(void)
{
    u8 *record = Func_020051c6(19);
    u32 i = 0;
    s32 step = 8;
    s32 zero;
    do {
        Func_02005118(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    zero = 0;
    *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)zero;
    Func_02005370(227);
    Func_02000f4c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Func_02000f62(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Func_02000f86(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Func_02000fa2(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}
