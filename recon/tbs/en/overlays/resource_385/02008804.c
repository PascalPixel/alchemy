/*
 * Draft: overlay 385 (KUUPUAPPU_MURA_SAI) at 0x02008804, between
 * DIALOGUE.C and SCENE_STEP.C; its rows stay in the listing.
 *
 * Remaining difference: the game loads the flag bit 2 as a pool word before
 * the halfword or; a constant bit compiles to a halfword pool load or a move
 * (2 halfwords differ). The message it loads from the pool is
 * MsgKuupuappuWatchingGuysMakes, which the draft now names.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgKuupuappuWatchingGuysMakes[];

void ActorPresentation_RunActorModeOneThenZero(s32 actor);
void SceneDialogue_RunActorFourteenFlagDialogue(void);

void FieldScene_RunSupplementalSequenceOne(void)
{
    {
        u16 *flags = (u16 *)((u8 *)Actor_Get(14) + 100);

        *flags |= 2;
    }
    Event_Begin();
    if (GameFlag_IsSet(0x307) != 0) {
        Event_SetMessage((s32)MsgKuupuappuWatchingGuysMakes);
        ActorPresentation_RunActorModeOneThenZero(14);
    } else {
        SceneDialogue_RunActorFourteenFlagDialogue();
        GameFlag_Set(0x307);
    }
    Event_End();
    {
        u8 *record = (u8 *)Actor_Get(14);
        s32 shown = 1;

        *(volatile u16 *)((s32)record + 100) = shown;
    }
}
