/* Vale, its home, storm and Babi scenes, Sol Sanctum's eruption and Venus
   Lighthouse's aerie: Psynergy shown in a script. One is the villager's in
   Vale: "Behold, the power of Psynergy!" */

#include "ability_ids.h"
#include "field/field_mode.h"

/* Prepares the effect state and spark tiles of Move (ABILITY_MOVE) for the
   script's Psynergy. */
void scStartScriptPsynergy(void)
{
	fdPsynergyLoadResources(ABILITY_MOVE, 0);
}

/* Ends the script's Psynergy and releases its effect state. */
void scEndScriptPsynergy(void)
{
	fdPsynergyEnd();
}
