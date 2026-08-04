#include "types.h"

extern s32 *Func_02005510();
extern void Func_020054cc();

void Func_020026b8(void) {
    s32 *p = Func_02005510(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Func_020054cc(38, 12, 1, 1, k5, k6);
            }
        }
    }
}
