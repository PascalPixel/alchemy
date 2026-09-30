/* The facing action line. */
#include "TOREBI.H"
extern u8 MsgTorebiIfCanMakeNameFor[];
extern u8 MsgTorebiMaybeCloseShop[];
extern u8 MsgTorebiWasntAbleWatch[];

void SceneDialogue_RunFacingAction(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(27, no);
    } else {
        if (GameFlag_IsSet(0x950) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiWasntAbleWatch);
            Event_ShowMessage(no, 0);
        } else if (GameFlag_IsSet(0x962) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiMaybeCloseShop);
            Event_ShowMessage(no, 0);
        } else {
            Event_SetMessage((s32)MsgTorebiIfCanMakeNameFor);
            Event_ShowMessage(no, 0);
        }
    }
}
