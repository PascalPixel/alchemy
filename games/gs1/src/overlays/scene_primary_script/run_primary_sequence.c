#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000e94

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001040();
void Func_02001058();
void Func_0200107a();
void Func_02001096();
void Func_020051f8();
s32 Func_020052a6();
void Func_02005462();

void FieldScene_RunPrimarySequence(void)
{
    s32 rec;
    u32 i;
    s32 step;

    rec = Func_020052a6(19);
    i = 0;
    step = 8;
    do {
        Func_020051f8(step);
        i = i + 1;
        *(s32 *)(rec + 16) += 0x10000;
        *(s32 *)(rec + 64) = -0x80000000;
        step = step - 2;
    } while (i <= 3);

    {
        s32 target = *(s32 *)(rec + 80);
        s32 shown = 0;

        /* The cleared halfword is a shared display cell: the reference
         * reloads the lift word below instead of reusing the value the
         * loop left in a register. */
        *(volatile u16 *)(target + 30) = shown;
    }
    *(volatile s32 *)(rec + 16) += 0x180000;
    *(s32 *)(rec + 64) = -0x80000000;
    Func_02005462(227);

    Func_02001040(*(s32 *)(rec + 8), *(s32 *)(rec + 12),
                  *(s32 *)(rec + 16) + 0xc0000, -0xcccc,
                  0, 0x3333, 0, 0);
    Func_02001058(*(s32 *)(rec + 8), *(s32 *)(rec + 12),
                  *(s32 *)(rec + 16) + 0xc0000, 0xcccc,
                  0, 0x3333, 0, 0);
    Func_0200107a(*(s32 *)(rec + 8) - 0x60000, *(s32 *)(rec + 12),
                  *(s32 *)(rec + 16) - 0x80000, 0x3333,
                  0, 0x10000, 0, 0);
    Func_02001096(*(s32 *)(rec + 8) + 0x60000, *(s32 *)(rec + 12),
                  *(s32 *)(rec + 16) - 0x80000, 0x3333,
                  0, 0x10000, 0, 0);
}
