/* Draft of resource_38d 0x02008130 (SceneDialogue_RunActor10Message1420),
 * built with games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/KYUDEN.H.
 * Remaining difference: the ROM loads message 0x1420 from the literal pool,
 * as a link-time message value would; the C constant is built with a move
 * and a shift. The listing keeps these rows. */
#include "KYUDEN.H"

extern u8 LinkedMessage_YouWillingGoKolimaForest;

void SceneDialogue_RunActor10Message1420(void)
{
    Event_Begin();
    Event_SetMessage((s32)&LinkedMessage_YouWillingGoKolimaForest);
    Event_AskYesNo(10, 0);
    Event_End();
}
