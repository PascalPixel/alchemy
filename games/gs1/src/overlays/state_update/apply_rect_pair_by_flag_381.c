#include "types.h"

extern void Func_02005d68(s32, s32, s32, s32, s32, s32);
extern void Func_02005d7a(s32, s32, s32, s32, s32, s32);
extern void Func_02005d8e(s32, s32, s32, s32, s32, s32);
extern void Func_02005da0(s32, s32, s32, s32, s32, s32);
extern void Func_02005d94(void);

void SceneState_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Func_02005d68(9, 45, 65, 5, n, n);
        Func_02005d7a(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Func_02005d8e(89, 2, 65, 5, n, n);
        Func_02005da0(102, 32, 67, 6, 1, n);
    }
    Func_02005d94();
}
