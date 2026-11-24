/// @file
/// Actions and Psynergy lists, characters, experience and levelling up.
// Part of game.c.

#include "game_internal.h"

/// Slots are four-byte entries, read as halfwords; UBFIX reads each entry
/// through its own type.
#ifdef UBFIX
#define PSYNERGY_ACTION_AT(slots, index) ((slots)[index].encodedAction)
#else
#define PSYNERGY_ACTION_AT(slots, index) (((u16 *)(slots))[(index) * 2])
#endif

/// Returns the definition of ability `abilityId` (its low 14 bits); an id past
/// the table gives the first.
struct AbilityDefinition *Ability_GetDirect(s32 abilityId) {
  u32 entryIndex;

  entryIndex = abilityId & PSYNERGY_ABILITY_MASK;
  // BUG: the bound is one past the table's rows, so id ABILITY_COUNT reads
  // the enemy table's first bytes as an action.
#ifdef BUGFIX
  if (entryIndex >= ABILITY_COUNT) {
#else
  if (entryIndex >= ABILITY_COUNT + 1) {
#endif
    entryIndex = 0;
  }
  return (struct AbilityDefinition *)&gAbilityDefinitions[entryIndex];
}

/// Returns 1 when `abilityId` is in the unit's Psynergy list.
s32 Unit_KnowsPsynergy(s32 unitId, s32 abilityId) {
  struct Unit *unit = Unit_Get(unitId);
  struct PsynergyEntry *entry;
  s32 actionMask = PSYNERGY_ABILITY_MASK;
  s32 index = 0;

  entry = unit->psynergy;
  for (; index < PSYNERGY_SLOTS; index++) {
    s32 currentAction = entry->encodedAction;

    currentAction &= actionMask;
    entry++;
    if (currentAction == abilityId) {
      return 1;
    }
  }
  return 0;
}

/// Marks on an action list entry: learned from the class, or granted by an
/// equipped item. Plain entries were taught for good.
enum { PSYNERGY_FROM_CLASS = 0x8000, PSYNERGY_FROM_ITEM = 0x4000 };

/// Rebuilds a character's Psynergy list: drops the actions its class and items
/// gave, closes up the list, adds every class action learned by its level and
/// the action of each equipped item that grants one, and closes it up again.
s32 Unit_RefreshPsynergy(s32 unitId) {
  struct Unit *unit;
  struct PsynergyEntry *abilities;
  struct ClassDefinition *classDefinition;
  const struct ItemDefinition *item;
  s32 slot;
  s32 sourceIndex;
  u16 originFlag;
  struct ClassAbility *classAbility;

  unit = Unit_Get(unitId);
  abilities = unit->psynergy;
  classDefinition = Class_GetDefinition(unit->classId);
  if (unit->classId == CLASS_NONE)
    return 0;
  // Remove actions inherited from the previous class and equipment.
  originFlag = PSYNERGY_FROM_CLASS;
  for (sourceIndex = 0; sourceIndex < PSYNERGY_SLOTS; sourceIndex++) {
    if (PSYNERGY_ACTION_AT(abilities, sourceIndex) & originFlag)
      PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  }
  originFlag = PSYNERGY_FROM_ITEM;
  for (sourceIndex = 0; sourceIndex < PSYNERGY_SLOTS; sourceIndex++) {
    if (PSYNERGY_ACTION_AT(abilities, sourceIndex) & originFlag)
      PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  }
  // Pack permanent actions at the end to leave room for class abilities.
  for (slot = PSYNERGY_SLOTS - 1, sourceIndex = PSYNERGY_SLOTS - 1;
       slot >= 0;) {
    if (abilities[slot].encodedAction == 0) {
      slot--;
    } else {
      abilities[sourceIndex].encodedAction = abilities[slot].encodedAction;
      slot--;
      sourceIndex--;
    }
  }
  for (; sourceIndex >= 0; sourceIndex--)
    PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;

  // Add level-eligible class abilities, followed by equipped-item abilities.
  originFlag = PSYNERGY_FROM_CLASS;
  for (sourceIndex = 0; sourceIndex < CLASS_ABILITY_SLOTS; sourceIndex++) {
    classAbility = &classDefinition->abilities[sourceIndex];
    if (classDefinition->abilities[sourceIndex].ability == 0)
      continue;
    if (unit->level < classDefinition->abilities[sourceIndex].level)
      continue;
    slot = 0;
    while (slot < PSYNERGY_SLOTS &&
           PSYNERGY_ACTION_AT(abilities, slot) !=
               classDefinition->abilities[sourceIndex].ability)
      slot++;
    if (slot != PSYNERGY_SLOTS)
      continue;
    for (slot = 0; slot < PSYNERGY_SLOTS; slot++) {
      if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
        PSYNERGY_ACTION_AT(abilities, slot) =
            classAbility->ability | originFlag;
        break;
      }
    }
    if (slot == PSYNERGY_SLOTS)
      break;
  }

  for (sourceIndex = 0; sourceIndex < INVENTORY_SLOTS; sourceIndex++) {
    if (unit->inventory[sourceIndex] == 0)
      continue;
    if (!(unit->inventory[sourceIndex] & INVENTORY_EQUIPPED))
      continue;
    item = Item_GetDirect(unit->inventory[sourceIndex]);
    if (item->useType != ITEM_USE_TEACHES)
      continue;
    for (slot = 0; slot < PSYNERGY_SLOTS; slot++) {
      if ((PSYNERGY_ACTION_AT(abilities, slot) & PSYNERGY_ABILITY_MASK) ==
          item->abilityId)
        break;
    }
    if (slot != PSYNERGY_SLOTS)
      continue;
    for (slot = 0; slot < PSYNERGY_SLOTS; slot++) {
      if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
        PSYNERGY_ACTION_AT(abilities, slot) =
            item->abilityId | PSYNERGY_FROM_ITEM;
        break;
      }
    }
    if (slot == PSYNERGY_SLOTS)
      break;
  }

  // Restore a dense display list with empty slots at the end.
  for (slot = 0, sourceIndex = 0; slot < PSYNERGY_SLOTS;) {
    if (abilities[slot].encodedAction == 0) {
      slot++;
    } else {
      abilities[sourceIndex].encodedAction = abilities[slot].encodedAction;
      slot++;
      sourceIndex++;
    }
  }
  for (; sourceIndex < PSYNERGY_SLOTS; sourceIndex++)
    PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  return 0;
}

/// Scratch for Unit_LevelUp, which takes it from EWRAM first
/// (Heap_AllocEwram) and frees it before returning.
struct LevelUpState {
  s32 classIndex; ///< the unit's class, written and not read again
  s32 level;      ///< the level before this one
  const struct CharacterDefinition *growth; ///< the character's growth curves
  u8 unused[32];                            ///< never written
};

/// Teaches a character `abilityId` for good (or makes a class action permanent
/// by clearing its marks), then rebuilds its list. Returns the action's place
/// in the list, or -1 when the list is full.
s32 Unit_LearnPsynergy(s32 unitId, s32 abilityId) {
  struct Unit *unit = Unit_Get(unitId);
  s32 ability = abilityId & PSYNERGY_ABILITY_MASK;
  s32 found = -1;
  s32 index;

  // Learning never takes the list's last slot.
  for (index = 0; index < PSYNERGY_SLOTS - 1; index++) {
    s32 masked = unit->psynergy[index].encodedAction & PSYNERGY_ABILITY_MASK;

    if ((masked ^ ability) == 0) {
      unit->psynergy[index].encodedAction = masked;
      found = index;
      break;
    }
  }

  if (found < 0) {
    for (index = 0; index < PSYNERGY_SLOTS - 1; index++) {
      s32 action = unit->psynergy[index].encodedAction;

      if (action == 0) {
        unit->psynergy[index].encodedAction = ability;
        found = index;
        break;
      }
    }
    if (found < 0) {
      return -1;
    }
  }

  Unit_RefreshPsynergy(unitId);
  for (index = 0; index < PSYNERGY_SLOTS; index++) {
    if (unit->psynergy[index].encodedAction == ability) {
      break;
    }
  }
  return index;
}

/// Levels the character up once if it has the experience, and returns gains, or
/// NULL.
struct LevelUpResult *Unit_CheckLevelUp(s32 unitId,
                                        struct LevelUpResult *gains) {
  return Unit_TryLevelUp(unitId, gains);
}

/// Returns character `characterId`'s definition: growth curves, starting level
/// and equipment.
const struct CharacterDefinition *Character_GetDefinition(s32 characterId) {
  const struct CharacterDefinition *definitions;

  definitions = gCharacter_DefinitionTable;
  return &definitions[characterId];
}

/// Sets up all eight characters for a new game: names from the message table,
/// starting equipment equipped, class and elements worked out, and levels
/// raised to their starting level.
void Party_InitializeCharacters(void) {
  struct Unit *unit;
  const struct CharacterDefinition *character;
  u16 nameBuf[16];
  s32 unitId;
  const s32 *startingUnitId = gCharacter_NewGameIds;
  s32 i;
  s32 slot;
  u8 *name;

  for (unitId = 0; unitId <= ALLY_MAX; unitId++) {
    unit = Unit_Get(unitId);
    Message_DecodeRawUnboundedFar(unitId + MsgCharacterName, nameBuf);
    name = unit->name;
    name[0] = nameBuf[0];
    i = 0;
    if (nameBuf[0] != 0) {
      do {
        i++;
        if (i >= UNIT_NAME_SIZE - 1)
          break;
        name[i] = nameBuf[i];
      } while (nameBuf[i] != 0);
    }
    unit->name[UNIT_NAME_SIZE - 1] = 0;
  }

  // gCharacter_NewGameIds lists the characters given starting
  // equipment, ended by -1 (starting_equipment.c).
  for (; *startingUnitId != -1; startingUnitId++) {
    unit = Unit_Get(*startingUnitId);
    if (unit != 0) {
      unit->id = *startingUnitId;
      character = Character_GetDefinition(unit->id);

      for (i = 0; i < INVENTORY_SLOTS; i++)
        unit->inventory[i] = 0;

      for (i = 0; i < sizeof(character->items) / sizeof(character->items[0]);
           i++) {
        slot = Inventory_AddItem(*startingUnitId,
                                 character->items[i] & INVENTORY_ITEM_ID_MASK);
        Inventory_Equip(*startingUnitId, slot);
      }

      Unit_UpdateClass(*startingUnitId);
      unit->ppGauge = UNIT_GAUGE_FULL;
      unit->hpGauge = UNIT_GAUGE_FULL;
      Unit_LevelUpTo(*startingUnitId, character->startingLevel);
      Unit_Calculate(*startingUnitId);
    }
  }
}

/// Unused: reserved entry; keeps its place in the far-call table.
UNUSED void Unit_LevelNoOp(void) {}

/// Returns the experience a character needs for `level` (0 for level 0 and
/// below); -1 for an enemy or a level above 99.
u32 Unit_GetExperienceForLevel(s32 unitId, s32 level) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->classId != CLASS_NONE) {
    if (level <= 0) {
      return 0;
    }
    if (level <= UNIT_LEVEL_MAX && unit->id <= PIERS) {
      return gCharacter_LevelExpTable[unit->id * UNIT_LEVEL_MAX + level - 1];
    }
  }
  return -1;
}

/// Raises a character one level, up to 99, each base stat growing along its
/// curve with a random part. Returns gains.
struct LevelUpResult *Unit_LevelUp(s32 unitId, struct LevelUpResult *gains) {
  struct Unit *unit;
  struct LevelUpState *state;
  u32 requiredExperience;
  s16 growthBand;
  s32 growthDifference;
  s32 level;

  unit = Unit_Get(unitId);
  state = Heap_AllocEwram(sizeof(struct LevelUpState));
  state->classIndex = unit->classId;
  level = unit->level;
  state->level = level;
  gains->level = level;
  gains->ability = -1;
  gains->hp = 0;
  gains->pp = 0;
  gains->attack = 0;
  gains->defense = 0;
  gains->agility = 0;
  gains->luck = 0;
  if (level < UNIT_LEVEL_MAX) {
    unit->level++;
    gains->level = level + 1;
    requiredExperience = Unit_GetExperienceForLevel(unitId, unit->level);
    if (requiredExperience != -1 && unit->experience < requiredExperience)
      unit->experience = requiredExperience;
    state->growth = Character_GetDefinition(unitId);
    // Level 1 starts from the curve's first point.
    if (gains->level == 1) {
      gains->hp += state->growth->hp[0];
      gains->pp += state->growth->pp[0];
      gains->attack += state->growth->attack[0];
      gains->defense += state->growth->defense[0];
      gains->agility += state->growth->agility[0];
      gains->luck += state->growth->luck[0];
    }
    // Each twenty-level band grows by the step between its growth points, a
    // twentieth a level with a random part; one roll per stat, in order.
    growthBand = gains->level / GROWTH_BAND_LEVELS;
    CLAMP(growthBand, 0, GROWTH_POINTS - 2);
    growthDifference =
        state->growth->hp[growthBand + 1] - state->growth->hp[growthBand];
    gains->hp += (Random16_Below(GROWTH_BAND_LEVELS) + growthDifference) /
                 GROWTH_BAND_LEVELS;
    growthDifference =
        state->growth->pp[growthBand + 1] - state->growth->pp[growthBand];
    gains->pp += (Random16_Below(GROWTH_BAND_LEVELS) + growthDifference) /
                 GROWTH_BAND_LEVELS;
    growthDifference = state->growth->attack[growthBand + 1] -
                       state->growth->attack[growthBand];
    gains->attack += (Random16_Below(GROWTH_BAND_LEVELS) + growthDifference) /
                     GROWTH_BAND_LEVELS;
    growthDifference = state->growth->defense[growthBand + 1] -
                       state->growth->defense[growthBand];
    gains->defense += (Random16_Below(GROWTH_BAND_LEVELS) + growthDifference) /
                      GROWTH_BAND_LEVELS;
    growthDifference = state->growth->agility[growthBand + 1] -
                       state->growth->agility[growthBand];
    gains->agility += (Random16_Below(GROWTH_BAND_LEVELS) + growthDifference) /
                      GROWTH_BAND_LEVELS;
    growthDifference =
        state->growth->luck[growthBand + 1] - state->growth->luck[growthBand];
    gains->luck += (Random16_Below(GROWTH_BAND_LEVELS) + growthDifference) /
                   GROWTH_BAND_LEVELS;
    // Commit the six gains before rebuilding Psynergy and derived stats.
    unit->baseHp += gains->hp;
    unit->basePp += gains->pp;
    unit->baseAttack += gains->attack;
    unit->baseDefense += gains->defense;
    unit->baseAgility += gains->agility;
    unit->baseLuck += gains->luck;
    // A character's base is one action a turn and no regeneration.
    unit->baseActionCount = 1;
    unit->baseHpRegen = 0;
    unit->basePpRegen = 0;
    Unit_RefreshPsynergy(unitId);
    Unit_Calculate(unitId);
  }
  Heap_Release(state);
  return gains;
}

/// Levels the character up once if its experience reaches the next level, and
/// returns gains, or NULL.
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains) {
  struct Unit *unit;

  unit = Unit_Get(unitId);
  if ((unit->experience >=
       Unit_GetExperienceForLevel(unitId, unit->level + 1)) &&
      (Unit_LevelUp(unitId, gains) != NULL)) {
    return gains;
  }
  return 0;
}

/// Levels character `unitId` up one level at a time until `targetLevel`, then
/// recomputes its stats.
void Unit_LevelUpTo(s32 unitId, s32 targetLevel) {
  // Each level's gains are discarded.
  struct LevelUpResult gains;
  s32 currentLevel;

  for (currentLevel = Unit_Get(unitId)->level; currentLevel < targetLevel;
       currentLevel++) {
    Unit_LevelUp(unitId, &gains);
  }
  Unit_Calculate(unitId);
}

#undef PSYNERGY_ACTION_AT
