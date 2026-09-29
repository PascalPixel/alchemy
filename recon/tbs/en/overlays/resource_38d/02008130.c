/* Draft of resource_38d 0x02008130 (SceneDialogue_RunActor10Message1420): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgBiribinoYouWillingGoKolimaForest).
 * The listing keeps these rows until the draft is adopted. */
#include "KYUDEN.H"
extern u8 MsgBiribinoYouWillingGoKolimaForest[];


void SceneDialogue_RunActor10Message1420(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoYouWillingGoKolimaForest);
    Event_AskYesNo(10, 0);
    Event_End();
}
