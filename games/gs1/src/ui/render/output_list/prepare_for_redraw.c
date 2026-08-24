#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void RenderOutput_RedrawSavedRect(void *);
s32 RenderOutput_ClearList(void *);

void RenderOutput_PrepareForRedraw(void *arg0) {
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & M2C_FIELD(arg0, u16 *, 0x16))) {
        RenderOutput_RedrawSavedRect(arg0);
        RenderOutput_ClearList(arg0);
    }
}
