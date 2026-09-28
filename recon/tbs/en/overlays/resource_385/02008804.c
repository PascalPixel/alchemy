/*
 * Draft: overlay 385 (KUUPUAPPU_MURA_SAI) at 0x02008804, between
 * DIALOGUE.C and SCENE_STEP.C; its rows stay in the listing.
 *
 * Remaining differences: the game loads the flag bit 2 as a pool word
 * before the halfword or, and loads message 0x1cc0 from the pool. A
 * constant bit compiles to a halfword pool load or a move, and a constant
 * message to mov #115 / lsl #6; only relocated symbols reproduce either.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"

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
        Event_SetMessage(0x1cc0);
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
