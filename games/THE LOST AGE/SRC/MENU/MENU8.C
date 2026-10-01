#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 CharacterMenu_CursorWidths[];
void Render_SetTilemapFlagRect(const u8 *, s32, s32, s32, s32, u32);

/* ⚓️ keeps the cursor's tilemap object at 0x28; ☀️ at 0x24. */
struct CursorMenuState {
    u8 padding[40];
    u8 *object;
};

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct CursorMenuState *state = Ram_HeapSlots->menu_runtime;
    u32 different;
    s32 count;
    s32 index;
    s32 x;
    s32 y;
    s32 last;
    u8 width;

    if (mode == 0) {
        y = selected * 2 + 5;
        x = 0;
        width = 5;
        count = 0;
        index = 0;
        while (index <= 4) {
            if (entries[index] != 0) {
                if (selected == count) {
                    width = CharacterMenu_CursorWidths[index];
                    break;
                }
                count++;
            }
            index++;
        }
    } else if (selected <= 3) {
        y = selected;
        x = 5;
        width = 13;
    } else {
        y = selected + 4;
        x = 8;
        width = 20;
    }

    different = 1 ^ (u32)invert;
    last = 15 - (((0u - different) | different) >> 31);
    Render_SetTilemapFlagRect(state->object, x, y, width, 1, last);
}
