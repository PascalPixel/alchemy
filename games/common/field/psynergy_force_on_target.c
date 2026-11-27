/* fdPsynergyForceOnTarget, the same in both games. The Broken Seal's
   field/script_commands_force.c and The Lost Age's
   field/script_commands_mind_read.c include it where the function stands. */

/* Runs Force cast on a target, through fdPsynergyForce. */
void fdPsynergyForceOnTarget(Sint32 unused)
{
	fdPsynergyForce();
}
