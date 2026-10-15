#include "types.h"
#include "window.h"
#include "ram_buffer.h"
#include "frame.h"
#include "scene_call.h"
#include "text/msg_ids.h"

s32 Func_08040798(s32 selection);
s32 Func_08043690(void);
s32 Func_080400e8(void);

// Runs the save screen's selection until a choice completes: saving (refused
// while flag 0x17c or 0x17d is set), sleep mode or the options. Returns -1
// when the screen is cancelled and 0 otherwise.
s32 Func_0804e0d0(void)
{
    s32 selection = 0;

again:
    selection = Func_08040798(selection);
    if (selection == -1)
        return -1;
    if (selection == 0) {
        if (GameFlag_Test(0x17c)) {
            UiText_ShowPositionedMessageAndWait((s32)MsgCannotSaveHere, 1);
            goto again;
        }
        if (GameFlag_Test(0x17d)) {
            UiText_ShowPositionedMessageAndWait((s32)MsgNoPlaceForThat, 1);
            goto again;
        }
        if (Func_08043690() == -1)
            goto again;
    } else if (selection == 1) {
        UiText_ShowPositionedMessageAndWait((s32)MsgSleepMode, 1);
        gSleepRequested = selection;
    } else if (selection == 2) {
        if (Func_080400e8() == -1)
            goto again;
    }
    return 0;
}

void Render_SetTilemapFlagBitRect(
    const struct UiWindow *object, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;

    x += (s16)object->x + 1;
    y += (s16)object->y + 1;
    field &= 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        do {
            u16 *pixel = &work->tilemap[y][x];
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *pixel;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *pixel = value;
                pixel++;
            }
            height--;
            y++;
        } while (height != 0);
        work->dirty = 1;
    }
}
