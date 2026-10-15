#include "mem.h"
#include "text_render_runtime.h"

void UiText_RenderWideStringAtOffset(void *text, s32 work, s32 x, s32 y);

void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
    u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
        /* FAKEMATCH: ⚓️ steps the output before it reads the next byte. */
        __asm__("");
    }
    *output = 0;
    UiText_RenderWideStringAtOffset(buffer, arg1, arg2, arg3);
    MemFree(buffer);
}
