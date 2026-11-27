/* Element levels and classes. Part of game.c. */

/* Element levels are counted in tenths: a Djinni set adds a whole level. */
#define ELEMENT_LEVEL_SCALE 10
/* The highest whole element level, the last row of
   ElementPowerResistByLevel. */
#define ELEMENT_LEVEL_MAX	15

/* Writes each element level in tenths (ELEMENT_LEVEL_SCALE) for the character
   or enemy `enemyId`: an enemy's from its preset, a character's from the Djinn
   set on it plus its own bonus. `src` points at the unit's Djinn bits
   (djinnHeld); the counts of set Djinn by element follow them in the unit. */
Sint32 gmUnitGetElementLevels(
	Sint32 enemyId, const Uint8 *src, Sint32 output[ELEMENT_COUNT])
{
	if(enemyId > PIERS) {
		const ENEMY_ELEMENT_PRESET *tbl;
		const ENEMY_ELEMENT_PRESET *preset;
		Uint32						idx;
		Sint32						i;

		idx = gmEnemyGetDefinition(enemyId)->elementPreset;
		if(idx >= ENEMY_ELEMENT_PRESET_COUNT)
			idx = 0;
		tbl = EnemyElementPresetTable;
		preset = &tbl[idx];
		for(i = 0; i < ELEMENT_COUNT; i++)
			output[i] = preset->elementLevels[i] * ELEMENT_LEVEL_SCALE;
	} else {
		Sint32 i;

		for(i = 0; i < ELEMENT_COUNT; i++)
			output[i] = src[offsetof(UNIT, djinnSetCnts) -
							offsetof(UNIT, djinnHeld) + i] *
				ELEMENT_LEVEL_SCALE;
		if(enemyId <= PIERS) {
			const CHARACTER_DEFINITION *definition =
				gmCharacterGetDefinition(enemyId);

			for(i = 0; i < ELEMENT_COUNT; i++)
				output[i] += definition->elementBonus[i];
		}
	}
	return 0;
}

/* Returns how strongly a unit is attuned to `element`, its whole element level
   (a tenth of what gmUnitGetElementLevels counts), and 0 for `ELEMENT_NONE`. */
Sint32 gmUnitGetElementLevel(Sint32 unitId, Sint32 element)
{
	UNIT  *unit = syUnitGet(unitId);
	Sint32 levels[ELEMENT_COUNT];
	Sint32 result = 0;

	if(element < ELEMENT_COUNT) {
		gmUnitGetElementLevels(
			unit->enemyId, (const Uint8 *)unit->djinnHeld, levels);
		result = levels[element] / ELEMENT_LEVEL_SCALE;
	}
	return result;
}

/* Returns the element an enemy attacks with, from its element preset; an id
   past the table's end reads the first preset. */
Sint32 gmEnemyGetAttackElement(UNIT *unit)
{
	Uint8 preset = gmEnemyGetDefinition(unit->enemyId)->elementPreset;

	if(preset >= ENEMY_ELEMENT_PRESET_COUNT)
		preset = 0;
	return EnemyElementPresetTable[preset].attackElement;
}

/* Writes a unit's base power and resistance in each element: an enemy's from
   its preset, a character's from its element levels. */
void gmUnitCalcBaseElements(Sint32 unitId, BATTLE_ELEMENT_STATS *dst)
{
	UNIT  *unit = syUnitGet(unitId);
	Uint32 idx;
	Sint32 levels[ELEMENT_COUNT];
	Sint32 i;
	extern const ELEMENT_LEVEL_BONUS
		ElementPowerResistByLevel[ELEMENT_LEVEL_MAX + 1];

	if(unit->classId == CLASS_NONE) {
		idx = gmEnemyGetDefinition(unit->enemyId)->elementPreset;
		if(idx >= ENEMY_ELEMENT_PRESET_COUNT)
			idx = 0;

		/* The copy returns from inside its loop, as the last element is
		   written. */
		i = 0;
		for(;;) {
			dst[i] = EnemyElementPresetTable[idx].stats[i];
			i++;
			if(i >= ELEMENT_COUNT)
				return;
		}
	}

	/* Element levels in tenths: the whole level picks the table's row, the
	   tenths add to it. */
	gmUnitGetElementLevels(
		unit->enemyId, (const Uint8 *)unit->djinnHeld, levels);
	for(i = 0; i < ELEMENT_COUNT; i++) {
		Sint32 tenths;
		Sint32 level;

		/* levels[i] / ELEMENT_LEVEL_SCALE, as a multiply by the reciprocal. */
		level = syMathUnsignedMulHigh(
			levels[i], RECIPROCAL_OF(ELEMENT_LEVEL_SCALE));
		tenths = levels[i] - level * ELEMENT_LEVEL_SCALE;

		if(level > ELEMENT_LEVEL_MAX)
			level = ELEMENT_LEVEL_MAX;
		if(level < 0)
			level = 0;

#if FIX_TYPE_PUNNING
		dst[i].power = ElementPowerResistByLevel[level].power + tenths;
		dst[i].resistance =
			ElementPowerResistByLevel[level].resistance + tenths;
#else
		/* UB: the stats are stored through an Sint16 [4][2] view of the four
		   structs. */
		((Sint16 (*)[2])dst)[i][0] =
			ElementPowerResistByLevel[level].power + tenths;
		((Sint16 (*)[2])dst)[i][1] =
			(ElementPowerResistByLevel + level)->resistance + tenths;
#endif
	}
}

/* Returns the class a character has with these Djinn: the series of its two
   strongest elements, or the one its class-changing item (Mysterious Card,
   Trainer's Whip, Tomegathericon) picks, and the last class of that series
   whose element requirements it meets. */
Sint32 gmClassDetermine(Sint32 character, Uint32 *djinn, Sint32 item)
{
	Sint32 levels[ELEMENT_COUNT];
	Sint32 primary;
	Sint32 best;
	Sint32 series;
	Sint32 result;
	Sint32 i;
	Sint32 second;
	Sint32 j;
	Sint32 darkMageItem = ITEM_TOMEGATHERICON;

	result = -1;
	if(character > PIERS)
		return 0;
	/* UB: gmUnitGetElementLevels returns a value; this call goes through a void
	   function type. */
#if FIX_FUNCTION_SIGNATURES
	gmUnitGetElementLevels(character, (const Uint8 *)djinn, levels);
#else
	((void (*)(Sint32, const Uint8 *, Sint32 *))gmUnitGetElementLevels)(
		character, (const Uint8 *)djinn, levels);
#endif
	best = result;
	primary = result;
	for(i = 0; i < ELEMENT_COUNT; i++) {
		if(best < levels[i]) {
			best = levels[i];
			primary = i;
		}
	}
	second = -1;
	best = -1;
	for(i = 0; i < ELEMENT_COUNT; i++) {
		if(i != primary && best < levels[i]) {
			best = levels[i];
			second = i;
		}
	}
	/* A second element below level 1 does not count, and ties go to the lower
	   element. */
	if(levels[second] < ELEMENT_LEVEL_SCALE)
		second = primary;
	series = ClassSeriesByElements[primary][second];
	/* Jenna and Piers have series of their own where the others would be a
	   Guard or a Water Seer. */
	if(series == CLASS_SERIES_GUARD && character == JENNA)
		series = CLASS_SERIES_FLAME_USER;
	else if(series == CLASS_SERIES_WATER_SEER && character == PIERS)
		series = CLASS_SERIES_MARINER;
	if(item == ITEM_TRAINERS_WHIP)
		series = CLASS_SERIES_TAMER;
	else if(item == ITEM_MYSTERIOUS_CARD)
		series = CLASS_SERIES_PIERROT;
	else if(item == darkMageItem)
		series = CLASS_SERIES_DARK_MAGE;
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
		result = 0;
	return result;
}

/* Returns class `classId`'s definition: its series, element requirements, stat
   multipliers and the Psynergy it teaches by level. */
CLASS_DEFINITION *gmClassGetDefinition(Sint32 classId)
{
	return (CLASS_DEFINITION *)&ClassDefinitionTable[classId];
}

/* Works out a character's class again after its Djinn or its class-changing
   item change, then its Psynergy list and its base elements. */
void gmUnitUpdateClass(Sint32 unitId)
{
	UNIT *unit = syUnitGet(unitId);

	unit->classId = gmClassDetermine(unit->enemyId, unit->djinnHeld,
		gmInventoryGetEquippedItem(unit, ITEM_TYPE_CLASS_CHANGE));
	gmUnitRebuildPsynergyList(unitId);
	gmUnitCalcBaseElements(unitId, unit->baseElement);
}

/* Works out a character's class, Psynergy and base elements again. */
void gmUnitRefreshPsynergy(Sint32 unitId)
{
	gmUnitUpdateClass(unitId);
}
