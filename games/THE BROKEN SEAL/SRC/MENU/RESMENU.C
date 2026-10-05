#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "RESMENU.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "GLOBAL_CELLS.H"
#include "AFFINE.H"
#include "VRAM_TAB.H"
#include "text/MSG_IDS.H"

struct WindowLine {
    u16 left;
    u16 span;
};

struct WindowBuffer {
    u8 unknown_000[4];
    struct WindowLine lines[160];
};

extern volatile u32 gFrameCount;
extern u16 Ui_BounceScales[];
extern u8 *gDisplayTransition;

void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *sprite;
    struct AffineTransform effect;
    s32 scale;
    s32 matrix;
    s32 edge;

    /* FAKEMATCH: register choice, measured 42 instructions without. */
    register s32 x asm("r0");
    s32 y;
    u32 end;
    u32 i;
    u16 *line;
    u8 *buffers;

    work = gMenuSelectWork;
    sprite = work->entries;
    scale = Ui_BounceScales[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    effect.scale_x = scale;
    effect.scale_y = scale;
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);
    for (i = 0; i < work->count; i++, sprite++) {
        u32 *attr;
        s32 slot;
        s32 px;

        px = sprite->x;
        if (px == 0)
            continue;
        if (i == work->selection) {
            s32 left = px + scale * 7 / 512 - 20;

            if (sprite->y != 0)
                y = sprite->y + scale * 3 / 256 - 20;
            else
                y = (scale * 15 / 256 - 30) & 255;
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 246;
        } else {
            u32 base;
            u32 value;

            attr = &sprite->link;

            /* FAKEMATCH: instruction order, measured 2 instructions without. */
            __asm__("" : : "r"(attr));
            y = sprite->y;
            *attr++ = 0;
            value = y | (px << 16) | 0x80002000;
            base = gVramBlockCache[sprite->slot].offset;
            *attr++ = value;
            *attr = base >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (work->row == 0) {
        struct WindowBuffer *window;

        /* FAKEMATCH: register choice, measured 4 instructions without. */
        register s32 span asm("r2");

        buffers = gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        x = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((x - span) << 8) + (x + span) + 23;
        end = work->entries[work->selection].y + 24;
        end += (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[24].span;
        i = 24;
        if (i < end) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i < end);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[0].span;
        for (i = 0; i <= 23; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    } else {
        struct WindowBuffer *window;
        s32 start;

        /* FAKEMATCH: register choice, measured 4 instructions without. */
        register s32 span asm("r3");

        buffers = gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        x = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((x - span) << 8) + (x + span) + 23;
        start = work->entries[work->selection].y;
        start -= (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[start].span;
        i = start;
        if (i <= 135) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i <= 135);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[136].span;
        for (i = 136; i <= 159; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    }
}

void AffineEffect_UpdateFrame(void);

void *AffineEffect_InitializeWork(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(58, 152);
    zero = 0;
    Dma_Set(&zero, work, 0x85000026, REG_DMA3);
    Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
    return work;
}

void Menu_EndResourceSelection(void)
{
    struct UiWindow *child;
    s32 i;
    struct ResourceMenuWork *work;

    work = gMenuSelectWork;
    Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
    child = work->window;
    if (child != 0) {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < work->count) {
        Resource_ResetEntry(work->entries[i].slot);
        i += 1;
    }
    Runtime_ReleaseHeapBlock(0x3A);
    WaitFrames(1U);
}


extern volatile u32 gKeyState, gKeysRepeat;

void RenderOutput_PrepareForRedraw(void *);

void UiText_DrawCharacterAtOffset(s32, void *, s32, s32);

void Audio_PlayCue(s32);

s32 Menu_RunResourceSelectionLoop(s32 initial)
{
    struct ResourceMenuWork *work = gMenuSelectWork;
    s32 resource;

    work->selection = initial;
redraw:
    RenderOutput_PrepareForRedraw(work->window);
    if (work->resource_base != 0)
        resource = work->resource_base + work->selection;
    else
        resource = work->resource_ids[work->selection] + (s32)MsgCommandName;
    UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
    for (;;) {
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            Audio_PlayCue(112);
            return work->selection;
        }
        if (gKeyState & KEY_B) {
            Audio_PlayCue(113);
            return -1;
        }
        if (gKeyState & KEY_START) {
            Audio_PlayCue(113);
            return -1;
        }
        if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
            Audio_PlayCue(111);
            work->selection--;
            if (work->selection < 0)
                work->selection = work->count - 1;
            goto redraw;
        }
        if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
            Audio_PlayCue(111);
            work->selection++;
            if (work->selection >= work->count)
                work->selection = 0;
            goto redraw;
        }
    }
}
