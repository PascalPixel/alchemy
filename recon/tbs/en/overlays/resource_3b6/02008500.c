/* Draft of resource_3b6 0x02008500 (SceneDialogue_RunFacingPrompt), built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/TOREBI.H.
 * Remaining difference: the ROM loads the message number once from the
 * literal pool and forms the following lines by adding to it, as a
 * link-time message value would; the C constant folds each sum into its
 * own pool constant (4 or 8 bytes longer).
 * The listing keeps these rows. */
#include "TOREBI.H"

void SceneDialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Shop_Open(28, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        Scene_Call1(Engine_EventSetMessage, 0x238d);
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Scene_Call1(Engine_EventSetMessage, 0x221b);
        Event_ShowMessage(no, 0);
    } else {
        msg = MSG_FIRST_TIME_TO_TOLBI;
        Event_SetMessage(msg);
        Scene_Value2(Engine_EventOpenMessage, no, 0);
        if (Scene_Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    }
}
