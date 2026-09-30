#include "TYPES.H"
#include "TBS_EDITION.H"

/* One of the three queued text renders in the glyph work area. */
struct TextRender {
    void *entries;
    s16 x;
    s16 y;
    u16 colours[4];
    s16 unknown_10;
    s16 window;
    s16 unknown_14;
    s16 unknown_16;
    s16 unknown_18;
    s16 unknown_1a;
    s16 unknown_1c;
    s16 start_x;
    s16 unknown_20;
    s16 unknown_22;
    s16 flags;
    s16 unknown_26;
};

extern u8 *gWindowWork;

/* Claims the first of the window work's three text render slots for a
   built entry list, placing it at x, y (whole pixels) in window with the
   given colours (or colour 0) and flags. Returns the slot, or NULL. */
struct TextRender *UiText_QueueRenderEntries(void *entries, s32 window, s32 x, s32 y, u16 *colours, s32 flags)
{
    struct TextRender *render = (struct TextRender *)(gWindowWork + RENDER_CHANNEL_OFS);
    struct TextRender *found = NULL;
    u32 i;

    for (i = 0; i != 3; i++, render++) {
        if (render->entries == NULL) {
            found = render;
            break;
        }
    }
    if (found != NULL) {
        found->entries = entries;
        found->start_x = x << 8;
        found->x = x << 8;
        found->y = y << 8;
        found->window = window;
        found->unknown_16 = 15;
        found->unknown_1a = 10;
        found->unknown_14 = 0;
        found->unknown_18 = 0;
        found->unknown_20 = 0;
        found->flags = flags;
        if (colours != NULL) {
            for (i = 0; i < 4; i++)
                found->colours[i] = *colours++;
        } else {
            for (i = 0; i < 4; i++)
                found->colours[i] = 0;
        }
        found->unknown_10 = 0;
    }
    return found;
}
