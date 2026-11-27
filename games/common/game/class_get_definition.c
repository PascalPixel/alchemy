/* gmClassGetDefinition, the same in both games. Both games'
   game/game_classes.c include it where the function stands. */

/* Returns class `classId`'s definition: its series, element requirements, stat
   multipliers and the Psynergy it teaches by level. */
CLASS_DEFINITION *gmClassGetDefinition(Sint32 classId)
{
	return (CLASS_DEFINITION *)&ClassDefinitionTable[classId];
}
