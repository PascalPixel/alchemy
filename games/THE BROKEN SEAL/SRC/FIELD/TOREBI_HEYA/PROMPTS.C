/* The facing prompts: a shop when the leader faces the counter, otherwise a
 * line chosen by the story flags. The yes-or-no lines load their first
 * message once and add to it for the two answers. */
#include "TOREBI.H"
extern u8 MsgTorebiColossoFinalsFinally[];
extern u8 MsgTorebiComeWayKalay[];
extern u8 MsgTorebiFirstTimeTolbi[];
extern u8 MsgTorebiLookStrongGo[];
extern u8 MsgTorebiRequireLotHealing[];
extern u8 MsgTorebiRightOneWell[];

void SceneDialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Shop_Open(28, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiRightOneWell);
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiRequireLotHealing);
        Event_ShowMessage(no, 0);
    } else {
        msg = (s32)MsgTorebiFirstTimeTolbi;
        Event_SetMessage(msg);
        Engine_EventOpenMessage(no, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    }
}

void SceneDialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(26, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        msg = (s32)MsgTorebiComeWayKalay;
        Event_SetMessage(msg);
        Engine_EventOpenMessage(no, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiColossoFinalsFinally);
        Event_ShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgTorebiLookStrongGo);
        Event_ShowMessage(no, 0);
        Engine_ActorShowEmote(no, 0x106, 0);
        Event_Wait(40);
        Event_ShowMessage(no, 0);
    }
}
