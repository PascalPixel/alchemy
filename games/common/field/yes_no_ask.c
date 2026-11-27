/* fdYesNoAsk, the same in both games. The Broken Seal's
   field/script_commands_script.c and The Lost Age's
   field/field_mode_script.c include it where the function stands. */

/* Asks yes or no at the top of the screen, or at the bottom with atBottom set.
   Returns 0 for yes, 1 for no. */
Sint32 fdYesNoAsk(Sint32 atBottom)
{
	return mnYesNoMenuRunStub(atBottom, 0, 0, 0);
}
