/* fdActorScriptInvokeCallback and fdActorScriptFindLabel, the same in both
   games. Both games' field/field_script_commands.c include it where the
   functions stand. */

/* SC_CALL(fn): calls fn with the actor. */
Bool fdActorScriptInvokeCallback(ACTOR *interpreter)
{
	Sint16		   initCursor = interpreter->scriptCursor;
	SCRIPT_COMMAND func = (SCRIPT_COMMAND)interpreter->script[initCursor + 1];

	if(func(interpreter) != 0)
		return FALSE;
	if(interpreter->scriptCursor == initCursor)
		interpreter->scriptCursor = interpreter->scriptCursor + 2;
	return TRUE;
}

/* Returns the cursor just past the script's label key (SC_LABEL or SC_TO,
   matched without the bit that tells them apart), searching its first
   SCRIPT_LABEL_SEARCH_WORDS words, or 0 (the start) when key is 0 or not found,
   so a missing label restarts the script. Clears the wait. */
Sint32 fdActorScriptFindLabel(ACTOR *interpreter, Uint32 labelKey)
{
	Uint32 *scriptWords;
	Sint32	wordIdx;

	interpreter->wait = 0;
	if(labelKey == 0) {
		return 0;
	}

	labelKey &= SCRIPT_LABEL_KEY_MASK;
	scriptWords = (Uint32 *)interpreter->script;
	for(wordIdx = 0; wordIdx < SCRIPT_LABEL_SEARCH_WORDS; wordIdx++) {
		if(*scriptWords++ == labelKey) {
			return wordIdx + 1;
		}
	}
	return 0;
}
