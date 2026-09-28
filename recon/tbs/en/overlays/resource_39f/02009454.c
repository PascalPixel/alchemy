/* Draft of resource_39f 0x02009454 (FieldScene_RunProbedActorEightOrTenScene), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but its cue call reaches the veneer COMMON/OBJECT/STAGED_ACTOR.C names Audio_PlayCue, which FIELD_EVENT.H's inline of that name hides from this source; linking it would give the veneer a second name.
 * The listing keeps these rows. */
#include "MORI.H"

void Func_02001a82(struct Resource39fProbe);
u8 *Func_0200429c(s32);

void FieldScene_RunProbedActorEightOrTenScene(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Event_Begin();

    if (StagedActor_FindClearPosition(&probe) != 0) {
        Func_02001a82(probe);

        if (probe.word[1] == 8 && (probe.word[4] >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Map_CopyCellAttributes(35, 67, 4, 1, fifth, sixth);
        } else if (probe.word[1] == 10 && (probe.word[2] >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            GameFlag_Set(0x311);
            Actor_SetAnimation(10, 3);
            Actor_SetDestinationOffset(10, -16, 6);
            Event_Wait(30);
            Actor_SetAnimation(10, 8);
            Audio_PlayCue(240);

            Func_0200429c(10)[35] = 2;

            fifth = 34;
            sixth = 30;
            Map_CopyCellAttributes(44, 30, 2, 4, fifth, sixth);
            height = 4;
            StagedActor_FillGridAttributeRectangle(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Event_End();
}
