// Part of game.c, the game object, compiled only through it.
// Actions and Psynergy lists, characters, experience and levelling up.

#include "../game_internal.h"

// Returns the definition of action `actionId` (its low 14 bits); an id
// outside the table gives the first.
struct BattleAct *BattleAction_GetDirect(s32 actionId) {
  u32 entryIndex;

  entryIndex = actionId & OWNER_ACTION_ID_MASK;
  if (entryIndex >= 0x208U) {
    entryIndex = 0;
  }
  return &gBattleAction_DefinitionTable[entryIndex];
}

// Returns 1 when `actionId` is in the unit's action list.
s32 Unit_KnowsAction(s32 owner, s32 actionId) {
  struct BattleUnit *unit = UnitGet(owner);
  struct OwnerActionSlot *entry;
  s32 actionMask = OWNER_ACTION_ID_MASK;
  s32 index = 0;

  entry = unit->actionSlots;
  do {
    s32 currentAction = entry->encodedAction;

    currentAction &= actionMask;
    entry++;
    if (currentAction == actionId) {
      return 1;
    }
    index++;
  } while (index <= 31);
  return 0;
}

// Marks on an action list entry: learned from the class, or granted by an
// equipped item. Plain entries were taught for good.
enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

// Slots are four-byte records, not a flat halfword array. The native view
// preserves load scheduling; the typed view shortens the English object by
// 52 bytes. UBFIX accesses each record through its actual declared type.
#ifdef UBFIX
#define PSYNERGY_ACTION_AT(slots, index) ((slots)[index].encodedAction)
#else
#define PSYNERGY_ACTION_AT(slots, index) (((u16 *)(slots))[(index) * 2])
#endif

// Rebuilds a character's Psynergy list: drops the actions its class and items
// gave, closes up the list, adds every class action learned by its level and
// the action of each equipped item that grants one, and closes it up again.
s32 Unit_RefreshPsynergy(s32 owner) {
  struct BattleUnit *unit;
  struct OwnerActionSlot *abilities;
  struct ClassDefinition *classDefinition;
  struct ItemDefinition *itemDefinition;
  s32 slot;
  s32 sourceIndex;
  u16 originFlag;
  struct ClassAbility *classAbility;

  unit = UnitGet(owner);
  abilities = unit->actionSlots;
  classDefinition = Class_GetDefinition(unit->classIndex);
  if (unit->classIndex == 0)
    return 0;
  // Remove actions inherited from the previous class and equipment.
  originFlag = OWNER_ACTION_CLASS;
  for (sourceIndex = 0; sourceIndex < 32; sourceIndex++) {
    if (PSYNERGY_ACTION_AT(abilities, sourceIndex) & originFlag)
      PSYNERGY_ACTION_AT(abilities, sourceIndex) = 0;
  }
  originFlag = OWNER_ACTION_ITEM;
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
  originFlag = OWNER_ACTION_CLASS;
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
    itemDefinition = Item_GetDirect(unit->inventory[sourceIndex]);
    if (itemDefinition->useType != 3)
      continue;
    for (slot = 0; slot < 32; slot++) {
      if ((PSYNERGY_ACTION_AT(abilities, slot) & OWNER_ACTION_ID_MASK) ==
          itemDefinition->actionId)
        break;
    }
    if (slot != 32)
      continue;
    for (slot = 0; slot < 32; slot++) {
      if (PSYNERGY_ACTION_AT(abilities, slot) == 0) {
        PSYNERGY_ACTION_AT(abilities, slot) =
            itemDefinition->actionId | OWNER_ACTION_ITEM;
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

// Scratch for Owner_LevelUp.
struct LevelUpWork {
  s32 classIndex;
  s32 level;
  struct CharacterDefinition *growth;
  u8 unk0c[0x20];
};

// Teaches a character action `value` for good (or makes a class action
// permanent), then rebuilds its list. Returns the action's place in the list,
// or -1 when the list is full.
s32 Unit_LearnPsynergy(s32 stateIndex, s32 value) {
  struct BattleUnit *state = UnitGet(stateIndex);
  s32 key = value & OWNER_ACTION_ID_MASK;
  s32 found = -1;
  s32 index;

  for (index = 0; index <= 30; index++) {
    s32 masked = state->actionSlots[index].encodedAction & OWNER_ACTION_ID_MASK;

    if ((masked ^ key) == 0) {
      state->actionSlots[index].encodedAction = masked;
      found = index;
      break;
    }
  }

  if (found < 0) {
    /* FAKEMATCH: the scalar member-address walk: the slot offset is index * 4 +
     * 88 from state, recomputed each pass as the bytes do. Plain forms measured
     * 2026-10-13: state->actionSlots[index].encodedAction differ 22 (75
     * instructions, loop.c walks the slot address); the slot through a u16
     * pointer, a struct OwnerActionSlot pointer or a u16 view of the array
     * differ 22 (79 instructions). */
    for (index = 0; index <= 30; index++) {
      s32 offset = index * sizeof(state->actionSlots[0]) +
                   (s32)&((struct BattleUnit *)0)->actionSlots;

      if (*(u16 *)((u8 *)state + offset) == 0) {
        *(u16 *)((u8 *)state + offset) = key;
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
    if (state->actionSlots[index].encodedAction == key) {
      break;
    }
  }
  return index;
}

// Levels the character up once if it has the experience. Returns `gains`
// filled with the gains, or NULL.
struct LevelUpResult *Unit_CheckLevelUp(s32 owner,
                                        struct LevelUpResult *gains) {
  return Unit_TryLevelUp(owner, gains);
}

// Returns character `index`'s definition: growth curves, starting level and
// equipment.
struct CharacterDefinition *Character_GetDefinition(s32 index) {
  struct CharacterDefinition *base;

  base = gCharacter_DefinitionTable;
  return &base[index];
}

// Sets up all eight characters for a new game: names from the message table,
// starting equipment equipped, class and elements worked out, and levels
// raised to their starting level.
void Party_InitCharacters(void) {
  struct BattleUnit *unit;
  struct CharacterDefinition *character;
  u16 nameBuf[16];
  s32 owner;
  const s32 *startingOwner = gCharacter_StartingEquipOwnerIds;
  s32 i;
  s32 slot;
  u8 *name;

  for (owner = 0; owner <= 7; owner++) {
    unit = UnitGet(owner);
    Ui_AdjustValueWithoutLimitFar(owner + MsgCharacterName, nameBuf);
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

  if (*startingOwner != -1) {
    do {
      unit = UnitGet(*startingOwner);
      if (unit != 0) {
        unit->classId = *startingOwner;
        character = Character_GetDefinition(unit->classId);

        for (i = 14; i >= 0; i--)
          unit->inventory[i] = 0;

        for (i = 0; i < sizeof(character->items) / sizeof(character->items[0]);
             i++) {
          slot = Inventory_AddItem(*startingOwner, character->items[i] & 0x1ff);
          Inventory_Equip(*startingOwner, slot);
        }

        Unit_UpdateClass(*startingOwner);
        unit->ppGauge = 0x4000;
        unit->hpGauge = 0x4000;
        Unit_LevelUpTo(*startingOwner, character->startingLevel);
        UnitCalc(*startingOwner);
      }
      startingOwner++;
    } while (*startingOwner != -1);
  }
}

// Unused reserved entry; keep its place in the far-call table.
UNUSED void Owner_LevelNoOp(void) {}

// Returns the experience a character needs for `level` (0 for level 0 and
// below); -1 for an enemy or a level above 99.
u32 Unit_GetExperienceForLevel(s32 owner, s32 level) {
  struct BattleUnit *unit = UnitGet(owner);

  if (unit->classIndex != 0) {
    if (level <= 0) {
      return 0;
    }
    if (level <= 99 && unit->classId <= 7) {
      return gCharacter_LevelExpTable[unit->classId * 99 + level - 1];
    }
  }
  return -1;
}

// Raises a character one level (up to 99), adding to each base stat the
// growth between the two points of its growth curve around the new level
// (one point every 20 levels), a twentieth of it per level with a random
// part. The gains go to `gains`, which is returned.
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *gains) {
  struct BattleUnit *unit;
  struct LevelUpWork *work;
  u32 requiredExperience;
  s16 growthBand;
  s32 growthDifference;
  s32 level;

  unit = UnitGet(owner);
  work = Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
  work->classIndex = unit->classIndex;
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
  if (level < 99) {
    unit->level++;
    gains->level = level + 1;
    requiredExperience = Unit_GetExperienceForLevel(owner, unit->level);
    if (requiredExperience != -1 && unit->experience < requiredExperience)
      unit->experience = requiredExperience;
    work->growth = Character_GetDefinition(owner);
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
    // order.
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
    unit->baseActionCount = 1;
    unit->baseHpRegen = 0;
    unit->basePpRegen = 0;
    Unit_RefreshPsynergy(owner);
    UnitCalc(owner);
  }
  MemFree(work);
  return gains;
}

// Levels the character up once if its experience reaches the next level.
// Returns `gains` filled with the gains, or NULL.
struct LevelUpResult *Unit_TryLevelUp(s32 owner, struct LevelUpResult *gains) {
  struct BattleUnit *unit;

  unit = UnitGet(owner);
  if ((unit->experience >=
       Unit_GetExperienceForLevel(owner, unit->level + 1)) &&
      (Owner_LevelUp(owner, gains) != NULL)) {
    return gains;
  }
  return 0;
}

// Levels character `owner` up one level at a time until `targetLevel`, then
// recomputes its stats.
void Unit_LevelUpTo(s32 owner, s32 targetLevel) {
  // Each level's gains are discarded.
  struct LevelUpResult gains;
  s32 currentLevel = UnitGet(owner)->level;

  while (currentLevel < targetLevel) {
    Owner_LevelUp(owner, &gains);
    currentLevel++;
  }
  UnitCalc(owner);
}

#undef PSYNERGY_ACTION_AT
