/* Draft of resource_3ab 0x02008a90 (RightGuard_MindRead): it matches the ROM
 * byte for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgRunpaRightGuardReopenedThoughts,
 * MsgRunpaYoudShadowSneak). The listing keeps these rows until the draft is
 * adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"
extern u8 MsgRunpaYoudShadowSneak[];
extern u8 MsgRunpaRightGuardReopenedThoughts[];

void RightGuard_MindRead(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage((s32)MsgRunpaRightGuardReopenedThoughts);
    } else {
        Event_SetMessage((s32)MsgRunpaYoudShadowSneak);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}
