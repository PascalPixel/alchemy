#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_string.h"

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0);

void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
    s16 *buffer;
    s16 *output;
    u8 *input;

    input = text;
    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    output = buffer;
    if (*input != 0) {
        do {
            *output = (s16)*input;
            input++;
            output++;
        } while (*input != 0);
    }
    *output = 0;
    Ui_SetMode(buffer, arg1, arg2, arg3);
    Ui_Check(buffer);
}
