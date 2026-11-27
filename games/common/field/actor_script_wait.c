/* fdActorScriptWait, the same in both games. Both games'
   field/field_script_commands.c include it where the function stands. */

/* SC_WAIT(n): waits n ticks; this tick counts as the first. */
Sint32 fdActorScriptWait(ACTOR *interpreter)
{
	Sint16 commandCursor = interpreter->scriptCursor;

	interpreter->wait = interpreter->script[commandCursor + 1] - 1;
	interpreter->scriptCursor += 2;
	return 0;
}
