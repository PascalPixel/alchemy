/* fdActorSystemResumeProjectionAndHideLayers and fdActorReturnTrue, the same
   in both games. Both games' field/field_actors.c include it where the
   functions stand. */

/* Resumes the actor camera updates and hides BG1-3 and the sprites. */
void fdActorSystemResumeProjectionAndHideLayers(void)
{
	syResumeEvent(fdActorSystemPlaceSprs);
	syResumeEvent(fdActorSystemProjectSprs);
	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/* Unused: returns 1. */
UNUSED Sint32 fdActorReturnTrue(void)
{
	return 1;
}
