#include "types.h"

#define SceneActor_SetPairZeroAndValue Func_02002780

extern void Func_02005454(s32 a, s32 b, s32 c);
extern s32 Func_02005392(s32 a);

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c) {
    Func_02005454(a, b, 0);
    Func_02005392(c);
}
