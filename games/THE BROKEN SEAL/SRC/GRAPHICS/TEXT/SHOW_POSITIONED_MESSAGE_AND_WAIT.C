#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

struct Work;

s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
struct Work *Func_08017658(s32 no, s32 x, s32 y, s32 flags);
s32 Func_0808a278(s32 object, s32 *out);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(struct Work *work, s32 release);
s32 UiWork_IsIdle(struct Work *work);
void WaitFrames(s32 frames);

extern s32 Data_02000240[];
#define PARTY_STATE Data_02000240

#define UiText_ShowPositionedMessageAndWait Func_0801776c

/* Shows message no in a window centred across the screen and waits until it
 * has printed. Flag 8 places the window high and flag 0x40 lower; otherwise it
 * goes above or below the screen row of the object in game-state word 125.
 * Flag 1 is passed on to the window as its release flag, flag 2 marks text
 * rendering busy while the message shows, flag 0x20 sets the menu busy byte
 * once it has printed, and flag 4 leaves the window open instead of closing
 * it and waiting for it to finish. Ends by clearing the busy byte and the
 * result words and waiting three frames. */
void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
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

        Func_0808a278(PARTY_STATE[125], pos);
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

    zero = 0;
    *(u8 *)(base + RENDER_BUSY_OFS) = zero;
    *(u16 *)(base + RENDER_RESULT_OFS) = zero;
    *(u16 *)(base + RENDER_RESULT_OFS + 2) = zero;
    WaitFrames(3);
}
