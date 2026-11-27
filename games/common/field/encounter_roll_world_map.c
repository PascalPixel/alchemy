/* fdEncounterRollWorldMap, the same in both games. Both games'
   field/field_mode_encounter.c include it where the function stands. */

/* Rolls a random battle on the World Map, taking the zone from the metatile at
   pos. Returns the formation to fight, or 0 for no battle. */
Sint32 fdEncounterRollWorldMap(Sint32 pos, FIXED steps)
{
	return fdEncounterCountStepsAndSelectFormation(
		fdEncounterResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}
