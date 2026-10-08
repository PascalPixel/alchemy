#include "types.h"
#include "system.h"
extern u8 Data_03001e8c[];

struct Work;

s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
struct Work *UiText_OpenMessageWindow(s32 no, s32 x, s32 y, s32 flags);
s32 Object_GetScreenPositionFar(s32 object, s32 *out);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(struct Work *work, s32 release);
s32 UiWork_IsIdle(struct Work *work);

extern s32 gGameState[];

void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags)
{
    u8 *base = *(u8 **)((u32)&Data_03001e8c);
    s32 release;
    s32 pos[2] = {0, 0};
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    struct Work *work = NULL;
    s32 zero;

    y = x = 0;
    release = flags & 1;

    if (flags & 2)
    {
        *(u8 *)(base + RENDER_BUSY_OFS) = 1;
    }

    UiText_GetResourceDimensions(no, (s32)&x, (s32)&y, (s32)&width, (s32)&height);

    x = (30 - width) >> 1;
    y = (12 - height) >> 1;

    if (flags & 8)
    {
        y = y + 4;
    }
    else if (flags & 0x40)
    {
        y = y + 12;
    }
    else
    {
        s32 dy;

        Object_GetScreenPositionFar(gGameState[125], pos);
        dy = pos[1] >> 3;
        if (dy > 9)
        {
            y = dy - 5;
        }
        else
        {
            y = dy + 4;
        }
    }

    work = UiText_OpenMessageWindow(no, x, y, release);

    if (work != NULL)
    {
        while (UiWork_IsComplete() == 0)
        {
            WaitFrames(1);
        }

        if (flags & 0x20)
        {
            *(u8 *)(*(u8 **)((u32)&Data_03001e8c) + RENDER_MENU_BUSY_OFS) = 1;
        }

        if (!(flags & 4))
        {
            UiWork_Finalize(work, release);
            while (UiWork_IsIdle(work) == 0)
            {
                WaitFrames(1);
            }
        }
    }

    zero = 0;
    *(u8 *)(base + RENDER_BUSY_OFS) = zero;
    *(u16 *)(base + RENDER_RESULT_OFS) = zero;
    *(u16 *)(base + RENDER_RESULT_OFS + 2) = zero;
    WaitFrames(3);
}
