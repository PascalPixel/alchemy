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

s32 UiText_SetRenderString(const u8 *str)
{
    u8 *base;
    u16 *dst;
    s32 count;
    s32 offset;
    s32 count_out;
    s32 width_out;

    base = *(u8 **)((u32)&Data_03001e8c);
    count = 0;
    if (*str != 0) {
        dst = (u16 *)(base + RENDER_ENTRY_TBL_OFS);
        do {
            *dst = *str;
            str++;
            dst++;
            count++;
        } while (*str != 0);
    }
    offset = RENDER_ENTRY_TBL_OFS + count * 2;
    *(u16 *)(base + offset) = 0;
    UiText_MeasureEntryDimensions(0, &count_out, &width_out, 0);
    return count_out;
}
