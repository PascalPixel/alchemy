#include "PROTO.H"
#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "TEXT_RENDER_RUNTIME.H"


void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
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
    x >>= 3;
    *output = 0;
    y >>= 3;
    UiText_RenderWideStringInWindow(buffer, arg1, x, y);
    Sys_Free(buffer);
}
