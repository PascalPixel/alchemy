/* fdPsynergyCatchOnTarget, the same in both games. The Broken Seal's
   field/script_commands_catch.c and The Lost Age's
   field/field_psynergy_radial_effects.c include it where the function
   stands. */

/* Catch cast on a target: runs fdPsynergyCatch. */
void fdPsynergyCatchOnTarget(Sint32 unused)
{
	fdPsynergyCatch();
}
