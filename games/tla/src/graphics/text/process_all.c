#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"


// テキストワークの末尾: 日本語版は表が 0x200 バイト短い
#if defined(TLA_EDITION_JA)
#define TEXT_WORK_TAIL 0x1184
#else
#define TEXT_WORK_TAIL 0x1384
#endif

struct Work;

s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
struct Work *UiText_OpenMessageWindow(s32 no, s32 x, s32 y, s32 flags);
s32 EventRuntime_GetControlledOwnerFar(void);
s32 Func_080c82a8(s32 object, s32 *out);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(struct Work *work, s32 release);
s32 UiWork_IsIdle(struct Work *work);

#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT) /* Spanish and Italian reach the owner and its screen position through far-call slots two entries earlier, unnamed in their scaffolds: their copies stay on the scaffold (S4) */
void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags)
{
    u8 *base = Ram_HeapSlots->windowTiles;
    s32 release;
    s32 pos[2] = {0, 0};
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    struct Work *work = NULL;
    s32 zero;
    s32 margin;
    s32 rows;

    y = x = 0;
    release = flags & 1;

    if (flags & 2)
    {
        *(u8 *)(base + TEXT_WORK_TAIL + 5) = 1;
    }

    UiText_GetResourceDimensions(no, (s32)&x, (s32)&y, (s32)&width, (s32)&height);

    margin = 30 - width;
    /* FAKEMATCH: the one-pass block ends the scheduling block: the native reads the height before it halves the horizontal margin (one reordered row, differ 2 without). */
    do {
        rows = height;
    } while (0);
    x = margin >> 1;
    y = (12 - rows) >> 1;

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

        Func_080c82a8(EventRuntime_GetControlledOwnerFar(), pos);
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
            Ram_HeapSlots->windowTiles[6] = 1;
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
    *(u8 *)(base + TEXT_WORK_TAIL + 5) = zero;
    *(u16 *)(base + TEXT_WORK_TAIL) = zero;
    *(u16 *)(base + TEXT_WORK_TAIL + 2) = zero;
    WaitFrames(3);
}
#endif

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}

void UiWork_ProcessAllOrChannels(void)
{
    if (Ram_HeapSlots->unke4[0] != 0) {
        UiWork_ProcessDirectWork();
        UiWork_ProcessRenderChannels();
        UiWork_AnimateSpriteSlots();
    } else {
        UiWork_ProcessRenderChannels();
    }
}
