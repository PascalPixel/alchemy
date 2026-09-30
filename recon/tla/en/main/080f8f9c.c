#include "TYPES.H"
#include "FIXED_MATH.H"

/* Move a row or page using repeat keys. Returns -1 for no input, zero for
 * a row change and one for a page change. Keep result lifetimes local to
 * each decision and join row changes before the common result exit. */
extern volatile u32 gKeysRepeat;

void Link_DrawShiftedTilePairFar(s32 addr);
void Audio_PlayCue(s32 cue);
void Runtime_SetMainState19(void);

s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor, s32 *page)
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
        next = gKeysRepeat & 16;
        previous = gKeysRepeat & 32;
        horizontal = gKeysRepeat & 64;
        page_forward = gKeysRepeat & 128;
    } else {
        next = gKeysRepeat & 128;
        previous = gKeysRepeat & 64;
        horizontal = gKeysRepeat & 32;
        page_forward = gKeysRepeat & 16;
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
        goto row_changed;
    }
    result = -1;
    if (next == 0)
        goto done;
    {
        Audio_PlayCue(111);
        result = 0;
        if (++*cursor == count - *page * per_page)
            *cursor = result;
        if (*cursor > per_page - 1)
            *cursor = result;
    }
row_changed:
    result = 0;
done:
    return result;
}
