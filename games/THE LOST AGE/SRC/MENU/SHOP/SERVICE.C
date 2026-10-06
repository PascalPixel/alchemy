#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"
#include "TYPES.H"


s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
    u8 value = ((struct BattleUnit *)Owner_GetState(entry_no))->level;
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    struct BattleUnit *entry = Owner_GetState(entry_no);
    s32 result = 0;

    if ((kind == 0 && entry->hp <= 0)
        || (kind == 1 && entry->poison != 0)
        || (kind == 2 && entry->evil_spirit != 0)
        || (kind == 3 && entry->restraint != 0)) {
        result = 1;
    }
    return result;
}
