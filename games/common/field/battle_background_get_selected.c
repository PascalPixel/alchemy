/* fdBattleBackgroundGetSelected, the same in both games. Both games'
   field/field_mode_encounter.c include it where the function stands. */

/* Returns the battle background chosen for the current scene. */
Sint16 fdBattleBackgroundGetSelected(void)
{
	return MainRoutineStateWork.battleBg;
}
