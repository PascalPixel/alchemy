#include "far_runtime.h"
#include "shop.h"

s32 Func_08015080(s32, s32, s32, s32);

void Shop_DrawMessage(s32 window, s32 message) {
    if (window != 0) {
        Func_08015270(window);
        Func_08015080(message, window, 0, 0);
    }
}
