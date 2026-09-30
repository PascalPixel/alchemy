/* The facing message. */
#include "TOREBI.H"
extern u8 MsgTorebiMissFinalsTolbis[];
extern u8 MsgTorebiShipsArentGoing[];
extern u8 MsgTorebiWasteStuckHereWhenSuch[];

void SceneDialogue_RunFacingMessage(s32 no)
{
    s32 GameFlag_IsSet(s32 flag);

    u8 *actor = Engine_ActorGet(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Sanctum_Open(no);
    } else {
        if (GameFlag_IsSet(0x950) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiShipsArentGoing);
            Event_ShowMessage(no, 0);
        } else if (GameFlag_IsSet(0x962) != 0) {
            Engine_EventSetMessage((s32)MsgTorebiMissFinalsTolbis);
            Event_ShowMessage(no, 0);
        } else {
            Event_SetMessage((s32)MsgTorebiWasteStuckHereWhenSuch);
            Event_ShowMessage(no, 0);
        }
    }
}
