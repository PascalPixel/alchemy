#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "M7_INTERFACES.H"


void ItemMenu_PosOwner(struct RenderOutput **slot, s32 index,
    s32 origin_x, s32 origin_y, s32 columns)
{
    struct RenderOutput *object;

    if (index > 15)
        index = 0;
    object = *slot;
    object->y = index / columns * 16 + origin_y;
    object->x = index % columns * 24 + origin_x;
    UiIcon_PrepareObject(object);
}
