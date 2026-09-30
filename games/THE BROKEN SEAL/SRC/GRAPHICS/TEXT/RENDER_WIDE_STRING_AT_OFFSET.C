#include "TYPES.H"
#include "TBS_EDITION.H"

struct TextWindow {
    u8 unknown_00[22];
    u16 flags;
};

struct GlyphInfo {
    u16 width;
    u8 unknown_02[30];
};

struct TextWork {
    u8 unknown_000[RENDER_WORD2_OFS];
    u16 color_a;
    u16 unknown_eaa;
    u16 color_b;
    u16 color_c;
    u16 entries[0x200];
    u16 unknown_12b0;
    u16 count;
};

extern u8 *gWindowWork;
extern struct GlyphInfo UiText_Glyphs[];

void UiWork_ResetCounters(void);
s32 Func_08018cac(struct TextWindow *window, u32 c, s32 x, s32 y, s32 flags);

void UiText_RenderWideStringAtOffset(u16 *text, struct TextWindow *window, s32 x, s32 y)
{
    u8 *base;
    struct TextWork *work;
    u32 c;
    u32 next;
    s16 start;

    base = gWindowWork;
    work = (struct TextWork *)base;
    c = 0;
    start = x;
    if (text == NULL) {
        text = work->entries;
        work->entries[work->count] = c;
        work->count = (work->count + 1) & 0x1ff;
    }
    for (;;) {
        c = *text++;
        if (c > 0xff)
            c = 0x40;
        if (c == 0)
            break;
        if (c <= 30) {
            switch (c) {
            case 8:
                work->color_c = *text;
                text++;
                break;
            case 9:
                work->color_b = *text;
                text++;
                break;
            case 10:
                work->color_a = *text;
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
            case 29:
                text++;
                break;
            }
        } else {
            if ((window->flags & 8) == 0) {
                next = *text;
#if defined(TBS_EDITION_ES)
                /* Spanish pairs narrower glyphs in colour 1. */
                if (c > 32 && next > 32) {
                    s16 width = UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width;

                    if (work->color_b == 1) {
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
            x += Func_08018cac(window, c, x, y, 0);
        }
    }
}
