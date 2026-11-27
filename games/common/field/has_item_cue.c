/* fdHasItemCue, the same in both games. Both games'
   field/field_mode_psynergy.c include it where the function stands. */

/* Returns whether the scene has a script for item and what the leader faces. */
Uint32 fdHasItemCue(Uint16 item)
{
	if(fdFindItemCue(item))
		return 1;
	return 0;
}
