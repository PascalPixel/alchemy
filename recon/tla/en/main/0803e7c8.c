/* Near miss, differ 6 of 140 (three reordered pairs), 2026-10-15: The
 * Broken Seal's Menu_SetupSelectionSide on this game's cursor tiles
 * (Data_0805c5c4 right, Data_0805c1c4 left). One empty one-pass block after
 * the left side's x (found by a device search, differ 12 without).
 * Remaining: on the left side the native loads base_x before it builds
 * 0xfff7 and stores the x before it copies the left tiles to r11; when the
 * entry is first set up it builds the size 128 before it reloads the slot
 * for VramBlock_LoadCached. A barrier inside the call's arguments or a size
 * temporary did not help.
 */
#include "types.h"
#include "resource.h"
#include "select.h"

extern u8 Data_0805c5c4[];
extern u8 Data_0805c1c4[];

void Func_0803e7c8(struct SelectionScreen *state, s32 index)
{
    struct SelectionSprite *entry = &state->records[index].sprite;
    u8 *frames = 0;
    u32 count;

    state->records[index].kind = 0;
    if (index != 0) {
        u8 *right = Data_0805c5c4;

        count = state->count;
        if (state->top != 0)
            count -= state->top;
        if (count > 5) {
            state->records[index].kind = 1;
            count = 5;
        }
        frames = right;
        state->records[1].x = state->base_x + 16 * (count - 1) + 17;
    } else {
        frames = Data_0805c1c4;
        state->records[0].x = state->base_x - 9;
    do { } while (0);
        if (state->top != 0)
            state->records[0].kind = 1;
    }
    if (state->records[index].y == 0) {
        state->records[index].slot = Resource_FindFreeEntry();
        state->records[index].tile = VramBlock_LoadCached(state->records[index].slot, 128, frames);
        state->records[index].y = state->base_y;
        state->records[index].base = 0;
        entry->mode = 0;
        entry->mosaic = 0;
        entry->colors = 1;
        entry->affine = 0;
        entry->param = 0;
        entry->size = 0;
        entry->shape = 2;
        entry->priority = 0;
    }
}
