#include "types.h"

#define SceneState_ApplyRectsByFlags2384And962 Func_02001c6c
/* resource_3b9 owner at 0x02001c6c, 100 bytes. Two alternative layouts. */
s32 Func_02004832();
void Func_02004820();
void Func_0200483c();
void Func_02004936();
s32 Func_0200486c();
void Func_02004864();
void SceneState_ApplyRectsByFlags2384And962(void)
{
    s32 pair;
    s32 a, b;

    if (Func_02004832(2384) != 0) {
        pair = 2;
        Func_02004820(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Func_0200483c(14, 8, 2, 1, a, b);
    } else {
        Func_02004936(16, 2);
        if (Func_0200486c(0x962) != 0) {
            a = 14;
            b = 11;
            Func_02004864(30, 22, 1, 2, a, b);
        }
    }
}
