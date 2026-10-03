#include "RUNTIME_MEM.H"
/*
 * Draft: UiText_DrawStringInWindow does not yet match; 4 halfwords differ from ☀️'s C, first at +0x24 (adds r5, #1).
 * Links as recon/tla/raw/08042188.s.
 */
#include "TEXT_RENDER_RUNTIME.H"

void UiText_RenderWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);

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
    UiText_RenderWideStringInWindow(buffer, arg1, x, y);
    Runtime_BumpFree(buffer);
}
