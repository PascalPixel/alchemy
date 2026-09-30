#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
extern u8 Data_03001f38[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void Scheduler_RemoveCallback(void *);
void UiWork_Finalize(struct Work *work, s32 release);
s32 Resource_ResetEntry(u32 index);
void AffineEffect_UpdateFrame(void);

void Menu_EndResourceSelection(void)
{
    struct Work *child;
    s32 i;
    u16 *entry;
    void *work;

    work = *(void **)((u32)&Data_03001f38);
    Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
    child = FIELD_AT_OFFSET(work, struct Work *, 0x78);
    if (child != 0) {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < (s32)FIELD_AT_OFFSET(work, s16, 0x8E)) {
        entry = (u16 *)((u8 *)work + 0x12) + i * 10;
        Resource_ResetEntry(*entry);
        i += 1;
    }
    Runtime_ReleaseHeapBlock(0x3A);
    WaitFrames(1U);
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct MenuSelectionState {
    u8 unknown_000[0x78];
    void *window;
    u8 unknown_07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 width;
    s16 resource_base;
};
extern struct MenuSelectionState *gMenuSelectWork;
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
    struct MenuSelectionState *work = gMenuSelectWork;
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
        if (gKeyState & 1) {
            Audio_PlayCue(112);
            return work->selection;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            return -1;
        }
        if (gKeyState & 8) {
            Audio_PlayCue(113);
            return -1;
        }
        if ((gKeysRepeat & 32) || (gKeysRepeat & 64)) {
            Audio_PlayCue(111);
            work->selection--;
            if (work->selection < 0)
                work->selection = work->item_count - 1;
            goto redraw;
        }
        if ((gKeysRepeat & 16) || (gKeysRepeat & 128)) {
            Audio_PlayCue(111);
            work->selection++;
            if (work->selection >= work->item_count)
                work->selection = 0;
            goto redraw;
        }
    }
}
#endif
