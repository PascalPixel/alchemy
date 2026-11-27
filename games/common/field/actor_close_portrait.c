/* fdActorClosePortrait, the same in both games. Both games'
   field/script_commands_messages.c include it where the function stands. */

/* Closes the portrait of an actor's sprite, if it has one. */
void fdActorClosePortrait(Sint32 actorId)
{
	Sint32 spr;

	spr = fdActorGetSprById(actorId);
	if(spr != -1)
		mnPortraitCloseWindowStub(spr);
}
