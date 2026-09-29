/* DRAFT: 564/572 bytes, 266 differing halfwords, 207 aligned edits. The
 * linked mask now uses ands, but the short-reach message puts the first
 * pool at +0x9c instead of the reference +0xe8. The index stays in fp
 * instead of sp+44; the window occupies sp+48 instead of sp+32. The ROM
 * keeps the mask in fp and the offer flag in r8. The 0x03001F2C work pointer
 * derives the window pointer by subs #160 as in the ROM. Not adopted. */
#include "TYPES.H"

/* H1 (2026-09-27): whole [080aafb8,080ab1f4), 572 bytes, including the
 * mid-loop and final pools. Exact CORE_COMPUTE_ENTRY_VALUES.C and the Djinn
 * menu caller confirm eight rows of ten halfword IDs plus signed counts.
 * The ROM tests a pooled mask with ands, not a single-bit shift. Transfer
 * the proven linked-constant recipe before changing any loop lifetimes.
 * Prediction: pooled 0x8000 test and original pool boundary/spill pressure.
 * Gate: complete exact extent plus compare/test/coverage/verify. Budget:
 * one corrected model and two justified variants, checkpoint 00:10 Lisbon.
 * Read full aligned diffs; preserve all results here and in Git.
 * H1: 560/572 bytes, 253 differing halfwords, 185 aligned edits. The test
 * now uses ands, but its SImode linked constant leaves all literals at the
 * end. Index stays in fp instead of its reference spill, and row addressing
 * is scaled offset induction rather than a 20-byte row-pointer walk.
 * H2: reference pool spells message 0x0bad as a halfword. Transfer the
 * short-reach HImode linked-message recipe: predict a mid-loop pool and
 * matching long-branch layout without changing the proven row semantics.
 * H2 result: 572/572 bytes, 232 differing halfwords, 174 aligned edits.
 * The pool moves too early (+0xa0 versus +0xe8), leaving the 0x8000 mask
 * in the final pool. Full normalized diff read: initial index allocation,
 * row induction, repeated ID loads and offer-flag lifetime remain wrong.
 * STOP at the 00:10 checkpoint: two models preserved, zero new DONE.
 * A third model remains untested: carry an explicit u16 row pointer with
 * a ten-element advance, as exact CORE_COMPUTE_ENTRY_VALUES.C and the ROM
 * outer-tail +20 update do. Do not repeat constant spellings or RA sweeps.
 * H3 (resumed 00:11): carry the row's typed ID pointer explicitly. Predict
 * the reference sp+4 pointer induction and +20 outer-tail update, removing
 * the repeated scaled-offset construction. Compare the entire 52-byte
 * frame, both fill/render loops, calls and pools. Acceptance remains exact
 * 572-byte owner plus compare/test/coverage/verify; one model plus at most
 * two evidence-backed follow-ups, stop by 00:30 and record every result.
 * H3 result: 564/572, 253 differing halfwords, 182 aligned edits. Explicit
 * +20 row-pointer induction is recovered, but its slot is sp+28 versus
 * sp+4 and initialized before the empty-row guard. Frame stays 52 bytes.
 * The index remains in fp and the offer flag spills. Read the complete
 * diff: repeated ID argument expressions emit two loads for the second
 * query where the ROM loads one ID; that is a call-input lifetime fact.
 * H4: make each call's packed-ID snapshot explicit, reloading only after
 * a call that may mutate the list. Predict one ldrh before each query and
 * drawing call, with v surviving the palette branch as in the ROM.
 * H4 result: 568/572, 251 differing halfwords, 185 aligned edits; full
 * diff read. Explicit snapshots remove the second query's duplicate ldrh,
 * but v still lives in r2 and the offer flag spills around both queries.
 * The reference's 52-byte frame remains; first and inner list indices
 * still share fp. This is not an accepted model of the whole frame.
 * H5: exact DJINN/SHOW_CURRENT_LIST.C gives setup loops their own block
 * counter; allocator evidence shows our shared i occupies fp in both
 * fill and render loops. Split only that lifetime. Prediction: independent
 * counters permit the reference spill/mask ownership without changing
 * row-pointer induction or call arguments. This is the final follow-up.
 * H5 result: 564/572, 266 differing halfwords, 207 aligned edits; full
 * diff read. The separate fill counter spills around its call, but grows
 * the frame to 56/52 bytes and leaves the render index in fp. Rejected as
 * a closing model. STOP: resumed one-model/two-follow-up budget exhausted;
 * zero new DONE. H3 pointer induction and H4 single query load are useful
 * local facts, not a credible whole-frame match. Preserve and move on.
 */
extern u8 Value_00008000;
extern u8 Value_00000bad;

/* menu/djinn_draw_element_list.c */
struct DjinnListTable {
    u16 ids[8][10];
    s8 counts[8];
};

struct MenuWork {
    u8 padding0[0x30];
    void *window;
    u8 padding34[0x208 - 0x34];
    u16 owners[8];
    u8 padding218;
    u8 owner_count;
};

/* IWRAM work pointers from 0x03001E8C. */
struct WorkPointers {
    u8 *window;
    u8 padding04[0x9c];
    struct MenuWork *menu;
};

extern struct WorkPointers gWindowWork;

s8 Djinn_ListOwnerEntries(void *list, s32 owner, s32 mode);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void UiWork_SetParamNibbleFar(s32 value);
s32 Trade_CanOfferDjinnFar(s32 owner, s32 element, s32 djinn);
s32 Func_08077208(s32 owner, s32 element, s32 djinn);
void UiWindow_SetTilemapEntryFar(void *window, s32 tile, s32 x, s32 y, s32 flags);
void UiWindow_DrawDividerLineFar(void *window, s32 x, s32 y, s32 width, s32 height);

void DjinnMenu_DrawElementList(struct DjinnListTable *tbl)
{
    struct MenuWork *state;
    u8 *window;
    s32 i;
    s32 row;
    s32 element;
    s32 line;
    u16 *row_ids;
    u16 *id;
    s32 flag;
    u32 v;
    u32 mask;

    state = *(struct MenuWork **)((u8 *)&gWindowWork + 0xa0);
    window = gWindowWork.window;
    window[0xea6] = 1;
    {
        s32 owner;

        for (owner = 0; owner < state->owner_count; owner++)
            tbl->counts[owner] = Djinn_ListOwnerEntries(tbl->ids[owner], state->owners[owner], -1);
    }
    RenderOutput_RedrawSavedRectFar(state->window);
    UiText_DrawCharacterAtOffsetFar((u16)(u32)&Value_00000bad, state->window, 0, 80);
    for (row = 0, row_ids = tbl->ids[0]; row < state->owner_count; row++, row_ids += 10) {
        line = 0;
        for (element = 0; element < 4; element++) {
            for (i = 0; i < tbl->counts[row]; i++) {
                id = &row_ids[i];
                mask = 0xe0;
                v = *id;
                if (element != (v & mask) >> 5)
                    continue;
                if ((v & (u32)&Value_00008000) == 0) {
                    UiWork_SetParamNibbleFar(2);
                    v = *id;
                }
                flag = 0;
                if (Trade_CanOfferDjinnFar((v & 0xf00) >> 8, (v & mask) >> 5, v & 31))
                    flag = 1;
                else {
                    v = *id;
                    if (Func_08077208((v & 0xf00) >> 8, (v & mask) >> 5, v & 31))
                        flag = 1;
                }
                if (!flag)
                    UiWork_SetParamNibbleFar(4);
                v = *id;
                UiWindow_SetTilemapEntryFar(state->window, ((v & mask) >> 5) + 0x5001, row * 7 + 1, line + 2, 0);
                v = *id;
                UiText_DrawCharacterAtOffsetFar(((v & mask) >> 5) * 20 + (v & 31) + 0x45f, state->window, row * 56 + 16, line * 8 + 16);
                line++;
                UiWork_SetParamNibbleFar(15);
            }
        }
    }
    UiWindow_DrawDividerLineFar(state->window, 0, 10, 28, 10);
    gWindowWork.window[0xea3] = 1;
    window[0xea6] = 0;
}
