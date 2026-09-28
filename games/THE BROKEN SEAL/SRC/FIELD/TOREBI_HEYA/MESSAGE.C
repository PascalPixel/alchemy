/* The facing message. */
#include "TOREBI.H"

void SceneDialogue_RunFacingMessage(s32 no)
{
    s32 GameFlag_IsSet(s32 flag);

    u8 *actor = Engine_ActorGet(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Sanctum_Open(no);
    } else {
        if (GameFlag_IsSet(0x950) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x23bf);
            Event_ShowMessage(no, 0);
        } else if (GameFlag_IsSet(0x962) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x2231);
            Event_ShowMessage(no, 0);
        } else {
            Event_SetMessage(MSG_WASTE_STUCK_HERE_WHEN_SUCH);
            Event_ShowMessage(no, 0);
        }
    }
}
