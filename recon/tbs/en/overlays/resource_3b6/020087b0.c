/* Draft of resource_3b6 0x020087b0 (FieldScene_RunScene3b6_020007b0), built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/TOREBI.H.
 * Remaining difference: the ROM loads the message number once from the
 * literal pool and forms the following lines by adding to it, as a
 * link-time message value would; the C constant folds each sum into its
 * own pool constant (4 or 8 bytes longer).
 * The listing keeps these rows. */
#include "TOREBI.H"

void FieldScene_RunScene3b6_020007b0(s32 a0)
{
    void Event_ShowMessage();

    u32 i;
    s32 record;
    s32 base5_2399;

    Event_Begin();
    if (GameFlag_IsSet(0x8bd) == 0) {
        base5_2399 = MSG_DO_I_LOOK_EXCITED;
        Event_SetMessage(base5_2399);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage((base5_2399 + 1));
        } else {
            Event_SetMessage((base5_2399 + 2));
        }
        Event_ShowMessage(a0, 0);
    } else {
        if (GameFlag_IsSet(0x8be) == 0) {
            GameFlag_Set(0x8be);
            Event_SetMessage(MSG_OH_HELLO);
            Event_ShowMessage(a0, 0);
            Event_Wait(10);
            Actor_RunRepeatedMotion(a0, 2);
            Event_Wait(20);
        }
        Event_SetMessage(MSG_HO_HUM_IM_FINE_ITS);
        Event_ShowMessage(a0, 0);
    }
    Event_End();
}
