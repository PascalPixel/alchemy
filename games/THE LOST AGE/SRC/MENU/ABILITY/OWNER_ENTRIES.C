#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "M7_INTERFACES.H"


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
