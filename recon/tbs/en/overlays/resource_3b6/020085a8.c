/* Draft of resource_3b6 0x020085a8 (SceneDialogue_RunFacingActionPrompt): it
 * matches the ROM byte for byte now that the messages it loads from the
 * literal pool have catalogue names (MsgTorebiColossoFinalsFinally,
 * MsgTorebiComeWayKalay, MsgTorebiLookStrongGo). The listing keeps these rows
 * until the draft is adopted. */
#include "TOREBI.H"
extern u8 MsgTorebiColossoFinalsFinally[];
extern u8 MsgTorebiComeWayKalay[];
extern u8 MsgTorebiLookStrongGo[];

void SceneDialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(26, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        msg = (s32)MsgTorebiComeWayKalay;
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
        Scene_Call1(Engine_EventSetMessage, (s32)MsgTorebiColossoFinalsFinally);
        Event_ShowMessage(no, 0);
    } else {
        Scene_Call1(Engine_EventSetMessage, (s32)MsgTorebiLookStrongGo);
        Event_ShowMessage(no, 0);
        Scene_Call3(Engine_ActorShowEmote, no, 0x106, 0);
        Event_Wait(40);
        Event_ShowMessage(no, 0);
    }
}
