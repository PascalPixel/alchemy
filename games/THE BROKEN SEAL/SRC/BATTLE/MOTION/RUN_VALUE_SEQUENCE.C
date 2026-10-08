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
#include "SYSTEM.H"
#include "FIELD_EVENT.H"

#include "MOTOBJ.H"

#include "BTLWORK.H"

#include "BTLSYS.H"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    UnitGetFar(id);
    ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 5);

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

    PtyStDrawFar(BtlWk->pty_st);
}
