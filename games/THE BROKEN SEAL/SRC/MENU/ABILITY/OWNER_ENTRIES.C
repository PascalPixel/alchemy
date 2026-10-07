#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "M7_INTERFACES.H"

#include "PSYNERGY_MENU.H"


void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
    PsynergyMenu_RefreshOwnerEntries(108, 40, 8);
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
    } while (index <= 31);
}

void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 columns) {
    struct RenderOutput *object;

    if (index > 31) {
        index = 0;
    }
    object = *slot;
    object->y = (index / columns) * 16 + origin_y;
    object->x = (index % columns) * 16 + origin_x;
    UiIcon_PrepareObject(object);
}
