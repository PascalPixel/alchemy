#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"

extern u8 Data_03001e8c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, s32 mode, u32 flags);
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);
struct Work;
s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Object_GetScreenPositionFar(s32 object, s32 *out);
s32 UiWork_IsComplete(void);
s32 UiWork_IsIdle(void *work);
struct RenderInput *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed);

void UiWork_ProcessRenderChannels(void);
void UiWork_ProcessDirectWork(void);
s32 UiWork_AnimateSpriteSlots(void);

void UiWork_SetBusyFlags(s32 flags)
{
    void *work;

    work = gWindowWork[0];
    if (work != NULL) {
        if (flags & 1) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 1) = 1;
        }
        if (2 & flags) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 2) = 1;
        }
    }
}

/* Build a message's render entries, fit a window for them on screen and
   queue them there. Bits 16-27 of packed select the first entry; its low
   bits choose the window's frame and blending. Returns the window, or
   NULL when the message is empty or no window is free. */
struct RenderInput *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed)
{
    s32 entry;
    u32 height;
    u32 width;
    u16 bounds[4];
    s32 flags;
    u16 *out;
    struct UiWindow *window;
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)gWindowWork[0];
    work->result[0] = (packed << 4) >> 20;
    /* FAKEMATCH: retain the null window as the scroll and layout zero. */
    window = NULL;
    work->result[1] = (u32)window;
    packed &= 0xffff;
    flags = 0;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] == 0)
        return NULL;
    UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (s32)(out = bounds), (u32)window);
    if (width == 0 && height == 0)
        return NULL;
    if (!(packed & 1))
        flags |= 2;
    if (packed & 8)
        flags |= 8;
    if (packed & 16)
        flags |= 128;
    if (packed & 32)
        flags |= 256;
    window = UiWindow_Create(x, y, width, height, flags);
    if (window == NULL)
        return NULL;
    if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
        UiWork_Finalize(window, 1);
        return NULL;
    }
    *((u8 *)work + RENDER_BUSY_OFS + 1) = 0;
    *((u8 *)work + RENDER_BUSY_OFS + 2) = 0;
    return (struct RenderInput *)window;
}

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
    u8 *base = gWindowWork[0];
    s32 release;
    s32 pos[2] = {0, 0};
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    struct UiWindow *work = NULL;
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

        Object_GetScreenPositionFar(gGameState.selected_actor, pos);
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

    work = (struct UiWindow *)UiText_OpenMessageWindow(no, x, y, release);

    if (work != NULL)
    {
        while (UiWork_IsComplete() == 0)
        {
            WaitFrames(1);
        }

        if (flags & 0x20)
        {
            ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
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

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}
