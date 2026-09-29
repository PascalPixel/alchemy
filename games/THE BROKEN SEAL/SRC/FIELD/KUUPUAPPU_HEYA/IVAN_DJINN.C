#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
u8 *Owner_GetState(s32 owner);
void Djinn_Transfer(s32 owner, s32 djinn, s32 element, s32 flags);
void Owner_RecalculateStats(s32 owner);

/* After twenty placement updates, if bit 0 of party member 2's word at
   +0xf8 is set, his djinni are transferred, cue 126 plays and members 0
   and 2 have their stats recalculated. It is declared to return a value,
   and returns none. */
s32 OverlayObject_RunObjectTwoWhenFlagged(void)
{
    u8 *state;

    BattlePlacement_UpdateTimedEntriesTwentyTimes();
    state = Owner_GetState(2);
    if (*(s32 *)(state + 0xf8) & 1) {
        Djinn_Transfer(2, 0, 0, 0);
        Audio_PlayCue(126);
        Owner_RecalculateStats(0);
        Owner_RecalculateStats(2);
    }
}
