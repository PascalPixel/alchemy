/// @file
/// Actions and Psynergy lists, characters, experience and levelling up.
// Part of game.c.

#include "../game_internal.h"

/// Returns the definition of ability `abilityId` (its low 14 bits); an id
/// past the table gives the first. Reached through GAME.S as
/// Ability_Get, which every bank uses to read an ability.
struct AbilityDefinition *Ability_GetDirect(s32 abilityId) {
  u32 entryIndex;

  entryIndex = abilityId & PSYNERGY_ABILITY_MASK;
  // BUG: the bound is one past the table's 519 rows, so id 519 reads the
  // enemy table's first bytes as an action.
#ifdef BUGFIX
  if (entryIndex >= 519U) {
#else
  if (entryIndex >= 0x208U) {
#endif
    entryIndex = 0;
  }
  return (struct AbilityDefinition *)&gAbilityDefinitions[entryIndex];
}

/// Returns 1 when `abilityId` is in the unit's Psynergy list. Called by the
/// events that check whether a party member knows a Psynergy
/// (Event_ValidatePackedId).
s32 Unit_KnowsPsynergy(s32 unitId, s32 abilityId) {
  struct Unit *unit = Unit_Get(unitId);
  struct PsynergyEntry *entry;
  s32 actionMask = PSYNERGY_ABILITY_MASK;
  s32 index = 0;

  entry = unit->psynergy;
  do {
    s32 currentAction = entry->encodedAction;

    currentAction &= actionMask;
    entry++;
    if (currentAction == abilityId) {
      return 1;
    }
    index++;
  } while (index <= 31);
  return 0;
}

/// Marks on an action list entry: learned from the class, or granted by an
/// equipped item. Plain entries were taught for good.
enum { PSYNERGY_FROM_CLASS = 0x8000, PSYNERGY_FROM_ITEM = 0x4000 };

// Slots are four-byte entries, not a flat halfword array. The native view
// preserves load scheduling; the typed view shortens the English object by
// 52 bytes. UBFIX accesses each entry through its actual declared type.
#ifdef UBFIX
#define PSYNERGY_ACTION_AT(slots, index) ((slots)[index].encodedAction)
#else
#define PSYNERGY_ACTION_AT(slots, index) (((u16 *)(slots))[(index) * 2])
#endif

/// Rebuilds a character's Psynergy list: drops the actions its class and items
/// gave, closes up the list, adds every class action learned by its level and
/// the action of each equipped item that grants one, and closes it up again.
/// Writes only the unit's psynergy; enemies (CLASS_NONE) are left
/// alone. Called whenever the class, level or equipment changes:
/// Inventory_Equip, Unit_UpdateClass, Unit_LevelUp, Unit_LearnPsynergy and the
/// item menu.
s32 Unit_RefreshPsynergy(s32 unitId) {
  struct Unit *unit;
  struct PsynergyEntry *abilities;
  struct ClassDefinition *classDefinition;
  struct ItemDefinition *item;
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
  for (sourceIndex = 0; sourceIndex < 32; sourceIndex++) {
    if (PSYNERGY_ACTION_AT(abilities, sourceIndex) & originFlag)
      PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  }
  originFlag = PSYNERGY_FROM_ITEM;
  for (sourceIndex = 0; sourceIndex < 32; sourceIndex++) {
    if (PSYNERGY_ACTION_AT(abilities, sourceIndex) & originFlag)
      PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  }
  // Pack permanent actions at the end to leave room for class abilities.
  for (slot = 31, sourceIndex = 31; slot >= 0;) {
    if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
      slot--;
    } else {
      PSYNERGY_ACTION_AT(abilities, sourceIndex) =
          PSYNERGY_ACTION_AT(abilities, slot);
      slot--;
      sourceIndex--;
    }
  }
  for (; sourceIndex >= 0; sourceIndex--)
    PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;

  // Add level-eligible class abilities, followed by equipped-item abilities.
  originFlag = PSYNERGY_FROM_CLASS;
  for (sourceIndex = 0; sourceIndex < 16; sourceIndex++) {
    classAbility = &classDefinition->abilities[sourceIndex];
    if (classDefinition->abilities[sourceIndex].ability == 0)
      continue;
    if (unit->level < classDefinition->abilities[sourceIndex].level)
      continue;
    slot = 0;
    while (slot < 32 && PSYNERGY_ACTION_AT(abilities, slot) !=
                            classDefinition->abilities[sourceIndex].ability)
      slot++;
    if (slot != 32)
      continue;
    for (slot = 0; slot < 32; slot++) {
      if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
        PSYNERGY_ACTION_AT(abilities, slot) =
            classAbility->ability | originFlag;
        break;
      }
    }
    if (slot == 32)
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
    for (slot = 0; slot < 32; slot++) {
      if ((PSYNERGY_ACTION_AT(abilities, slot) & PSYNERGY_ABILITY_MASK) ==
          item->abilityId)
        break;
    }
    if (slot != 32)
      continue;
    for (slot = 0; slot < 32; slot++) {
      if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
        PSYNERGY_ACTION_AT(abilities, slot) =
            item->abilityId | PSYNERGY_FROM_ITEM;
        break;
      }
    }
    if (slot == 32)
      break;
  }

  // Restore a dense display list with empty slots at the end.
  for (slot = 0, sourceIndex = 0; slot < 32;) {
    if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
      slot++;
    } else {
      PSYNERGY_ACTION_AT(abilities, sourceIndex) =
          PSYNERGY_ACTION_AT(abilities, slot);
      slot++;
      sourceIndex++;
    }
  }
  for (; sourceIndex < 32; sourceIndex++)
    PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  return 0;
}

/// Scratch for Unit_LevelUp, which takes it from EWRAM first
/// (Heap_AllocEwram) and frees it before returning.
struct LevelUpWork {
  s32 classIndex; ///< the unit's class, written and not read again
  s32 level;      ///< the level before this one
  struct CharacterDefinition *growth; ///< the character's growth curves
  u8 unused[0x20];                    ///< never written
};

/// Teaches a character action `value` for good (or makes a class action
/// permanent by clearing its marks), then rebuilds its list. Returns the
/// action's place in the list, or -1 when the list is full. Called by the
/// events that teach Psynergy, the prologue's end, the debug presets and the
/// debug shortcut menu.
s32 Unit_LearnPsynergy(s32 stateIndex, s32 value) {
  struct Unit *state = Unit_Get(stateIndex);
  s32 key = value & PSYNERGY_ABILITY_MASK;
  s32 found = -1;
  s32 index;

  for (index = 0; index <= 30; index++) {
    s32 masked = state->psynergy[index].encodedAction & PSYNERGY_ABILITY_MASK;

    if ((masked ^ key) == 0) {
      state->psynergy[index].encodedAction = masked;
      found = index;
      break;
    }
  }

  if (found < 0) {
    for (index = 0; index <= 30; index++) {
      s32 action = state->psynergy[index].encodedAction;

      if (action == 0) {
        state->psynergy[index].encodedAction = key;
        found = index;
        break;
      }
    }
    if (found < 0) {
      return -1;
    }
  }

  Unit_RefreshPsynergy(stateIndex);
  for (index = 0; index <= 31; index++) {
    if (state->psynergy[index].encodedAction == key) {
      break;
    }
  }
  return index;
}

/// Levels the character up once if it has the experience. Returns `gains`
/// filled with the gains, or NULL. Only GAME.S's bare veneer reaches it;
/// nothing calls it by name.
struct LevelUpResult *Unit_CheckLevelUp(s32 unitId,
                                        struct LevelUpResult *gains) {
  return Unit_TryLevelUp(unitId, gains);
}

/// Returns character `index`'s definition: growth curves, starting level and
/// equipment.
struct CharacterDefinition *Character_GetDefinition(s32 index) {
  struct CharacterDefinition *base;

  base = (struct CharacterDefinition *)gCharacter_DefinitionTable;
  return &base[index];
}

/// Sets up all eight characters for a new game: names from the message table,
/// starting equipment equipped, class and elements worked out, and levels
/// raised to their starting level. Called by
/// MainRoutineWork_InitializeDefaults.
void Party_InitializeCharacters(void) {
  struct Unit *unit;
  struct CharacterDefinition *character;
  u16 nameBuf[16];
  s32 unitId;
  const s32 *startingUnitId = gCharacter_StartingEquipUnitIds;
  s32 i;
  s32 slot;
  u8 *name;

  for (unitId = 0; unitId <= 7; unitId++) {
    unit = Unit_Get(unitId);
    Message_DecodeRawUnboundedFar(unitId + MsgCharacterName, nameBuf);
    name = unit->name;
    name[0] = nameBuf[0];
    i = 0;
    if (nameBuf[0] != 0) {
      do {
        i++;
        if (i > 13)
          break;
        name[i] = nameBuf[i];
      } while (nameBuf[i] != 0);
    }
    unit->name[14] = 0;
  }

  // gCharacter_StartingEquipUnitIds lists the characters given starting
  // equipment, ended by -1 (starting_equipment.c).
  if (*startingUnitId != -1) {
    do {
      unit = Unit_Get(*startingUnitId);
      if (unit != 0) {
        unit->id = *startingUnitId;
        character = Character_GetDefinition(unit->id);

        for (i = 14; i >= 0; i--)
          unit->inventory[i] = 0;

        for (i = 0; i < sizeof(character->items) / sizeof(character->items[0]);
             i++) {
          slot =
              Inventory_AddItem(*startingUnitId, character->items[i] & 0x1ff);
          Inventory_Equip(*startingUnitId, slot);
        }

        Unit_UpdateClass(*startingUnitId);
        unit->ppGauge = 0x4000;
        unit->hpGauge = 0x4000;
        Unit_LevelUpTo(*startingUnitId, character->startingLevel);
        Unit_Calculate(*startingUnitId);
      }
      startingUnitId++;
    } while (*startingUnitId != -1);
  }
}

/// Unused: reserved entry; keeps its place in the far-call table.
UNUSED void Unit_LevelNoOp(void) {}

/// Returns the experience a character needs for `level` (0 for level 0 and
/// below); -1 for an enemy or a level above 99. Called by the levelling code
/// and by the two status screens to show the experience to the next level.
u32 Unit_GetExperienceForLevel(s32 unitId, s32 level) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->classId != CLASS_NONE) {
    if (level <= 0) {
      return 0;
    }
    if (level <= UNIT_LEVEL_MAX && unit->id <= 7) {
      return gCharacter_LevelExpTable[unit->id * UNIT_LEVEL_MAX + level - 1];
    }
  }
  return -1;
}

/// Raises a character one level (up to 99), adding to each base stat the
/// growth between the two points of its growth curve around the new level
/// (one point every 20 levels), a twentieth of it per level with a random
/// part. Writes the unit's level, base stats and, if short, its experience,
/// then rebuilds its Psynergy and stats. The gains go to `gains`, which is
/// returned.
struct LevelUpResult *Unit_LevelUp(s32 unitId, struct LevelUpResult *gains) {
  struct Unit *unit;
  struct LevelUpWork *work;
  u32 requiredExperience;
  s16 growthBand;
  s32 growthDifference;
  s32 level;

  unit = Unit_Get(unitId);
  work = Heap_AllocEwram(sizeof(struct LevelUpWork));
  work->classIndex = unit->classId;
  level = unit->level;
  work->level = level;
  gains->level = level;
  gains->ability = 0xffff;
  gains->hp = 0;
  gains->pp = 0;
  gains->atk = 0;
  gains->def = 0;
  gains->agi = 0;
  gains->luck = 0;
  if (level < UNIT_LEVEL_MAX) {
    unit->level++;
    gains->level = level + 1;
    requiredExperience = Unit_GetExperienceForLevel(unitId, unit->level);
    if (requiredExperience != -1 && unit->experience < requiredExperience)
      unit->experience = requiredExperience;
    work->growth = Character_GetDefinition(unitId);
    // Level 1 starts from the curve's first point.
    if (gains->level == 1) {
      gains->hp += work->growth->hp[0];
      gains->pp += work->growth->pp[0];
      gains->atk += work->growth->atk[0];
      gains->def += work->growth->def[0];
      gains->agi += work->growth->agi[0];
      gains->luck += work->growth->luck[0];
    }
    // Each twenty-level band interpolates between adjacent growth points.
    // Keep one RNG draw per stat, in HP, PP, attack, defense, agility, luck
    // order. Random16() * 20 >> 16 is 0 to 19, so a rising step of d gives
    // d / 20 rounded up with a chance of about (d % 20) / 20: on average a
    // twentieth of the step a level, the whole step over a 20-level band
    // (band 0, levels 1 to 19, gains nineteen twentieths).
    growthBand = gains->level / 20;
    CLAMP(growthBand, 0, 4);
    growthDifference =
        work->growth->hp[growthBand + 1] - work->growth->hp[growthBand];
    gains->hp += ((Random16() * 20 >> 16) + growthDifference) / 20;
    growthDifference =
        work->growth->pp[growthBand + 1] - work->growth->pp[growthBand];
    gains->pp += ((Random16() * 20 >> 16) + growthDifference) / 20;
    growthDifference =
        work->growth->atk[growthBand + 1] - work->growth->atk[growthBand];
    gains->atk += ((Random16() * 20 >> 16) + growthDifference) / 20;
    growthDifference =
        work->growth->def[growthBand + 1] - work->growth->def[growthBand];
    gains->def += ((Random16() * 20 >> 16) + growthDifference) / 20;
    growthDifference =
        work->growth->agi[growthBand + 1] - work->growth->agi[growthBand];
    gains->agi += ((Random16() * 20 >> 16) + growthDifference) / 20;
    growthDifference =
        work->growth->luck[growthBand + 1] - work->growth->luck[growthBand];
    gains->luck += ((Random16() * 20 >> 16) + growthDifference) / 20;
    // Commit the six gains before rebuilding Psynergy and derived stats.
    unit->baseHp += gains->hp;
    unit->basePp += gains->pp;
    unit->baseAttack += gains->atk;
    unit->baseDefense += gains->def;
    unit->baseAgility += gains->agi;
    unit->baseLuck += gains->luck;
    // A character's base is always one action a turn and no regeneration;
    // Unit_Calculate adds equipment's actions and regeneration, Kite's extra
    // action and 4 PP a turn once the character's lighthouse is lit
    unit->baseActionCount = 1;
    unit->baseHpRegen = 0;
    unit->basePpRegen = 0;
    Unit_RefreshPsynergy(unitId);
    Unit_Calculate(unitId);
  }
  Heap_Release(work);
  return gains;
}

/// Levels the character up once if its experience reaches the next level.
/// Returns `gains` filled with the gains, or NULL. Called after a battle by
/// Battle_AwardSpoils, once for each level a party member reaches. At level 99
/// the next level needs -1 (0xffffffff) experience, which none reaches.
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
/// recomputes its stats. Called by Party_InitializeCharacters for the starting
/// levels, and by the debug menus and presets.
void Unit_LevelUpTo(s32 unitId, s32 targetLevel) {
  // Each level's gains are discarded.
  struct LevelUpResult gains;
  s32 currentLevel = Unit_Get(unitId)->level;

  while (currentLevel < targetLevel) {
    Unit_LevelUp(unitId, &gains);
    currentLevel++;
  }
  Unit_Calculate(unitId);
}

#undef PSYNERGY_ACTION_AT
