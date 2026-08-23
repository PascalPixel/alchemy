#include "far_runtime.h"
#include "shop.h"

s32 UiText_DrawAt(s32, s32, s32, s32);

void Shop_DrawMessage(s32 window, s32 message) {
    if (window != 0) {
        UiWindow_Commit(window);
        UiText_DrawAt(message, window, 0, 0);
    }
}
