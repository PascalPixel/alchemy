/* Draft of resource_3c5 0x0200a5c8..0x0200a660 (152 bytes with pool),
 * FieldScene_RunFlag985DialogueBranch; the listing keeps the rows. The C
 * compiles to the reference's instructions, but its Audio_PlayCue is the
 * field event header's inline into Engine_AudioPlayCue, while this overlay's
 * audio import veneer is named Audio_PlayCue for the linked staged-actor code;
 * linking it would give that veneer a second name. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

void FieldScene_RunFlag985DialogueBranch(void)
{
    extern u8 *Data_03001ebc;

    u8 *base = Data_03001ebc;
    s16 *h;

    Event_Begin();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (GameFlag_IsSet(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Message_ShowCentered(MSG_ROBIN_FLIPPED_SWITCH, 1);
            Audio_PlayCue(155);
            Engine_MapCopyCellsTo(35, 78, 1, 2, k5, k6);
            Iriguchi_Wait(10);
            Engine_MapCopyCellsTo(34, 78, 1, 2, k5, k6);
            Iriguchi_Wait(10);
            FieldScene_RunScene3c5_020024d0();
        }
    } else {
        Event_SetMessage(MSG_TRUTH_DOOR_OPEN_THOSE_SEEING);
        Event_ShowMessage(-1, 0);
    }
    Event_End();
}
