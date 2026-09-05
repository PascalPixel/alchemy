#include "types.h"

u8 *Func_02005004();
void Func_02004f56();
s32 Func_02004f8c();
s32 Func_02004f96();
void Func_020051cc();
void Func_02000dae();
void Func_02000dc6();
void Func_02000de4();

void FieldScene_RunForwardArcBurst(void)
{
    u8 *record = Func_02005004(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Func_02004f56((index - 4) * 2);
        *(s32 *)(record + 8) += Func_02004f8c(angle) * 6;
        *(s32 *)(record + 16) += Func_02004f96(angle) * 6;
    }

    *(s32 *)(record + 12) = 0x120000;
    *(s32 *)(record + 60) = 0x120000;

    Func_020051cc(227);

    Func_02000dae(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Func_02000dc6(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Func_02000de4(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}
