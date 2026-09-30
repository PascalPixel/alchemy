#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "SCENE.H"
#include "RUNTIME_INTERFACES.H"
void Runtime_BumpFree(void *buffer);
extern u8 Data_03001e8c[];
u32 Resource_DecodeType01(const void *source, void *destination);
s32 UiText_MeasureEntryDimensions(s32 start, s32 *width, s32 *count, s32 mode);

/* ui/window/window_copy_tilemap_region.c */
/* ui/window/copy_tilemap_region.c */
extern u8 *gWindowWork;

s16 *Runtime_BumpAllocateAlternatePool(s32 size);

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
    u8 text[20];
    s32 length = 0;

    if (*input != 0) {
        do {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6) {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    UiText_DrawString(text, output, 0, -2);
}
