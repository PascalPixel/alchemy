#include "TYPES.H"
#include "TBS_EDITION.H"
#include "SCENE.H"

extern u8 *volatile gWindowWork;

s32 Audio_PlayCue();

void UiWork_SetMenuBusy(void)
{
    u8 *base = gWindowWork;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void UiWork_ClearMenuBusy(void)
{
    u8 *base = gWindowWork;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}

s32 Audio_PlayCueReturnOne(void)
{
    Audio_PlayCue();
    return 1;
}
