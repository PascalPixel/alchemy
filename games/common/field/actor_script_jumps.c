/* fdActorScriptJumpToLabel, fdActorScriptJumpIfTrue and
   fdActorScriptJumpIfFalse, the same in both games. Both games'
   field/field_script_commands.c include it where the functions stand. */

/* SC_JUMP(to): jumps to label to. */
Sint32 fdActorScriptJumpToLabel(ACTOR *interpreter)
{
	interpreter->scriptCursor = fdActorScriptFindLabel(
		interpreter, interpreter->script[interpreter->scriptCursor + 1]);
	return 1;
}

/* SC_IF(to): jumps to label to when the last test was true. */
Sint32 fdActorScriptJumpIfTrue(ACTOR *interpreter)
{
	Sint32		  commandCursor = interpreter->scriptCursor;
	const Sint32 *script = interpreter->script;
	Uint32		  labelKey = script[commandCursor + 1];

	if(interpreter->conditionResult != 0) {
		interpreter->scriptCursor =
			fdActorScriptFindLabel(interpreter, labelKey);
	} else {
		interpreter->scriptCursor += 2;
	}
	return 1;
}

/* SC_IFNOT(to): jumps to label to when the last test was false. */
Sint32 fdActorScriptJumpIfFalse(ACTOR *interpreter)
{
	Sint32		  commandCursor = interpreter->scriptCursor;
	const Sint32 *script = interpreter->script;
	Uint32		  labelKey = script[commandCursor + 1];

	if(interpreter->conditionResult == 0) {
		interpreter->scriptCursor =
			fdActorScriptFindLabel(interpreter, labelKey);
	} else {
		interpreter->scriptCursor += 2;
	}
	return 1;
}
