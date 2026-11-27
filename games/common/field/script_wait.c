/* fdScriptWait, the same in both games. The Broken Seal's
   field/script_commands_script.c and The Lost Age's
   field/field_mode_script.c include it where the function stands. */

/* Waits the requested ticks through sySynch(), unless debug fast-forward
   (fdScriptUpdateDebugSkip) is on: the wait scripts use. */
void fdScriptWait(Sint32 ticks)
{
	if(FieldStateWork->fastForward == 0 && ticks != 0) {
		sySynch(ticks);
	}
}
