#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);
s32 UiWork_IsComplete(void);
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed);

void UiWork_SetBusyFlags(s32 flags)
{
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)gWindowWork[0];
    if (work != NULL) {
        if (flags & 1) {
            work->spacing_before = 1;
        }
        if (2 & flags) {
            work->spacing_after = 1;
        }
    }
}

struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed)
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
    /* FAKEMATCH: needed to match: retain the null window as the scroll and layout zero. */
    window = NULL;
    work->result[1] = (u32)window;
    packed &= 0xffff;
    flags = 0;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] == 0)
        return NULL;
    UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), (u32)window);
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
    work->spacing_before = 0;
    work->spacing_after = 0;
    return window;
}

void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
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
        state->busy = 1;
    }

    UiText_GetResourceDimensions(no, &x, &y, (u32 *)&width, (u32 *)&height);

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

    work = UiText_OpenMessageWindow(no, x, y, release);

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
    state->busy = zero;
    state->result[0] = zero;
    state->result[1] = zero;
    WaitFrames(3);
}

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}
