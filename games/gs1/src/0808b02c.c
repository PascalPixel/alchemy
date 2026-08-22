#include "types.h"

extern u8 *Data_03001ebc;

extern s32 Func_0808ae74();

s32 Func_0808b02c(u32 index) {
    u8 *table = Data_03001ebc + 0x1a0;

    /* 第2引数は呼出元のr1を引き継ぐ特殊な呼出規約。 */
    return Func_0808ae74(table[index]);
}
