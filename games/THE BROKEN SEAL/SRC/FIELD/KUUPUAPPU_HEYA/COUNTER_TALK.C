#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuMasterHammetIsntOnlyOne[];
extern u8 MsgKuupuappuWeFoundOurStolenWeapons[];

/* Actor 19 serves the shop when the leader faces it across the counter and
 * talks otherwise. */
void FieldScene_RunActorNineteenAngleDialogue(void)
{
    s32 v = *(u16 *)((u8 *)Engine_ActorGet(0) + 6);

    Event_Begin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Shop_Open(4, 19);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Event_SetMessage((s32)MsgKuupuappuMasterHammetIsntOnlyOne);
        } else {
            Event_SetMessage((s32)MsgKuupuappuWeFoundOurStolenWeapons);
        }
        Event_ShowMessage(19, 0);
    }
    Event_End();
}
