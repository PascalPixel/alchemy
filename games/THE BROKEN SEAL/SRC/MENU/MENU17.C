#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "RESMENU.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "GLOBAL_CELLS.H"

void AffineEffect_UpdateFrame(void);


void *AffineEffect_InitializeWork(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(58, 152);
    zero = 0;
    Dma_Set(&zero, work, 0x85000026, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback((s32)(AffineEffect_UpdateFrame), 0xc76);
    return work;
}

void Menu_EndResourceSelection(void)
{
    struct UiWindow *child;
    s32 i;
    struct ResourceMenuWork *work;

    work = gMenuSelectWork;
    Scheduler_RemoveCallback((u32)(AffineEffect_UpdateFrame));
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


extern u8 MsgCommandName;

extern volatile u32 gKeyState, gKeysRepeat;

void RenderOutput_PrepareForRedraw(void *);

void UiText_DrawCharacterAtOffset(s32, void *, s32, s32);

void Audio_PlayCue(s32);

/* Runs the open selection from INITIAL: draws the current entry's name
   (base message plus the index, or command name plus the entry's id), moves
   with the pad and returns the chosen index, or -1 when cancelled. */
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
        resource = work->resource_ids[work->selection] + (s32)&MsgCommandName;
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
