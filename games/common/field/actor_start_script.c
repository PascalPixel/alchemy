/* fdActorStartScript, the same in both games. Both games'
   field/field_actors.c include it where the function stands. */

/* Gives an actor a script and starts it from the top. Sets the script and
   clears the cursor, the pause, the loop count and the last flag test. */
void fdActorStartScript(ACTOR *actor, const Sint32 *script)
{
	if(actor != 0) {
		actor->scriptCursor = 0;
		actor->script = script;
		actor->paused = 0;
		actor->repeatCnt = 0;
		actor->conditionResult = 0;
	}
}
