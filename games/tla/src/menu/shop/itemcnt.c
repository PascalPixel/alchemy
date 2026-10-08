#include "types.h"

extern u8 MsgNoItems[];
void RenderOutput_PrepareForRedrawFar(void *window);
void Func_0810bf98(void *window, s32 x, s32 y, s32 item);
s32 Inventory_CountFar(s32 item);
void UiText_DrawResourceFar(s32 message, void *window, s32 x, s32 y);

void Shop_DrawItemCount(void *window, s32 item)
{
    if (window != NULL) {
        RenderOutput_PrepareForRedrawFar(window);
        Func_0810bf98(window, 8, 0, item);
        if (Inventory_CountFar(item) == 0)
#if defined(TLA_EDITION_IT)
            UiText_DrawResourceFar((s32)MsgNoItems, window, 20, 12);
#else
            UiText_DrawResourceFar((s32)MsgNoItems, window, 8, 20);
#endif
    }
}
