#include "types.h"
#include "scene.h"


void RenderOutput_RedrawSavedRect(void *);
s32 RenderOutput_ClearList(void *);

void RenderOutput_PrepareForRedraw(void *arg0)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & FIELD_AT_OFFSET(arg0, u16 *, 0x16))) {
        RenderOutput_RedrawSavedRect(arg0);
        RenderOutput_ClearList(arg0);
    }
}
