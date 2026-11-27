/* Element levels and classes. Part of game.c. */

/* Element levels are counted in tenths: a Djinni set adds a whole level. */
#define ELEMENT_LEVEL_SCALE 10
/* The highest whole element level, the last row of
   ElementPowerResistByLevel. */
#define ELEMENT_LEVEL_MAX	15

/* Returns the class series of a character whose strongest element is
   `primaryElement` and second `secondaryElement`, from the ELEMENT_COUNT by
   ELEMENT_COUNT table. */
Sint32 gmClassGetSeries(Sint32 primaryElement, Sint32 secondaryElement)
{
	return ClassSeriesByElements[(primaryElement * ELEMENT_COUNT) +
		secondaryElement];
}

/* Writes each element level in tenths (ELEMENT_LEVEL_SCALE): an enemy's from
   its preset, a character's from the Djinn set on it plus its own bonus.
   `djinnHeld` is the unit's Djinn (UNIT.djinnHeld). */
Sint32 gmUnitGetElementLevels(
	Sint32 unitId, const Uint8 *djinnHeld, Sint32 output[4])
{
	Sint32	i;
	Sint32 *cursor;

	if(unitId > PIERS) {
		Uint32 index;

		index = gmEnemyGetDefinition(unitId)->elementPreset;
		if(index >= ENEMY_ELEMENT_PRESET_COUNT)
			index = 0;

		i = 0;
		cursor = output;
		for(; i < ELEMENT_COUNT; i++)
			*cursor++ = EnemyElementPresetTable[index].elementLevels[i] *
				ELEMENT_LEVEL_SCALE;
	} else {
		cursor = output;
		/* The counts of set Djinn by element follow the Djinn bits in the
		   unit. */
		djinnHeld += offsetof(UNIT, djinnSetCounts) - offsetof(UNIT, djinnHeld);
		for(i = 0; i < ELEMENT_COUNT; i++) {
			Uint32 count = *djinnHeld++;

			*cursor++ = count * ELEMENT_LEVEL_SCALE;
		}

		if(unitId <= PIERS) {
			for(i = 0; i < ELEMENT_COUNT; i++) {
				*output += gmCharacterGetDefinition(unitId)->elementBonus[i];
				output++;
			}
		}
	}

	return 0;
}

/* Returns how strongly a unit is attuned to `element`, its whole element level
   (a tenth of what gmUnitGetElementLevels counts), and 0 for `ELEMENT_NONE`. */
Sint32 gmUnitGetElementLevel(Sint32 unitId, Sint32 element)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 levels[ELEMENT_COUNT];
	Sint32 result = 0;

	if(element < ELEMENT_COUNT) {
		gmUnitGetElementLevels(unit->id, (Uint8 *)unit->djinnHeld, levels);
		result = levels[element] / ELEMENT_LEVEL_SCALE;
	}
	return result;
}

/* Returns the element an enemy attacks with, from its element preset. */
Sint32 gmEnemyGetAttackElement(UNIT *unit)
{
	const ENEMY_DEFINITION *enemy = gmEnemyGetDefinition(unit->id);
	Uint8					presetIndex = enemy->elementPreset;

	if(presetIndex >= ENEMY_ELEMENT_PRESET_COUNT)
		presetIndex = 0;
	return EnemyElementPresetTable[presetIndex].attackElement;
}

/* Writes a unit's base Power and Resist in each element, from its element
   levels. */
UNREAD_RESULT
gmUnitCalculateBaseElements(Sint32 unitId, BATTLE_ELEMENT_STATS *dest)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 levels[ELEMENT_COUNT];
	Sint32 i;

	if(unit->classId == CLASS_NONE) {
		Uint32 preset = gmEnemyGetDefinition(unit->id)->elementPreset;

		if(preset >= ENEMY_ELEMENT_PRESET_COUNT)
			preset = 0;
		/* The copy returns from inside its loop, as the last element is
		   written. */
		for(i = 0;;) {
			dest[i] = EnemyElementPresetTable[preset].stats[i];
			if(++i >= ELEMENT_COUNT)
				RETURN_UNREAD(preset);
		}
	}

	/* Element levels in tenths: the whole level picks the table's row, the
	   tenths add to it. */
	gmUnitGetElementLevels(unit->id, (Uint8 *)unit->djinnHeld, levels);
	for(i = 0; i < ELEMENT_COUNT; i++) {
		Sint32 tenths = levels[i] % ELEMENT_LEVEL_SCALE;
		Sint32 level = levels[i] / ELEMENT_LEVEL_SCALE;

		if(level > ELEMENT_LEVEL_MAX)
			level = ELEMENT_LEVEL_MAX;
		if(level < 0)
			level = 0;
		/* The pair is written as two halfwords in a row, power then
		   resistance. */
		((Sint16 *)dest)[i * 2] =
			ElementPowerResistByLevel[level].power + tenths;
		((Sint16 *)dest)[i * 2 + 1] =
			ElementPowerResistByLevel[level].resistance + tenths;
	}
}

/* Returns the class a character has with these Djinn (UNIT.djinnHeld): the
   series of its two strongest elements, and the last class of it whose
   requirements it meets. */
Sint32 gmClassDetermine(Sint32 character, const Uint8 *djinn)
{
	Sint32 levels[ELEMENT_COUNT];
	Sint32 primary;
	Sint32 best;
	Sint32 series;
	Sint32 result;
	Sint32 j;
	Sint32 i;

	result = -1;
	if(character > PIERS)
		return CLASS_NONE;
	gmUnitGetElementLevels(character, djinn, levels);
	if(gmFlagGet(FLAG_CHILDHOOD)) {
		if(character == ISAAC)
			return CLASS_ISAAC_CHILD;
		if(character == GARET)
			return CLASS_GARET_CHILD;
	}
	if(character == JENNA)
		return CLASS_FLAME_USER;
	if(result != -1)
		return result;
	best = result;
	primary = result;
	for(i = 0; i < ELEMENT_COUNT; i++) {
		if(best < levels[i]) {
			best = levels[i];
			primary = i;
		}
	}
	j = -1;
	best = -1;
	for(i = 0; i < ELEMENT_COUNT; i++) {
		if(i != primary && best < levels[i]) {
			best = levels[i];
			j = i;
		}
	}
	/* A second element below level 1 does not count, and ties go to the lower
	   element. */
	series = gmClassGetSeries(
		primary, levels[j] >= ELEMENT_LEVEL_SCALE ? j : primary);
	/* The last class of the series that the element levels reach. */
	for(i = CLASS_COUNT - 1; i >= 0; i--) {
		if(ClassDefinitionTable[i].series != series)
			continue;
		for(j = 0; j < ELEMENT_COUNT; j++) {
			if(levels[j] <
				ClassDefinitionTable[i].required[j] * ELEMENT_LEVEL_SCALE)
				break;
		}
		if(j == ELEMENT_COUNT) {
			result = i;
			break;
		}
	}
	if(result == -1)
		result = CLASS_NONE;
	return result;
}

/* Returns class `classId`'s definition: its series, element requirements, stat
   multipliers and the Psynergy it teaches by level. */
CLASS_DEFINITION *gmClassGetDefinition(Sint32 classId)
{
	return (CLASS_DEFINITION *)&ClassDefinitionTable[classId];
}

/* Works out a character's class again after its Djinn change, then its Psynergy
   list and its base elements. */
void gmUnitUpdateClass(Sint32 unitId)
{
	UNIT *unit = gmUnitGet(unitId);

	unit->classId = gmClassDetermine(unit->id, (Uint8 *)unit->djinnHeld);
	gmUnitRefreshPsynergy(unitId);
	gmUnitCalculateBaseElements(unitId, unit->baseElement);
}

#undef ELEMENT_LEVEL_SCALE
#undef ELEMENT_LEVEL_MAX
