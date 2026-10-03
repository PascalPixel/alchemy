#include "TYPES.H"
#include "TBS_EDITION.H"
#include "MENU_LIST.H"

s32 UiText_RenderStringTiles(u16 *, s32, s32, s32);

void UiText_RenderGlyphTileAtWorkOffset(
    u16 *buffer,
    const struct RenderInput *work,
    s32 offset_x,
    s32 offset_y)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u32 cell;

    if (buffer == NULL) {
        u16 *counter = &canvas->count;

        buffer = canvas->entries;
        canvas->entries[*counter] = 0;
        *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    }

    cell = ((work->y + offset_y + 1) << 5)
        + (work->x + offset_x) + 1;
    if (cell < 0x280) {
        s32 src;
        s32 dst;

        cell *= 2;
        dst = 0x06002000 + cell;
        src = (s32)canvas->tilemap + cell;

        UiText_RenderStringTiles(
            buffer,
            src,
            dst,
            0x06002000);
    }
}
