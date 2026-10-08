#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "m7_interfaces.h"

#include "psynergy_menu.h"


void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
    PsynergyMenu_RefreshOwnerEntries(0x6C, 0x28, 8);
}

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    s32 index;

    index = 0;
    slot = gMenuWork->entry_icons;
    scan = slot;
    do {
        if (*scan++ != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, columns);
        }
        index++;
        slot++;
    } while (index <= 0x1F);
}

void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 columns) {
    struct RenderOutput *object;

    if (index > 0x1F) {
        index = 0;
    }
    object = *slot;
    object->y = (index / columns) * 0x10 + origin_y;
    object->x = (index % columns) * 0x10 + origin_x;
    UiIcon_PrepareObject(object);
}
