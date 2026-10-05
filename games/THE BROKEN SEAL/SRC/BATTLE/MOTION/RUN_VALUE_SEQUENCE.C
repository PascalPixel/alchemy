#include "PROTO.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIELD_EVENT.H"

#include "MOTION_OBJECT.H"

#include "BATTLE_WORK.H"

#include "BATTLE_RUNTIME.H"
void BattleLayout_HighlightPartyPanelsFar(u16 *selection);
void BattleMotion_SetRecordChildValues(void *object, s32 value);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);

void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    Owner_GetStateFar(id);
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

    UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode);
}
