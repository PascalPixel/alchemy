#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0801a404(u8, s32, s32, s32, s32);
void *Func_08077080(s32);

/* 取得項目の+4値を先頭引数として転送する。 */
void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Func_0801a404(
        M2C_FIELD(Func_08077080(arg0), u8 *, 4),
        arg1,
        arg2,
        arg3,
        arg4);
}
