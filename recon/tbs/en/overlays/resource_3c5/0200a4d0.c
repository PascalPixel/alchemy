/* Draft of resource_3c5 0x0200a4d0..0x0200a548 (120 bytes with pool),
 * FieldScene_RunScene3c5_020024d0; the listing keeps the rows. The C
 * compiles to the reference's instructions, but its Audio_PlayCue is the
 * field event header's inline into Engine_AudioPlayCue, while this overlay's
 * audio import veneer is named Audio_PlayCue for the linked staged-actor code;
 * linking it would give that veneer a second name. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

void FieldScene_RunScene3c5_020024d0(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x985) == 0) {
        GameFlag_Set(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x118, 240);
        Actor_SetDestination(9, 0x148, 240);
        Iriguchi_WaitForMove(8);
        Iriguchi_WaitForMove(9);
        Iriguchi_CopyCellAttributes(81, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x989) == 0) {
            FieldScene_RunActorEventSequence();
        }
    }
}
