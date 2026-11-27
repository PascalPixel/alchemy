// Actions and Psynergy lists, characters, experience and levelling up. Part of game.c.

// Slots are four-byte entries, read as halfwords; UBFIX reads each entry through its own type.
#if FIX_TYPE_PUNNING
#	define PSYNERGY_ACTION_AT(slots, index) ((slots)[index].encodedAction)
#else
#	define PSYNERGY_ACTION_AT(slots, index) (((Uint16 *)(slots))[(index) * 2])
#endif

// Returns the definition of ability `abilityId` (its low 14 bits); an id past the table gives the
// first.
ABILITY_DEFINITION *gmAbilityGetDirect(Sint32 abilityId)
{
	Uint32 entryIndex;

	entryIndex = abilityId & PSYNERGY_ABILITY_MASK;
	// BUG: the bound is one past the table's rows, so id ABILITY_COUNT reads the enemy table's
	// first bytes as an action.
#if FIX_ABILITY_ID_BOUND
	if (entryIndex >= ABILITY_COUNT)
	{
#else
	if (entryIndex >= ABILITY_COUNT + 1)
	{
#endif
		entryIndex = 0;
	}
	return (ABILITY_DEFINITION *)&AbilityDefinitions[entryIndex];
}

// Returns 1 when `abilityId` is in the unit's Psynergy list.
Bool gmUnitKnowsPsynergy(Sint32 unitId, Sint32 abilityId)
{
	UNIT           *unit = gmUnitGet(unitId);
	PSYNERGY_ENTRY *entry;
	Sint32          actionMask = PSYNERGY_ABILITY_MASK;
	Sint32          index      = 0;

	entry                      = unit->psynergy;
	for (; index < PSYNERGY_SLOTS; index++)
	{
		Sint32 currentAction  = entry->encodedAction;

		currentAction        &= actionMask;
		entry++;
		if (currentAction == abilityId)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// Marks on an action list entry: learned from the class, or granted by an equipped item. Plain
// entries were taught for good.
enum
{
	PSYNERGY_FROM_CLASS = 0x8000,
	PSYNERGY_FROM_ITEM  = 0x4000
};

// Rebuilds a character's Psynergy list: drops the actions its class and items gave, closes up the
// list, adds every class action learned by its level and the action of each equipped item that
// grants one, and closes it up again.
Sint32 gmUnitRefreshPsynergy(Sint32 unitId)
{
	UNIT                  *unit;
	PSYNERGY_ENTRY        *abilities;
	CLASS_DEFINITION      *classDefinition;
	const ITEM_DEFINITION *item;
	Sint32                 slot;
	Sint32                 srcIndex;
	Uint16                 originFlag;
	CLASS_ABILITY         *classAbility;

	unit            = gmUnitGet(unitId);
	abilities       = unit->psynergy;
	classDefinition = gmClassGetDefinition(unit->classId);
	if (unit->classId == CLASS_NONE)
		return 0;
	// Remove actions inherited from the previous class and equipment.
	originFlag = PSYNERGY_FROM_CLASS;
	for (srcIndex = 0; srcIndex < PSYNERGY_SLOTS; srcIndex++)
	{
		if (PSYNERGY_ACTION_AT(abilities, srcIndex) & originFlag)
			PSYNERGY_ACTION_AT(abilities, srcIndex) = 0;
	}
	originFlag = PSYNERGY_FROM_ITEM;
	for (srcIndex = 0; srcIndex < PSYNERGY_SLOTS; srcIndex++)
	{
		if (PSYNERGY_ACTION_AT(abilities, srcIndex) & originFlag)
			PSYNERGY_ACTION_AT(abilities, srcIndex) = 0;
	}
	// Pack permanent actions at the end to leave room for class abilities.
	for (slot = PSYNERGY_SLOTS - 1, srcIndex = PSYNERGY_SLOTS - 1; slot >= 0;)
	{
		if (abilities[slot].encodedAction == 0)
		{
			slot--;
		}
		else
		{
			abilities[srcIndex].encodedAction = abilities[slot].encodedAction;
			slot--;
			srcIndex--;
		}
	}
	for (; srcIndex >= 0; srcIndex--)
		PSYNERGY_ACTION_AT(abilities, srcIndex) = 0;

	// Add level-eligible class abilities, followed by equipped-item abilities.
	originFlag = PSYNERGY_FROM_CLASS;
	for (srcIndex = 0; srcIndex < CLASS_ABILITY_SLOTS; srcIndex++)
	{
		classAbility = &classDefinition->abilities[srcIndex];
		if (classDefinition->abilities[srcIndex].ability == 0)
			continue;
		if (unit->level < classDefinition->abilities[srcIndex].level)
			continue;
		slot = 0;
		while (slot < PSYNERGY_SLOTS
		       && PSYNERGY_ACTION_AT(abilities, slot)
		              != classDefinition->abilities[srcIndex].ability)
			slot++;
		if (slot != PSYNERGY_SLOTS)
			continue;
		for (slot = 0; slot < PSYNERGY_SLOTS; slot++)
		{
			if (PSYNERGY_ACTION_AT(abilities, slot) == 0)
			{
				PSYNERGY_ACTION_AT(abilities, slot) = classAbility->ability | originFlag;
				break;
			}
		}
		if (slot == PSYNERGY_SLOTS)
			break;
	}

	for (srcIndex = 0; srcIndex < INVENTORY_SLOTS; srcIndex++)
	{
		if (unit->inventory[srcIndex] == 0)
			continue;
		if (!(unit->inventory[srcIndex] & INVENTORY_EQUIPPED))
			continue;
		item = gmItemGetDirect(unit->inventory[srcIndex]);
		if (item->useType != ITEM_USE_TEACHES)
			continue;
		for (slot = 0; slot < PSYNERGY_SLOTS; slot++)
		{
			if ((PSYNERGY_ACTION_AT(abilities, slot) & PSYNERGY_ABILITY_MASK) == item->abilityId)
				break;
		}
		if (slot != PSYNERGY_SLOTS)
			continue;
		for (slot = 0; slot < PSYNERGY_SLOTS; slot++)
		{
			if (PSYNERGY_ACTION_AT(abilities, slot) == 0)
			{
				PSYNERGY_ACTION_AT(abilities, slot) = item->abilityId | PSYNERGY_FROM_ITEM;
				break;
			}
		}
		if (slot == PSYNERGY_SLOTS)
			break;
	}

	// Restore a dense display list with empty slots at the end.
	for (slot = 0, srcIndex = 0; slot < PSYNERGY_SLOTS;)
	{
		if (abilities[slot].encodedAction == 0)
		{
			slot++;
		}
		else
		{
			abilities[srcIndex].encodedAction = abilities[slot].encodedAction;
			slot++;
			srcIndex++;
		}
	}
	for (; srcIndex < PSYNERGY_SLOTS; srcIndex++)
		PSYNERGY_ACTION_AT(abilities, srcIndex) = 0;
	return 0;
}

// Scratch for gmUnitLevelUp, which takes it from EWRAM first (syHeapAllocateEWRAM) and frees it
// before returning.
typedef struct LEVEL_UP_STATE
{
	Sint32                      classIndex; // the unit's class, written and not read again
	Sint32                      level;      // the level before this one
	const CHARACTER_DEFINITION *growth;     // the character's growth curves
	Uint8                       unused[32]; // never written
} LEVEL_UP_STATE;

// Teaches a character `abilityId` for good (or makes a class action permanent by clearing its
// marks), then rebuilds its list. Returns the action's place in the list, or -1 when the list is
// full.
Sint32 gmUnitLearnPsynergy(Sint32 unitId, Sint32 abilityId)
{
	UNIT  *unit    = gmUnitGet(unitId);
	Sint32 ability = abilityId & PSYNERGY_ABILITY_MASK;
	Sint32 found   = -1;
	Sint32 index;

	// Learning never takes the list's last slot.
	for (index = 0; index < PSYNERGY_SLOTS - 1; index++)
	{
		Sint32 masked = unit->psynergy[index].encodedAction & PSYNERGY_ABILITY_MASK;

		// FAKEMATCH(dead): the test written as an XOR; as masked == ability,
		// * which stores ability in place of masked.
		if ((masked ^ ability) == 0)
		{
			unit->psynergy[index].encodedAction = masked;
			found                               = index;
			break;
		}
	}

	if (found < 0)
	{
		for (index = 0; index < PSYNERGY_SLOTS - 1; index++)
		{
			Sint32 action = unit->psynergy[index].encodedAction;

			if (action == 0)
			{
				unit->psynergy[index].encodedAction = ability;
				found                               = index;
				break;
			}
		}
		if (found < 0)
		{
			return -1;
		}
	}

	gmUnitRefreshPsynergy(unitId);
	for (index = 0; index < PSYNERGY_SLOTS; index++)
	{
		if (unit->psynergy[index].encodedAction == ability)
		{
			break;
		}
	}
	return index;
}

// Levels the character up once if it has the experience, and returns gains, or NULL.
LEVEL_UP_RESULT *gmUnitCheckLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains)
{
	return gmUnitTryLevelUp(unitId, gains);
}

// Returns character `characterId`'s definition: growth curves, starting level and equipment.
const CHARACTER_DEFINITION *gmCharacterGetDefinition(Sint32 characterId)
{
	const CHARACTER_DEFINITION *definitions;

	definitions = CharacterDefinitionTable;
	return &definitions[characterId];
}

// Sets up all eight characters for a new game: names from the message table, starting equipment
// equipped, class and elements worked out, and levels raised to their starting level.
void gmPartyInitCharacters(void)
{
	UNIT                       *unit;
	const CHARACTER_DEFINITION *character;
	Uint16                      nameBuf[16];
	Sint32                      unitId;
	const Sint32               *startingUnitId = CharacterNewGameIds;
	Sint32                      i;
	Sint32                      slot;
	Uint8                      *name;

	for (unitId = 0; unitId <= ALLY_MAX; unitId++)
	{
		unit = gmUnitGet(unitId);
		mnMessageDecodeRawUnboundedStub(unitId + MsgCharacterName, nameBuf);
		name    = unit->name;
		name[0] = nameBuf[0];
		i       = 0;
		if (nameBuf[0] != 0)
		{
			do
			{
				i++;
				if (i >= UNIT_NAME_SIZE - 1)
					break;
				name[i] = nameBuf[i];
			} while (nameBuf[i] != 0);
		}
		unit->name[UNIT_NAME_SIZE - 1] = 0;
	}

	// CharacterNewGameIds lists the characters given starting equipment, ended by -1
	// (starting_equipment.c).
	for (; *startingUnitId != -1; startingUnitId++)
	{
		unit = gmUnitGet(*startingUnitId);
		if (unit != 0)
		{
			unit->id  = *startingUnitId;
			character = gmCharacterGetDefinition(unit->id);

			for (i = 0; i < INVENTORY_SLOTS; i++)
				unit->inventory[i] = 0;

			for (i = 0; i < sizeof(character->items) / sizeof(character->items[0]); i++)
			{
				slot = gmInventoryAddItem(*startingUnitId,
				                          character->items[i] & INVENTORY_ITEM_ID_MASK);
				gmInventoryEquip(*startingUnitId, slot);
			}

			gmUnitUpdateClass(*startingUnitId);
			unit->ppGauge = UNIT_GAUGE_FULL;
			unit->hpGauge = UNIT_GAUGE_FULL;
			gmUnitLevelUpTo(*startingUnitId, character->startingLevel);
			gmUnitCalculate(*startingUnitId);
		}
	}
}

// Unused: reserved entry; keeps its place in the stub table.
UNUSED void gmUnitLevelNoOperation(void)
{
}

// Returns the experience a character needs for `level` (0 for level 0 and below); -1 for an enemy
// or a level above 99.
Uint32 gmUnitGetExperienceForLevel(Sint32 unitId, Sint32 level)
{
	UNIT *unit = gmUnitGet(unitId);

	if (unit->classId != CLASS_NONE)
	{
		if (level <= 0)
		{
			return 0;
		}
		if (level <= UNIT_LEVEL_MAX && unit->id <= PIERS)
		{
			return CharacterLevelExpTable[unit->id * UNIT_LEVEL_MAX + level - 1];
		}
	}
	return -1;
}

// Raises a character one level, up to 99, each base stat growing along its curve with a random
// part. Returns gains.
LEVEL_UP_RESULT *gmUnitLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains)
{
	UNIT           *unit;
	LEVEL_UP_STATE *state;
	Uint32          requiredExperience;
	Sint16          growthBand;
	Sint32          growthDifference;
	Sint32          level;

	unit              = gmUnitGet(unitId);
	state             = syHeapAllocateEWRAM(sizeof(LEVEL_UP_STATE));
	state->classIndex = unit->classId;
	level             = unit->level;
	state->level      = level;
	gains->level      = level;
	gains->ability    = -1;
	gains->hp         = 0;
	gains->pp         = 0;
	gains->attack     = 0;
	gains->defense    = 0;
	gains->agility    = 0;
	gains->luck       = 0;
	if (level < UNIT_LEVEL_MAX)
	{
		unit->level++;
		gains->level       = level + 1;
		requiredExperience = gmUnitGetExperienceForLevel(unitId, unit->level);
		if (requiredExperience != -1 && unit->experience < requiredExperience)
			unit->experience = requiredExperience;
		state->growth = gmCharacterGetDefinition(unitId);
		// Level 1 starts from the curve's first point.
		if (gains->level == 1)
		{
			gains->hp      += state->growth->hp[0];
			gains->pp      += state->growth->pp[0];
			gains->attack  += state->growth->attack[0];
			gains->defense += state->growth->defense[0];
			gains->agility += state->growth->agility[0];
			gains->luck    += state->growth->luck[0];
		}
		// Each twenty-level band grows by the step between its growth points, a twentieth a level
		// with a random part; one roll per stat, in order.
		growthBand = gains->level / GROWTH_BAND_LEVELS;
		CLAMP(growthBand, 0, GROWTH_POINTS - 2);
		growthDifference = state->growth->hp[growthBand + 1] - state->growth->hp[growthBand];
		gains->hp += (syRandom16Below(GROWTH_BAND_LEVELS) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference = state->growth->pp[growthBand + 1] - state->growth->pp[growthBand];
		gains->pp += (syRandom16Below(GROWTH_BAND_LEVELS) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference =
			state->growth->attack[growthBand + 1] - state->growth->attack[growthBand];
		gains->attack +=
			(syRandom16Below(GROWTH_BAND_LEVELS) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference =
			state->growth->defense[growthBand + 1] - state->growth->defense[growthBand];
		gains->defense +=
			(syRandom16Below(GROWTH_BAND_LEVELS) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference =
			state->growth->agility[growthBand + 1] - state->growth->agility[growthBand];
		gains->agility +=
			(syRandom16Below(GROWTH_BAND_LEVELS) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference = state->growth->luck[growthBand + 1] - state->growth->luck[growthBand];
		gains->luck +=
			(syRandom16Below(GROWTH_BAND_LEVELS) + growthDifference) / GROWTH_BAND_LEVELS;
		// Commit the six gains before rebuilding Psynergy and derived stats.
		unit->baseHp          += gains->hp;
		unit->basePp          += gains->pp;
		unit->baseAttack      += gains->attack;
		unit->baseDefense     += gains->defense;
		unit->baseAgility     += gains->agility;
		unit->baseLuck        += gains->luck;
		// A character's base is one action a turn and no regeneration.
		unit->baseActionCount  = 1;
		unit->baseHpRegen      = 0;
		unit->basePpRegen      = 0;
		gmUnitRefreshPsynergy(unitId);
		gmUnitCalculate(unitId);
	}
	syHeapRelease(state);
	return gains;
}

// Levels the character up once if its experience reaches the next level, and returns gains, or
// NULL.
LEVEL_UP_RESULT *gmUnitTryLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains)
{
	UNIT *unit;

	unit = gmUnitGet(unitId);
	if ((unit->experience >= gmUnitGetExperienceForLevel(unitId, unit->level + 1))
	    && (gmUnitLevelUp(unitId, gains) != NULL))
	{
		return gains;
	}
	return 0;
}

// Levels character `unitId` up one level at a time until `targetLevel`, then recomputes its stats.
void gmUnitLevelUpTo(Sint32 unitId, Sint32 targetLevel)
{
	// Each level's gains are discarded.
	LEVEL_UP_RESULT gains;
	Sint32          currentLevel;

	for (currentLevel = gmUnitGet(unitId)->level; currentLevel < targetLevel; currentLevel++)
	{
		gmUnitLevelUp(unitId, &gains);
	}
	gmUnitCalculate(unitId);
}

#undef PSYNERGY_ACTION_AT
