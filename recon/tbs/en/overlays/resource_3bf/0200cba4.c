/* Draft of resource_3bf 0x0200cba4 (ConfigureSceneActor13): it matches the
 * ROM byte for byte now that the message it loads from the literal pool has a
 * catalogue name (MsgRunpaRightRightGive). The listing keeps these rows until
 * the draft is adopted. */
#include "FORTRESS.H"
extern u8 MsgRunpaRightRightGive[];

void ConfigureSceneActor13(void)
{
    Actor_RunRepeatedMotion(13, 2);
    Event_SetMessage((s32)MsgRunpaRightRightGive);
    Event_ShowMessage(13, 0);
}
