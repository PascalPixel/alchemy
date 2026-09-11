#include "types.h"
#include "scene.h"
#include "abi/ui/render/output_list/redraw_saved_rect.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void RenderOutput_RedrawSavedRect(void *arg0)
{
    /* 保存済みの矩形を再描画する。 */
    Ui_SetMode(FIELD_AT_OFFSET(arg0, u16 *, 0xC), FIELD_AT_OFFSET(arg0, u16 *, 0xE), FIELD_AT_OFFSET(arg0, u16 *, 8), FIELD_AT_OFFSET(arg0, u16 *, 0xA));
}
