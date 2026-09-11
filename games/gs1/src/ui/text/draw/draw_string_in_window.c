#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_string_in_window.h"

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0);

void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
{
    u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;
    x >>= 3;
    y >>= 3;
    Ui_SetMode(buffer, arg1, x, y);
    Ui_Do(buffer);
}
