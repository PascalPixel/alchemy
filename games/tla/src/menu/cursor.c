#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

void Render_SetTilemapFlagRect(const u8 *, s32, s32, s32, s32, u32);

struct CursorMenuState {
    u8 padding[40];
    u8 *object;
};

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct CursorMenuState *state = Ram_HeapSlots->menuRuntime;
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
                    width = gCharacterMenu_CursorWidths[index];
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

    different = 1 ^ invert;
    last = 15 - (((0u - different) | different) >> 31);
    Render_SetTilemapFlagRect(state->object, x, y, width, 1, last);
}

void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected,
    const u8 *entries)
{
    s32 count = 0;
    s32 index = 0;

    do {
        if (entries[index] != 0) {
            if (selected == count) {
                s32 message = (s32)(MsgStatusAdvice + index * 2);
                UiText_DrawResourceFar(message, target, 0, -1);
                UiText_DrawResourceFar(message + 1, target, 0, 15);
            }
            count++;
        }
        index++;
    } while (index <= 4);

    if (count == 0)
        UiText_DrawResourceFar((s32)MsgStatusNormal, target, 0, 0);
}
