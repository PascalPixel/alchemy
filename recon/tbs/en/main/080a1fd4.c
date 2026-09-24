/* Draft, not exact (2026-09-24): candidate=384 reference=368, 132 differing
   halfwords. Hand-written from the assembly; the paging, clamping and key
   decoding follow the reference. Residual: the reference keeps only r5-sl
   (this spelling also takes fp), reuses the first parameter for the page-back
   key, sets the -1 and 0 results before their tests, and shares the page clamp
   tail between the two page branches. */
#include "TYPES.H"

#define KEYS_REPEAT (*(volatile u32 *)0x03001b04)

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

    if (count == 0)
        return -1;
    Link_DrawShiftedTilePairFar(0x06002500);
    pages = Math_Div(count, per_page);
    if (Math_Mod(count, per_page) != 0)
        pages++;
    if (horizontal) {
        next = KEYS_REPEAT & 16;
        previous = KEYS_REPEAT & 32;
        horizontal = KEYS_REPEAT & 64;
        page_forward = KEYS_REPEAT & 128;
    } else {
        next = KEYS_REPEAT & 128;
        previous = KEYS_REPEAT & 64;
        horizontal = KEYS_REPEAT & 32;
        page_forward = KEYS_REPEAT & 16;
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
        return 1;
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
        return 1;
    }
    if (previous) {
        Audio_PlayCue(111);
        if (--*cursor < 0) {
            *cursor = per_page - 1;
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        return 0;
    }
    if (next) {
        Audio_PlayCue(111);
        if (++*cursor == count - *page * per_page)
            *cursor = 0;
        if (*cursor > per_page - 1)
            *cursor = 0;
        return 0;
    }
    return -1;
}
