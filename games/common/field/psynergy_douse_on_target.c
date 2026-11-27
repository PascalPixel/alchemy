/* fdPsynergyDouseOnTarget, the same in both games. The Broken Seal's
   field/script_commands_douse.c and The Lost Age's
   field/script_commands_cloak.c include it where the function stands. */

/* Runs Douse cast on a target, through fdPsynergyDouse. */
void fdPsynergyDouseOnTarget(Sint32 unused)
{
	fdPsynergyDouse();
}
