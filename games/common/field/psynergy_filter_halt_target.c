/* fdPsynergyFilterHaltTarget, the same in both games. The Broken Seal's
   field/script_commands_halt.c and The Lost Age's
   field/field_psynergy_radial_effects.c include it where the function
   stands. */

/* Returns no target (-1) when Halt passes actor id over (its sprite has no
   halted lines, SPEAKER_HALTED_LINES_NONE), else id. */
Sint32 fdPsynergyFilterHaltTarget(Sint32 id)
{
	if(fdSpeakerGetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
		return -1;
	return id;
}
