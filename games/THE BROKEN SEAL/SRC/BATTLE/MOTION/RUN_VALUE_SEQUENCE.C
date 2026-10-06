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

#include "MOTION_OBJECT.H"

#include "BATTLE_WORK.H"

#include "BATTLE_RUNTIME.H"

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
