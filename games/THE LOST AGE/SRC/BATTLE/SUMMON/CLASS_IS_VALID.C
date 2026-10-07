
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MOTOBJ.H"


s32 BattleMotion_GetSlotPalette(s32 id)
{
    return BtlObjGet(id)->palette;
}

struct Layout {
    u8 pad[4];
    s16 field[6];
};

s32 EnmChk(s32 arg0)
{
    s32 retval;
    s32 i;
    struct Layout *ptr;

    retval = Summon_IsEntryFlagged(arg0);
    ptr = (struct Layout *)Ram_HeapSlots->battle_work;
    i = 0;
    for (; i < 6; i++) {
        if (ptr->field[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->field[i + 1] == 0)
            break;
    }
    return i != 6;
}
