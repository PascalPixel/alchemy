#include "EDITION.H"
#include "TYPES.H"
#include "TBS_EDITION.H"

struct TextWork {
    u8 unknown_000[RENDER_DIRTY_OFS];
    u8 dirty;
    u8 unknown_ea4[0xc];
    u16 entries[RENDER_ENTRY_MASK + 1];
    u16 unknown_12b0;
    u16 count;
};

extern u8 *gWindowWork;

/* Draws a wide string one glyph tile per column; 3 starts a new row, the
   other control codes skip their operands. */

s32 UiWindow_PutGlyph(void *window, u32 c, s32 x, s32 y, s32 flags);

void UiText_RenderWideStringInWindow(s16 *text, void *window, s32 x, s32 y)
{
    struct TextWork *work;
    u16 c;
    s16 start;

    work = (struct TextWork *)gWindowWork;
    start = x;
    if (text == NULL) {
        text = (s16 *)work->entries;
        work->entries[work->count] = 0;
        work->count = (work->count + 1) & RENDER_ENTRY_MASK;
    }
    c = *text++;
    if (c != 0) do {
        if (c <= 30) {
            switch (c) {
            case 3:
                x = start;
                y++;
                break;
            case 14:
            case 15:
            case 28:
                text++;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 17:
#if EDITION_INTERNATIONAL
            /* Code 29 carries an operand outside the Japanese edition. */
            case 29:
#endif
                text++;
                break;
            }
        } else {
            UiWindow_PutGlyph(window, c, x, y, 0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            /* The European editions advance a column for every glyph. */
            x++;
#else
            if ((u16)(c - 0xde) > 1)
                x++;
#endif
        }
        c = *text++;
    } while (c != 0);
    work->dirty = 1;
}
