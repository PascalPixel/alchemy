/* Draft of resource_374 0x020085e8 (SceneDialogue_ShowLineEB1OrEB0), built
 * with games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/HAIDIA.H. Remaining
 * difference: the ROM loads message 0xeb0 from the literal pool, as a
 * link-time message value would; the C constant 0xeb0 is built with
 * movs/lsls (4 bytes shorter). The listing keeps these rows. */
#include "HAIDIA.H"

void SceneDialogue_ShowLineEB1OrEB0(void)
{
    Event_Begin();
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(0x840) != 0) {
        Event_SetMessage(MSG_BE_SURE_TO_HELP_GARCIA);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage(MSG_JASMINE_WENT_OFF_THAT_WAY);
        Event_ShowMessage(16, 0);
    }
    Event_End();
}
