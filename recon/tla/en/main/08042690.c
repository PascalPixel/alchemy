#include "TYPES.H"
extern u8 Data_03001e90[];
extern u8 Data_03001e8c[];

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct UiWindowBounds *window;
    s8 *busy;

    window = Runtime_AllocateBlock(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)((u32)&Data_03001e8c) + RENDER_MENU_BUSY_OFS);
    zero = 0;
    *busy = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->height, 6);
    UiWindow_DrawPartyStatusContents(flags);
    *busy = zero;
}
