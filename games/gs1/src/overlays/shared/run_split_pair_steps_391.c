#include "types.h"

#define FieldScene_RunSplitPairSteps Func_02002768

extern void Func_02005434(s32 arg0, s32 arg1);
extern s32 Func_0200537a(s32 arg0);

void FieldScene_RunSplitPairSteps(s32 a, s32 b) {
    Func_02005434(a, 0);
    Func_0200537a(b);
}
