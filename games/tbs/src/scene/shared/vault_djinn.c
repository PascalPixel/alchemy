/* Vault and its rooms: Ivan hands back Isaac's Djinni before he leaves the
   party. "Oh, wait, I don't want to take your Djinni." */

#include "djinn_ids.h"

/* Brings every Djinni back from recovery (twenty turns of it), then, if Ivan
   holds DJINNI_FLINT, gives it back to Isaac and recalculates both. */
static UNREAD_RESULT scVaultReturnIsaacsDjinni(void)
{
	btDjinnRecoveryCountDownTwenty();
	if((gmUnitGet(IVAN)->djinnHeld[ELEMENT_VENUS] &
		   (1 << DJINNI_INDEX(DJINNI_FLINT))) != 0) {
		gmDjinnTransfer(IVAN, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT), ISAAC);
		sySndPlay(SE_RECOVERY);
		gmUnitCalc(ISAAC);
		gmUnitCalc(IVAN);
	}
}
