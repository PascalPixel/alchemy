/* Draft of resource_3b6 0x020085a8 (SceneDialogue_RunFacingActionPrompt), built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/TOREBI.H.
 * Remaining difference: the ROM loads the message number once from the
 * literal pool and forms the following lines by adding to it, as a
 * link-time message value would; the C constant folds each sum into its
 * own pool constant (4 or 8 bytes longer).
 * The listing keeps these rows. */
#include "TOREBI.H"

void SceneDialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(26, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        msg = MSG_ALL_THE_WAY_FROM_KALAY;
        Event_SetMessage(msg);
        Scene_Value2(Engine_EventOpenMessage, no, 0);
        if (Scene_Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Scene_Call1(Engine_EventSetMessage, 0x2219);
        Event_ShowMessage(no, 0);
    } else {
        Scene_Call1(Engine_EventSetMessage, 0x1fd2);
        Event_ShowMessage(no, 0);
        Scene_Call3(Engine_ActorShowEmote, no, 0x106, 0);
        Event_Wait(40);
        Event_ShowMessage(no, 0);
    }
}
