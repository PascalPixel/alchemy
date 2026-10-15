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
#include "battle_work.h"

struct WindowTiles {
    u8 unknown_00[3];
    u8 dirty;
    u8 unknown_04[4];
    u16 cells[32][32];
};

void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt)
{
    struct WindowTiles *tiles = (struct WindowTiles *)Ram_HeapSlots->window_tiles;

    alt &= 1;
    alt <<= 12;

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
            u16 *p = &tiles->cells[y][x];
            s32 col = width;

            if (col != 0) {
                do {
                    u32 tile = *p;
                    col--;
                    tile = (tile & ~0x1000) | alt;
                    *p = tile;
                    p++;
                } while (col != 0);
            }
            tiles->dirty |= 2 << ((u32)y >> 2);
            /* FAKEMATCH: the ARM9 scheduling would count the row down before marking it dirty. */
            __asm__("");
            height--;
            y++;
        } while (height != 0);
    }
}


s32 BattleLayout_HighlightPartyPanels(u16 *ids)
{
    struct BattleWork *battle = Ram_HeapSlots->battle_work;
    s32 count;
    s32 i;
    s32 j;

    count = BattleParty_ListActorIdsFar(1, 0);
    Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
    i = 0;
    if (ids[0] != 0xff) {
    next_id:
        for (j = 0; j < 4 && battle->party_units[j] != ids[i]; j++) {
            if (battle->party_units[j] == 0xff) {
                j = 4;
                break;
            }
        }
        if (j != 4)
            Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
        if (++i < 4 && ids[i] != 0xff)
            goto next_id;
    }
    return 0;
}
