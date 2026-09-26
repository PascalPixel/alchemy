#include "TYPES.H"
#include "RENDER_INPUT.H"

extern u8 *gWindowWork;

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

/* Draws a horizontal or vertical divider inside a window, joining it to
   any frame or divider tiles it crosses: each end and each middle cell
   swaps the tile under it for the matching junction piece. */
void UiWindow_DrawDividerLine(struct RenderInput *win, u32 x1, u32 y1, u32 x2, u32 y2)
{
    u8 *base = gWindowWork;
    u16 *cursor;
    u32 pos;
    u32 tile;

    if (x1 == x2) {
        if (y1 == y2)
            return;
        if (y1 > y2) {
            pos = y1;
            y1 = y2;
            y2 = pos;
        }
        UiWindow_ClearTileAttributesInRect(win->x + x2, win->y + y1, 1, y2 - y1);
        cursor = (u16 *)((((win->y + y1) << 5) + win->x + x2) * 2 + (u32)base);
        for (pos = y1; pos <= y2; pos++) {
            tile = *cursor;
            if (pos == y1) {
                switch (tile) {
                case 0xf009:
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                case 0xf018:
                    break;
                case 0xf011:
                    tile = 0xf018;
                    break;
                case 0xf00a:
                    tile = 0xf00d;
                    break;
                case 0xf00e:
                    tile = 0xf009;
                    break;
                default:
                    tile = 0xf00f;
                    break;
                }
            } else if (pos == y2) {
                switch (tile) {
                case 0xf00a:
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                case 0xf019:
                    break;
                case 0xf014:
                    tile = 0xf019;
                    break;
                case 0xf009:
                    tile = 0xf00d;
                    break;
                case 0xf00e:
                    tile = 0xf00a;
                    break;
                default:
                    tile = 0xf00f;
                    break;
                }
            } else {
                switch (tile) {
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                    break;
                case 0xf009:
                case 0xf00a:
                case 0xf00e:
                    tile = 0xf00d;
                    break;
                default:
                    tile = 0xf00f;
                    break;
                }
            }
            *cursor = tile;
            cursor += 32;
        }
    } else if (y1 == y2) {
        if (x1 == x2)
            return;
        if (x1 > x2) {
            pos = x1;
            x1 = x2;
            x2 = pos;
        }
        UiWindow_ClearTileAttributesInRect(win->x + x1, win->y + y1, x2 - x1, 1);
        cursor = (u16 *)((((win->y + y1) << 5) + win->x + x1) * 2 + (u32)base);
        for (pos = x1; pos <= x2; pos++) {
            tile = *cursor;
            if (pos == x1) {
                switch (tile) {
                case 0xf009:
                case 0xf00a:
                case 0xf00b:
                case 0xf00d:
                case 0xf01a:
                    break;
                case 0xf016:
                    tile = 0xf01a;
                    break;
                case 0xf00c:
                    tile = 0xf00d;
                    break;
                case 0xf00f:
                    tile = 0xf00b;
                    break;
                default:
                    tile = 0xf00e;
                    break;
                }
            } else if (pos == x2) {
                switch (tile) {
                case 0xf009:
                case 0xf00a:
                case 0xf00c:
                case 0xf00d:
                case 0xf01b:
                    break;
                case 0xf017:
                    tile = 0xf01b;
                    break;
                case 0xf00b:
                    tile = 0xf00d;
                    break;
                case 0xf00f:
                    tile = 0xf00c;
                    break;
                default:
                    tile = 0xf00e;
                    break;
                }
            } else {
                switch (tile) {
                case 0xf009:
                case 0xf00a:
                case 0xf00d:
                    break;
                case 0xf00b:
                case 0xf00c:
                case 0xf00f:
                    tile = 0xf00d;
                    break;
                default:
                    tile = 0xf00e;
                    break;
                }
            }
            *cursor = tile;
            cursor++;
        }
    }
}
