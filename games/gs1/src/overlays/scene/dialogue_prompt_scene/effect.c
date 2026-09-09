#include "types.h"

#define SceneEffect_ApplyThreeValuesAndFinish Func_0200463c
#define SceneEffect_ApplyPairWithValue141 Func_02004658
void Func_02009418(s32, s32, s32);
void Func_0200942e(s32);
void Func_0200938c(s32);
void Func_02009538(s32, s32);
void Func_02009548(s32, s32);
void Func_02009564(void);
void Func_02009542(s32);
void Func_020092e8(s32);

void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
    Func_02009418(a, b, c);
    Func_0200942e(a);
    Func_0200938c(c);
}

void SceneEffect_ApplyPairWithValue141(s32 a, s32 b)
{
    Func_02009538(141, 1);
    Func_02009548(a, b);
    Func_02009564();
    Func_02009542(1);
    Func_020092e8(1);
}
