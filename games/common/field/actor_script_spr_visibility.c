/* fdActorScriptHideSprs and fdActorScriptShowSpr, the same in both games.
   The Broken Seal's field/field_actor_motion.c and The Lost Age's
   field/field_actors.c include it where the functions stand. */

/* SC_HIDE_SPRITES: clears spriteStorageKind, hiding the sprites, and moves on
   to the next command. */
Sint32 fdActorScriptHideSprs(ACTOR *interpreter)
{
	interpreter->sprStorageKind = ACTOR_SPRITE_NONE;
	interpreter->scriptCursor++;
	return 1;
}

/* SC_SHOW_SPRITE: shows the actor's one sprite again (ACTOR_SPRITE_ONE) and
   moves on to the next command. */
Sint32 fdActorScriptShowSpr(ACTOR *interpreter)
{
	interpreter->sprStorageKind = ACTOR_SPRITE_ONE;
	interpreter->scriptCursor++;
	return 1;
}
