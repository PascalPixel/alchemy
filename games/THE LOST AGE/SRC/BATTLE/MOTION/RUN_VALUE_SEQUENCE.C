#include "PROTO.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "MOTION_OBJECT.H"
#include "OWNER_STATE.H"
#include "BATTLE_WORK.H"
#include "FIELD_EVENT.H"


void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    Owner_GetState(id);
    Object_SetMode(GetBattleObjectSlot(id)->object, 5);

    sel = selection;
    remaining = 1;
    do {
        target = 0xff;
        sel[1] = target;
        sel[0] = id;
        BattleLayout_HighlightPartyPanelsFar(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, 7);
        WaitFrames(2);

        sel[0] = id;
        BattleLayout_HighlightPartyPanelsFar(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, BattleMotion_GetSlotPalette(id));
        WaitFrames(2);
        remaining--;
    } while (remaining >= 0);

    UiWindow_DrawPartyStatusContentsFar(Ram_HeapSlots->battle_work->party_status_mode);
}
