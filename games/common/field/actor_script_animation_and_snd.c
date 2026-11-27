/* fdActorScriptSetAnimation to fdActorScriptPlaySnd, the same in both games.
   Both games' field/field_script_commands.c include it where the functions
   stand. */

/* SC_ANIMATION(v): switches the actor's sprites to animation v. */
Sint32 fdActorScriptSetAnimation(ACTOR *interpreter)
{
	fdActorSelectAnimation(
		interpreter, interpreter->script[interpreter->scriptCursor + 1]);
	interpreter->scriptCursor += 2;
	return 1;
}

/* SC_RELEASE: releases the actor through fdActorRelease, which releases its
   sprites and zeroes its slot. */
Sint32 fdActorScriptRelease(ACTOR *interpreter)
{
	fdActorRelease(interpreter);
	return 0;
}

/* SC_NOP(v): does nothing with its operand. */
Sint32 fdActorScriptNoOperation(ACTOR *interpreter)
{
	interpreter->scriptCursor += 2;
	return 1;
}

/* SC_SE(n): sends sound command n. */
Sint32 fdActorScriptPlaySnd(ACTOR *interpreter)
{
	sySndPlayStub(interpreter->script[interpreter->scriptCursor + 1]);
	interpreter->scriptCursor += 2;
	return 1;
}
