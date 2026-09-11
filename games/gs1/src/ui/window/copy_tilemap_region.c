#include "types.h"
#include "scene.h"
#include "abi/ui/window/copy_tilemap_region.h"

extern u8 *gIw;

struct UiWindowTilemapRegion {
    u8 padding0[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

s16 *Runtime_BumpAllocateAlternatePool(s32 size);

void UiWindow_CopyTilemapRegion(const struct UiWindowTilemapRegion *window, const void *source)
{
    s16 *mirror = (s16 *)gIw;
    s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
    s16 *input = buffer;
    u32 cell;
    s16 *vram;
    s32 row;

    Ui_Apply(source, buffer);
    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;

    for (row = 0; row < window->height; row++) {
        s32 column;

        for (column = 0; column < window->width; column++) {
            s16 value = *input++;

            *vram++ = value;
            *mirror++ = value;
        }
        vram += 32 - window->width;
        mirror += 32 - window->width;
    }
    Ui_Do(buffer);
}
