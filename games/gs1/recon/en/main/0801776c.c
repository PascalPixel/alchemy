#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

struct Work;

s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
struct Work *Func_08017658(s32 no, s32 x, s32 y, s32 flags);
s32 Func_0808a278(s32 object, s32 *out);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(struct Work *work, s32 release);
s32 UiWork_IsIdle(struct Work *work);
void WaitFrames(s32 frames);

extern u8 Data_02000240[];

void Func_0801776c(s32 no, s32 flags)
{
    u8 *base;
    s32 release;
    s32 pos[2] = {0, 0};
    s32 height;
    s32 width;
    s32 y = 0;
    s32 x = 0;
    struct Work *work;

    base = *(u8 **)ADDR_03001E8C;
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

        Func_0808a278(*(s32 *)(Data_02000240 + 500), pos);
        dy = pos[1] >> 3;
        if (dy <= 9)
        {
            y = dy + 4;
        }
        else
        {
            y = dy - 5;
        }
    }

    work = Func_08017658(no, x, y, release);

    if (work != NULL)
    {
        while (UiWork_IsComplete() == 0)
        {
            WaitFrames(1);
        }

        if (flags & 0x20)
        {
            *(u8 *)(*(u8 **)ADDR_03001E8C + RENDER_MENU_BUSY_OFS) = 1;
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

    *(u8 *)(base + RENDER_BUSY_OFS) = 0;
    *(u16 *)(base + RENDER_RESULT_OFS) = 0;
    *(u16 *)(base + RENDER_RESULT_OFS + 2) = 0;
    WaitFrames(3);
}
