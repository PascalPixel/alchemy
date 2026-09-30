/*
 * Draft: UiText_DrawString does not yet match; 4 halfwords differ from ☀️'s C, first at +0x24 (adds r5, #1).
 * Links as recon/tla/raw/080420a8.s.
 */
#include "TEXT_RENDER_RUNTIME.H"

s16 *Runtime_BumpAllocateAlternatePool(s32 size);
void UiText_RenderWideStringAtOffset(void *text, s32 work, s32 x, s32 y);

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
    UiText_RenderWideStringAtOffset(buffer, arg1, arg2, arg3);
    Runtime_BumpFree(buffer);
}
