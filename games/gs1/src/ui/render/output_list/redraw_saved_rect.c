#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080170f8(u16, u16, u16, u16);

void RenderOutput_RedrawSavedRect(void *arg0) {
    /* 保存済みの矩形を再描画する。 */
    Func_080170f8(FIELD_AT_OFFSET(arg0, u16 *, 0xC), FIELD_AT_OFFSET(arg0, u16 *, 0xE), FIELD_AT_OFFSET(arg0, u16 *, 8), FIELD_AT_OFFSET(arg0, u16 *, 0xA));
}
