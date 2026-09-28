/* The facing action line. */
#include "TOREBI.H"

void SceneDialogue_RunFacingAction(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(27, no);
    } else {
        if (GameFlag_IsSet(0x950) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x238f);
            Event_ShowMessage(no, 0);
        } else if (GameFlag_IsSet(0x962) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x221d);
            Event_ShowMessage(no, 0);
        } else {
            Event_SetMessage(MSG_IF_CAN_MAKE_NAME_FOR);
            Event_ShowMessage(no, 0);
        }
    }
}
