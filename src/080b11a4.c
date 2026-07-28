#include "far_runtime.h"
#include "types.h"

s32 Func_08015080(s32, s32, s32, s32);

/* 対象が非0のとき同じarg0を2つの処理へ渡す。 */
void Func_080b11a4(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        Func_08015270(arg0);
        Func_08015080(arg1, arg0, 0, 0);
    }
}
