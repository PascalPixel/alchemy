/* Draft of resource_3b6 0x02008500 (SceneDialogue_RunFacingPrompt): it
 * matches the ROM byte for byte now that the messages it loads from the
 * literal pool have catalogue names (MsgTorebiFirstTimeTolbi,
 * MsgTorebiRequireLotHealing, MsgTorebiRightOneWell). The listing keeps these
 * rows until the draft is adopted. */
#include "TOREBI.H"
extern u8 MsgTorebiFirstTimeTolbi[];
extern u8 MsgTorebiRequireLotHealing[];
extern u8 MsgTorebiRightOneWell[];

void SceneDialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Engine_ActorGet(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Shop_Open(28, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        Scene_Call1(Engine_EventSetMessage, (s32)MsgTorebiRightOneWell);
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Scene_Call1(Engine_EventSetMessage, (s32)MsgTorebiRequireLotHealing);
        Event_ShowMessage(no, 0);
    } else {
        msg = (s32)MsgTorebiFirstTimeTolbi;
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
