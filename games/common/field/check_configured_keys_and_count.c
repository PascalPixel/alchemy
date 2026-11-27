/* fdCheckConfiguredKeysAndCount, the same in both games. The Broken Seal's
   field/field_player_control.c and The Lost Age's field/field_pad_movement.c
   include it where the function stands. */

/* SC_KEY_COUNT: checks the configured keys (fdCheckConfiguredKeys) and steps
   the script on. Returns 1. */
Sint32 fdCheckConfiguredKeysAndCount(ACTOR *actor)
{

	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}
