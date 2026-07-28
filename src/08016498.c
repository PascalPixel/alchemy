#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080170f8(u16, u16, u16, u16);

void Func_08016498(void *arg0) {
    /* 保存済みの矩形を再描画する。 */
    Func_080170f8(M2C_FIELD(arg0, u16 *, 0xC), M2C_FIELD(arg0, u16 *, 0xE), M2C_FIELD(arg0, u16 *, 8), M2C_FIELD(arg0, u16 *, 0xA));
}
