// Actions and Psynergy lists, characters, experience and levelling up. Part of game.c.

#include "text/msg_ids.h"

// Slots are four-byte entries, read as halfwords; UBFIX reads each entry through its own type.
#if FIX_TYPE_PUNNING
#	define PSYNERGY_ACTION_AT(slots, index) ((slots)[index].encodedAction)
#else
#	define PSYNERGY_ACTION_AT(slots, index) (((Uint16 *)(slots))[(index) * 2])
#endif

// Returns the definition of ability `abilityId` (its low 14 bits); an id past the table gives the
// first. Every module reads an ability through its stub call, gmAbilityGetDirectStub.
ABILITY_DEFINITION *gmAbilityGetDirect(Sint32 abilityId)
{
	Uint32 entryIndex = abilityId & PSYNERGY_ABILITY_MASK;

	// BUG: id ABILITY_COUNT, one past the table, is let through.
#if FIX_ABILITY_ID_BOUND
	if (entryIndex >= ABILITY_COUNT)
#else
	if (entryIndex > ABILITY_COUNT)
#endif
		entryIndex = 0;
	return (ABILITY_DEFINITION *)&AbilityDefinitions[entryIndex];
}

// Returns the first party member who knows Psynergy `action`, or -1.
Sint32 gmPartyFindOwnerWithAction(Sint32 action)
{
	Sint16 owners[10];
	Sint32 count = gmPartyListMembers(owners);
	Sint32 i;

	for (i = 0; i < count; i++)
	{
		if (gmUnitKnowsPsynergy(owners[i], action) != 0)
			break;
	}
	if (i == count)
		return -1;
	return owners[i];
}

// Returns 1 when `abilityId` is in the unit's Psynergy list. Called by the scripts that check
// whether a party member knows a Psynergy (fdPartyCheckShortcut).
Bool gmUnitKnowsPsynergy(Sint32 unitId, Sint32 abilityId)
{
	UNIT           *unit = syUnitGet(unitId);
	PSYNERGY_ENTRY *entry;
	Sint32          index      = 0;
	Sint32          actionMask = PSYNERGY_ABILITY_MASK;

	entry                      = unit->psynergy;
	for (; index < PSYNERGY_SLOTS; index++)
	{
		Sint32 currentAction = entry->encodedAction;

		entry++;
		if ((currentAction & actionMask) == abilityId)
			return TRUE;
	}
	return FALSE;
}

// Rebuilds a character's Psynergy list: what it learned for good stays, in order, then come its
// class's Psynergy up to its level and its equipped items' Psynergy, while the list has room.
Sint32 gmUnitRebuildPsynergyList(Sint32 owner)
{
	UNIT             *unit;
	PSYNERGY_ENTRY   *abilities;
	CLASS_DEFINITION *classDefinition;
	ITEM_DEFINITION  *item;
	Sint32            i;
	Sint32            j;
	Uint16            mask;
	CLASS_ABILITY    *classAbility;

	unit            = syUnitGet(owner);
	abilities       = unit->psynergy;
	classDefinition = gmClassGetDefinition(unit->classId);
	if (unit->classId == CLASS_NONE)
		return 0;
	mask = OWNER_ACTION_CLASS | OWNER_ACTION_ITEM;
	for (j = 0; j < PSYNERGY_SLOTS; j++)
	{
		if (PSYNERGY_ACTION_AT(abilities, j) & mask)
			PSYNERGY_ACTION_AT(abilities, j) = 0;
	}
	for (i = PSYNERGY_SLOTS - 1, j = PSYNERGY_SLOTS - 1; i >= 0;)
	{
		if (PSYNERGY_ACTION_AT(abilities, i) == 0)
		{
			i--;
		}
		else
		{
			PSYNERGY_ACTION_AT(abilities, j) = PSYNERGY_ACTION_AT(abilities, i);
			i--;
			j--;
		}
	}
	for (; j >= 0; j--)
		PSYNERGY_ACTION_AT(abilities, j) = 0;

	mask = OWNER_ACTION_CLASS;
	for (j = 0; j < CLASS_ABILITY_SLOTS; j++)
	{
		classAbility = &classDefinition->abilities[j];
		if (classDefinition->abilities[j].ability == 0)
			continue;
		if (unit->level < classDefinition->abilities[j].level)
			continue;
		i = 0;
		while (i < PSYNERGY_SLOTS
		       && PSYNERGY_ACTION_AT(abilities, i) != classDefinition->abilities[j].ability)
			i++;
		if (i != PSYNERGY_SLOTS)
			continue;
		for (i = 0; i < PSYNERGY_SLOTS; i++)
		{
			if (PSYNERGY_ACTION_AT(abilities, i) == 0)
			{
				PSYNERGY_ACTION_AT(abilities, i) = classAbility->ability | OWNER_ACTION_CLASS;
				break;
			}
		}
		if (i == PSYNERGY_SLOTS)
			break;
	}

	for (j = 0; j < INVENTORY_SLOTS; j++)
	{
		if (unit->inventory[j] == 0)
			continue;
		if (!(unit->inventory[j] & INVENTORY_EQUIPPED))
			continue;
		item = gmItemGetDirect(unit->inventory[j]);
		if (item->useType != ITEM_USE_TEACHES)
			continue;
		for (i = 0; i < PSYNERGY_SLOTS; i++)
		{
			if ((PSYNERGY_ACTION_AT(abilities, i) & PSYNERGY_ABILITY_MASK) == item->abilityId)
				break;
		}
		if (i != PSYNERGY_SLOTS)
			continue;
		for (i = 0; i < PSYNERGY_SLOTS; i++)
		{
			if (PSYNERGY_ACTION_AT(abilities, i) == 0)
			{
				PSYNERGY_ACTION_AT(abilities, i) = item->abilityId | OWNER_ACTION_ITEM;
				break;
			}
		}
		if (i == PSYNERGY_SLOTS)
			break;
	}

	for (i = 0, j = 0; i < PSYNERGY_SLOTS;)
	{
		if (PSYNERGY_ACTION_AT(abilities, i) == 0)
		{
			i++;
		}
		else
		{
			PSYNERGY_ACTION_AT(abilities, j) = PSYNERGY_ACTION_AT(abilities, i);
			i++;
			j++;
		}
	}
	for (; j < PSYNERGY_SLOTS; j++)
		PSYNERGY_ACTION_AT(abilities, j) = 0;
	return 0;
}

// Teaches character `owner` ability `ability` for good (or makes a class action permanent by
// clearing its marks), then rebuilds its list. Returns the action's place in the list, or -1 when
// the list is full.
Sint32 gmUnitLearnPsynergy(Sint32 owner, Sint32 ability)
{
	UNIT  *unit  = syUnitGet(owner);
	Sint32 key   = ability & PSYNERGY_ABILITY_MASK;
	Sint32 found = -1;
	Sint32 index;

	// Learning never takes the list's last slot.
	for (index = 0; index < PSYNERGY_SLOTS - 1; index++)
	{
		Sint32 masked = unit->psynergy[index].encodedAction & PSYNERGY_ABILITY_MASK;

		// FAKEMATCH(dead): the test written as an XOR; as masked == key, which
		// * stores key in place of masked.
		if ((masked ^ key) == 0)
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
				unit->psynergy[index].encodedAction = key;
				found                               = index;
				break;
			}
		}
		if (found < 0)
		{
			return -1;
		}
	}

	gmUnitRefreshPsynergy(owner);
	for (index = 0; index < PSYNERGY_SLOTS; index++)
	{
		if (unit->psynergy[index].encodedAction == key)
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

// Returns character `index`'s definition: growth curves, starting level and equipment.
const CHARACTER_DEFINITION *gmCharacterGetDefinition(Sint32 index)
{
	const CHARACTER_DEFINITION *base;

	base = CharacterDefinitionTable;
	return &base[index];
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
		unit = syUnitGet(unitId);
		mnMessageDecodeRawUnboundedStub(unitId + (Sint32)MsgCharacterName, nameBuf);
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
	// (rare_item_counters.c).
	if (*startingUnitId != -1)
	{
		for (; *startingUnitId != -1; startingUnitId++)
		{
			unit = syUnitGet(*startingUnitId);
			if (unit != 0)
			{
				unit->enemyId = *startingUnitId;
				character     = gmCharacterGetDefinition(unit->enemyId);
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
				gmUnitUpdateClass(*startingUnitId);
				gmUnitCalculate(*startingUnitId);
			}
		}
	}
}

// Unused: reserved entry; keeps its place in the stub table.
UNUSED void gmUnitLevelNoOperation(void)
{
}

// Returns the experience a character needs for `level`: 0 below level 1, and -1 for an enemy or a
// level past UNIT_LEVEL_MAX.
Uint32 gmUnitGetExperienceForLevel(Sint32 unitId, Sint32 level)
{
	UNIT *unit = syUnitGet(unitId);

	if (unit->classId != CLASS_NONE)
	{
		if (level <= 0)
		{
			return 0;
		}
		if (level <= UNIT_LEVEL_MAX && unit->enemyId <= PIERS)
		{
			return CharacterLevelExpTable[unit->enemyId * UNIT_LEVEL_MAX + level - 1];
		}
	}
	return -1;
}

// Raises a character one level (up to UNIT_LEVEL_MAX), adding to each base stat the growth between
// the two points of its growth curve around the new level (a point every GROWTH_BAND_LEVELS
// levels), a twentieth of it per level with a random part. Writes the unit's level, base stats and,
// if short, its experience, then rebuilds its Psynergy and stats. The gains go to `gains`, which is
// returned.
LEVEL_UP_RESULT *gmUnitLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains)
{
	UNIT           *unit;
	LEVEL_UP_STATE *state;
	Uint32          requiredExperience;
	Sint16          growthBand;
	Sint32          growthDifference;
	Sint32          level;

	unit              = syUnitGet(unitId);
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
		if (requiredExperience != (Uint32)-1 && unit->experience < requiredExperience)
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
		if (growthBand < 0)
			growthBand = 0;
		if (growthBand > GROWTH_POINTS - 2)
			growthBand = GROWTH_POINTS - 2;
		growthDifference = state->growth->hp[growthBand + 1] - state->growth->hp[growthBand];
		gains->hp += ((syRandomBelow(GROWTH_BAND_LEVELS)) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference = state->growth->pp[growthBand + 1] - state->growth->pp[growthBand];
		gains->pp += ((syRandomBelow(GROWTH_BAND_LEVELS)) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference =
			state->growth->attack[growthBand + 1] - state->growth->attack[growthBand];
		gains->attack +=
			((syRandomBelow(GROWTH_BAND_LEVELS)) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference =
			state->growth->defense[growthBand + 1] - state->growth->defense[growthBand];
		gains->defense +=
			((syRandomBelow(GROWTH_BAND_LEVELS)) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference =
			state->growth->agility[growthBand + 1] - state->growth->agility[growthBand];
		gains->agility +=
			((syRandomBelow(GROWTH_BAND_LEVELS)) + growthDifference) / GROWTH_BAND_LEVELS;
		growthDifference = state->growth->luck[growthBand + 1] - state->growth->luck[growthBand];
		gains->luck +=
			((syRandomBelow(GROWTH_BAND_LEVELS)) + growthDifference) / GROWTH_BAND_LEVELS;
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
		((Sint32 (*)(Sint32))gmUnitRefreshPsynergy)(unitId);
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

	unit = syUnitGet(unitId);
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

	for (currentLevel = syUnitGet(unitId)->level; currentLevel < targetLevel; currentLevel++)
	{
		gmUnitLevelUp(unitId, &gains);
	}
	gmUnitCalculate(unitId);
}

#undef PSYNERGY_ACTION_AT
