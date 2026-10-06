#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "A9_MOTION.H"


void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y = no / phase * 0x10 + origin_y;
    obj->x = no % phase * 0x10 + origin_x;
    UiIcon_PrepareObject(obj);
}
