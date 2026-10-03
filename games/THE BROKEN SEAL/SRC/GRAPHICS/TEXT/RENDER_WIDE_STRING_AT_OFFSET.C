#include "EDITION.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "TBS_EDITION.H"

struct UiWindow;

struct GlyphInfo {
    u16 width;
    u8 unknown_02[30];
};

extern struct GlyphInfo UiText_Glyphs[];

void UiWork_ResetCounters(void);
s32 Func_08018cac(struct UiWindow *window, u32 c, s32 x, s32 y, s32 flags);

void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y)
{
    u8 *base;
    struct UiRenderWork *work;
    u32 c;
    u32 next;
    s16 start;

    base = gWindowWork[0];
    work = (struct UiRenderWork *)base;
    c = 0;
    start = x;
    if (text == NULL) {
        text = work->entries;
        work->entries[work->count] = c;
        work->count = (work->count + 1) & RENDER_ENTRY_MASK;
    }
#if EDITION_INTERNATIONAL
    for (;;) {
        c = *text++;
        if (c > 0xff)
            c = 0x40;
        if (c == 0)
            break;
#else
    while ((c = *text++) != 0) {
#endif
        if (c <= 30) {
            switch (c) {
            case 8:
                work->colour = *text;
                text++;
                break;
            case 9:
                work->outline = *text;
                text++;
                break;
            case 10:
                work->line_spacing = *text;
                text++;
                break;
            case 7:
                UiWork_ResetCounters();
                break;
            case 3:
                x = start;
                y += 15;
                break;
            case 14:
            case 15:
            case 28:
                text++;
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
#if EDITION_INTERNATIONAL
            if ((((struct UiWindow *)window)->flags & 8) == 0) {
                next = *text;
#if defined(TBS_EDITION_ES)
                /* Spanish pairs narrower glyphs in colour 1. */
                if (c > 32 && next > 32) {
                    s16 width = UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width;

                    if (work->outline == 1) {
                        if ((u16)width <= 14) {
                            c |= next << 8;
                            text++;
                        }
                    } else if ((u16)width <= 15) {
                        c |= next << 8;
                        text++;
                    }
                }
#else
                if (c > 32 && next > 32
                    && (u16)(UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width) <= 15) {
                    c |= next << 8;
                    text++;
                }
#endif
            }
#else
            /* A Japanese voicing mark after a kana rides in the glyph's
               upper bits instead of taking a column. */
            if (*text == 0xde) {
                c |= 0x4000;
                text++;
            } else if (*text == 0xdf) {
                c |= 0x8000;
                text++;
            }
#endif
            x += Func_08018cac(window, c, x, y, 0);
        }
    }
}
