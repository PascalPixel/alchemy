/* Near miss, differ 2 of 122: the twin of The Broken Seal's UiMenu_SlideCursor.
   The native loads the 0x1ff mask of the sprite's x field before it subtracts 56
   (adds r1, r1, r3; ldr r3, [pc]; subs r1, #56); here the subtraction comes first.
   The mask belongs to the bit-field store, so no source value can carry a barrier;
   splitting the sum from the subtraction changes nothing. */
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct OwnerMenuState {
    u8 unknown_000[16];
    struct RenderInput *owner_window;
    struct RenderOutput *owner_cursors[2];
    s8 owner_index[2];
    u8 unknown_01e[0x1fe];
    u16 page;
    u16 skip_slide;
};

void WaitFrames(s32 frames);

void Func_080f8ab4(s32 x, s32 y)
{
    struct OwnerMenuState *work = (struct OwnerMenuState *)Ram_HeapSlots->menu_runtime;
    struct RenderOutput *cursor;
    s32 steps;
    s32 start_x;
    s32 start_y;
    s32 px;
    s32 py;
    s32 dx;
    s32 dy;

    steps = 2;
    if (work->skip_slide != 0) {
        work->skip_slide = 0;
        return;
    }
    cursor = work->owner_cursors[0];
    {
        s32 sprite_x = cursor->oam.x;
        s32 sprite_y = cursor->oam.y;

        cursor->x = sprite_x + 64;
        cursor->y = sprite_y + 64;
    }
    x += 64;
    y += 64;
    if (cursor->x - 8 > 0)
        cursor->x -= 8;
    py = cursor->y;
    if (py - 8 > 0) {
        cursor->y -= 8;
        py = cursor->y;
    }
    start_x = cursor->x << 4;
    dx = ((x << 4) - start_x + 1) / steps;
    start_y = py << 4;
    dy = ((y << 4) - start_y + 1) / steps;
    px = start_x;
    py = start_y;
    do {
        px += dx;
        {
            s32 sprite_x = (px >> 4) + (work->owner_window->x << 3);

            cursor->oam.x = cursor->x = sprite_x - 56;
        }
        py += dy;
        cursor->oam.y = cursor->y = (py >> 4) + (work->owner_window->y << 3) - 56;
        steps--;
        if (steps != 0)
            WaitFrames(1);
    } while (steps != 0);
}
