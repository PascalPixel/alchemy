/* Draft, not exact (2026-09-24): candidate=384 reference=368, 132 differing
   halfwords. Hand-written from the assembly; the paging, clamping and key
   decoding follow the reference. Residual: the reference keeps only r5-sl
   (this spelling also takes fp), reuses the first parameter for the page-back
   key, sets the -1 and 0 results before their tests, and shares the page clamp
   tail between the two page branches. */
#include "TYPES.H"

/* H1 (2026-09-26): complete [080a1fd4,080a2144) owner, 368 bytes.
 * Exact SELECT_ACTION/RUN_LIST callers confirm signed row/page pointers and
 * the -1/no input, 0/row, 1/page result contract. Transfer the named volatile
 * key cell from exact FLAG_GRID_INPUT.C; the literal-address model can keep
 * a different base lifetime. All five callee interfaces are retained.
 * Prediction: reference key base and fewer saved-register differences.
 * One corrected model plus at most two evidence-backed variants; full extent
 * exactness and compare/test/coverage/verify are required for adoption.
 * H1: 384/368 bytes, 132 differing halfwords, 100 aligned edits; unchanged
 * from the old literal cell. The extra live previous-key zero survives the
 * next-key Audio_PlayCue and spends a saved register; literal returns also
 * share a late -1 block instead of the reference's early result values.
 * H2: model one result local and common exit, initialized at each decision
 * boundary. The next-key branch writes its own zero rather than depending
 * on the previous-key value surviving Audio_PlayCue. Predict early -1/zero
 * materialization and removal of the extra saved-register lifetime.
 * H2: 368/368 bytes, 29 differing halfwords, seven aligned edits. All
 * register roles and the first 120 instructions now agree. Remaining:
 * previous-row exits use a separate zero block before next-row handling;
 * ROM shares the final zero-result block after both row-change branches.
 */
extern volatile u32 Data_03001b04;

void Link_DrawShiftedTilePairFar(s32 addr);
s32 Math_Div(s32 numerator, s32 denominator);
s32 Math_Mod(s32 numerator, s32 denominator);
void Audio_PlayCue(s32 cue);
void Runtime_SetMainState19(void);

s32 Func_080a1fd4(s32 horizontal, s32 count, s32 per_page, s32 *cursor, s32 *page)
{
    s32 pages;
    s32 next;
    s32 previous;
    s32 page_forward;
    s32 result;

    result = -1;
    if (count == 0)
        goto done;
    Link_DrawShiftedTilePairFar(0x06002500);
    pages = Math_Div(count, per_page);
    if (Math_Mod(count, per_page) != 0)
        pages++;
    if (horizontal) {
        next = Data_03001b04 & 16;
        previous = Data_03001b04 & 32;
        horizontal = Data_03001b04 & 64;
        page_forward = Data_03001b04 & 128;
    } else {
        next = Data_03001b04 & 128;
        previous = Data_03001b04 & 64;
        horizontal = Data_03001b04 & 32;
        page_forward = Data_03001b04 & 16;
    }
    if (horizontal) {
        Audio_PlayCue(111);
        if (--*page < 0)
            *page = pages - 1;
        if (*cursor + *page * per_page > count - 1) {
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        Runtime_SetMainState19();
        result = 1;
        goto done;
    }
    if (page_forward) {
        Audio_PlayCue(111);
        if (++*page > pages - 1)
            *page = 0;
        if (*cursor + *page * per_page > count - 1) {
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        Runtime_SetMainState19();
        result = 1;
        goto done;
    }
    if (previous) {
        Audio_PlayCue(111);
        if (--*cursor < 0) {
            *cursor = per_page - 1;
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        result = 0;
        goto done;
    }
    result = -1;
    if (next) {
        Audio_PlayCue(111);
        result = 0;
        if (++*cursor == count - *page * per_page)
            *cursor = result;
        if (*cursor > per_page - 1)
            *cursor = result;
    }
done:
    return result;
}
