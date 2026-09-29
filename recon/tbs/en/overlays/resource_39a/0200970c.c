/* NONMATCHING: resource_39a at 0x0200970c..0x020097a8 (156 bytes with their
 * pools), between FIELD/COMMON/IMIRU_FUCHIN/LAYOUT.C and ARRIVAL.C, stay
 * listing: the two workspace-word helpers and the exported entry hook.
 *
 * Remaining differences: the workspace helpers read the flag word just past
 * the overlay's image and a resident work pointer at 0x03001ee0 that the main
 * image does not name yet; the entry hook compares the scene with 0x34 loaded
 * from its pool, as a link-time scene symbol does.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

void SceneState_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = Data_03001ee0;

        *(s32 *)(state + 24) = Func_02003a6e(0);
    }
}

void SceneState_ClearWorkWord24(void)
{
    if (Data_0200b328 != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
    }
}

s32 Func_02001750(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x109) == 0 && Data_02000240_t[224][0] == (s32)Data_00000034) {
        GameFlag_Set(0x144);
        FieldScene_RunScene39aSequenceA();
    } else {
        Func_02002f72_a();
    }
    return 0;
}
