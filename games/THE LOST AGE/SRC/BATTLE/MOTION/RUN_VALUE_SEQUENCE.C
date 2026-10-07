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
#include "SYSTEM.H"
#include "MOTOBJ.H"
#include "OWNER_STATE.H"
#include "BTLWORK.H"
#include "FIELD_EVENT.H"


void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    UnitGet(id);
    Object_SetMode(BtlObjGet(id)->object, 5);

    sel = selection;
    remaining = 1;
    do {
        target = 0xff;
        sel[1] = target;
        sel[0] = id;
        BattleLayout_HighlightPartyPanelsFar(sel);
        BattleMotion_SetRecordChildValues(BtlObjGet(id)->object, 7);
        WaitFrames(2);

        sel[0] = id;
        BattleLayout_HighlightPartyPanelsFar(sel);
        BattleMotion_SetRecordChildValues(BtlObjGet(id)->object, BattleMotion_GetSlotPalette(id));
        WaitFrames(2);
        remaining--;
    } while (remaining >= 0);

    PtyStDrawFar(Ram_HeapSlots->battle_work->pty_st);
}
