/* NONMATCHING: 300 bytes, candidate 300, 112 differing halfwords, 82 halfword edits.
 * Fresh reconstruction from the complete listing (2026-09-25).
 * WALL: The polling loop and cancel branch have different block order; size and volatile input reads now agree.
 * 2026-09-29 (alchemy permute scorer): the draft scored 2200. The permuter
 * found that the polling loop written as while (1) instead of for (;;)
 * fixes most of the block order the WALL above describes: 605, and 565 with
 * the callees and globals under their build names. Remaining: the accept and
 * cancel tails are laid out in the opposite order (113 before 112), three
 * branches follow from that, and the resource-id index uses r0/r2 where
 * the reference has r1/r0. The 0x1f offset is still the Value_ symbol:
 * written as a plain 0x1f (in any width) GCC builds it with an add instead
 * of the reference's pool load and the score rises to 820, so this stays
 * a draft.
 */
#include "TYPES.H"
struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};
extern struct MenuSelectionState *gMenuSelectWork;
extern u8 Value_0000001f;
extern volatile u32 gKeyState, gKeysRepeat;
void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawCharacterAtOffset(s32, void *, s32, s32);
void WaitFrames(s32);
void Audio_PlayCue(s32);
s32 Menu_RunResourceSelectionLoop(s32 initial)
{
    struct MenuSelectionState *work = gMenuSelectWork;
    s32 resource;

    work->selection = initial;
    for (;;) {
        RenderOutput_PrepareForRedraw(work->work);
        if (work->resource_base != 0)
            resource = work->resource_base + work->selection;
        else
            resource = work->resource_ids[work->selection] + (s32)&Value_0000001f;
        UiText_DrawCharacterAtOffset(resource, work->work, 0, 0);
        while (1) {
            WaitFrames(1);
            if (gKeyState & 1)
                goto accept;
            if (gKeyState & 2)
                goto cancel;
            if (gKeyState & 8)
                goto cancel;
            if ((gKeysRepeat & 32) || (gKeysRepeat & 64)) {
                Audio_PlayCue(111);
                work->selection--;
                if (work->selection < 0)
                    work->selection = work->item_count - 1;
                break;
            }
            if ((gKeysRepeat & 16) || (gKeysRepeat & 128)) {
                Audio_PlayCue(111);
                work->selection++;
                if (work->selection >= work->item_count)
                    work->selection = 0;
                break;
            }
        }
    }
cancel:
    Audio_PlayCue(113);
    return -1;
accept:
    Audio_PlayCue(112);
    return work->selection;
}
