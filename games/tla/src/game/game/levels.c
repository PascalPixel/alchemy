/// @file
/// Actions and Psynergy lists, characters, experience and levelling up.
// Part of game.c.

#include "../game_internal.h"
#include "game/character.h"
#include "module/game/game.h"
#include "text/msg_ids.h"

/// Returns the definition of ability `abilityId` (its low 14 bits); an id
/// past the table gives the first. Every bank reads an ability through its far
/// call, Ability_Get.
struct AbilityDefinition *Ability_GetDirect(s32 abilityId) {
  u32 entryIndex = abilityId & OWNER_ACTION_ID_MASK;

  // BUG: id ABILITY_COUNT, one past the table, is let through.
#ifdef BUGFIX
  if (entryIndex >= ABILITY_COUNT)
#else
  if (entryIndex > ABILITY_COUNT)
#endif
    entryIndex = 0;
  return (struct AbilityDefinition *)&gAbilityDefinitions[entryIndex];
}

s32 Party_FindOwnerWithAction(s32 action) {
  s16 owners[10];
  s32 count = Party_ListMembers(owners);
  s32 i;

  for (i = 0; i < count; i++) {
    if (Unit_KnowsPsynergy(owners[i], action) != 0)
      break;
  }
  if (i == count)
    return -1;
  return owners[i];
}

/// Returns 1 when `abilityId` is in the unit's Psynergy list. Called by the
/// events that check whether a party member knows a Psynergy
/// (Party_CheckShortcut).
s32 Unit_KnowsPsynergy(s32 owner, s32 value) {
  struct BattleUnit *unit = Unit_Get(owner);
  struct OwnerActionSlot *entry;
  s32 index = 0;
  s32 mask = OWNER_ACTION_ID_MASK;

  entry = unit->psynergy;
  do {
    s32 current = entry->encodedAction;

    entry++;
    if ((current & mask) == value)
      return 1;
    index++;
  } while (index <= PSYNERGY_SLOTS - 1);
  return 0;
}

/// Rebuilds a character's Psynergy list: what it learned for good stays, in
/// order, then come its class's Psynergy up to its level and its equipped
/// items' Psynergy, while the list has room.
s32 Owner_UpdateClassActions(s32 owner) {
  struct BattleUnit *st;
  u16 *ability;
  struct ClassDefinition *cls;
  struct ItemDefinition *def;
  s32 i;
  s32 j;
  u16 mask;
  struct ClassAbility *cg;

  st = Unit_Get(owner);
  ability = (u16 *)st->psynergy;
  cls = Class_GetDefinition(st->classId);
  if (st->classId == CLASS_NONE)
    return 0;
  mask = OWNER_ACTION_CLASS | OWNER_ACTION_ITEM;
  for (j = 0; j < PSYNERGY_SLOTS; j++) {
    if (ability[j * 2] & mask)
      ability[j * 2] = 0;
  }
  for (i = PSYNERGY_SLOTS - 1, j = PSYNERGY_SLOTS - 1; i >= 0;) {
    if (ability[i * 2] == 0) {
      i--;
    } else {
      ability[j * 2] = ability[i * 2];
      i--;
      j--;
    }
  }
  for (; j >= 0; j--)
    ability[j * 2] = 0;

  mask = OWNER_ACTION_CLASS;
  for (j = 0; j < CLASS_ABILITY_SLOTS; j++) {
    cg = &cls->abilities[j];
    if (cls->abilities[j].ability == 0)
      continue;
    if (st->level < cls->abilities[j].level)
      continue;
    i = 0;
    while (i < PSYNERGY_SLOTS && ability[i * 2] != cls->abilities[j].ability)
      i++;
    if (i != PSYNERGY_SLOTS)
      continue;
    for (i = 0; i < PSYNERGY_SLOTS; i++) {
      if (ability[i * 2] == 0) {
        ability[i * 2] = cg->ability | OWNER_ACTION_CLASS;
        break;
      }
    }
    if (i == PSYNERGY_SLOTS)
      break;
  }

  for (j = 0; j < INVENTORY_SLOTS; j++) {
    if (st->inventory[j] == 0)
      continue;
    if (!(st->inventory[j] & INVENTORY_EQUIPPED))
      continue;
    def = Item_GetDirect(st->inventory[j]);
    if (def->useType != ITEM_USE_TEACHES)
      continue;
    for (i = 0; i < PSYNERGY_SLOTS; i++) {
      if ((ability[i * 2] & OWNER_ACTION_ID_MASK) == def->abilityId)
        break;
    }
    if (i != PSYNERGY_SLOTS)
      continue;
    for (i = 0; i < PSYNERGY_SLOTS; i++) {
      if (ability[i * 2] == 0) {
        ability[i * 2] = def->abilityId | OWNER_ACTION_ITEM;
        break;
      }
    }
    if (i == PSYNERGY_SLOTS)
      break;
  }

  for (i = 0, j = 0; i < PSYNERGY_SLOTS;) {
    if (ability[i * 2] == 0) {
      i++;
    } else {
      ability[j * 2] = ability[i * 2];
      i++;
      j++;
    }
  }
  for (; j < PSYNERGY_SLOTS; j++)
    ability[j * 2] = 0;
  return 0;
}

/// Teaches unit `stateIndex` the Psynergy `value` for good, then rebuilds its
/// list; returns the slot it lands in, or -1 when the list is full.
s32 Unit_LearnPsynergy(s32 stateIndex, s32 value) {
  struct BattleUnit *state = Unit_Get(stateIndex);
  s32 key = value & OWNER_ACTION_ID_MASK;
  s32 found = -1;
  s32 index;

  for (index = 0; index <= 30; index++) {
    s32 masked = state->psynergy[index].encodedAction & OWNER_ACTION_ID_MASK;

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
  for (index = 0; index < PSYNERGY_SLOTS; index++) {
    if (state->psynergy[index].encodedAction == key) {
      break;
    }
  }
  return index;
}

struct LevelUpResult;

/// Levels the character up once if it has the experience, and returns gains, or
/// NULL.
struct LevelUpResult *Unit_CheckLevelUp(s32 unitId,
                                        struct LevelUpResult *gains) {
  return Unit_TryLevelUp(unitId, gains);
}

/// Returns character `index`'s definition: growth curves, starting level and
/// equipment.
const struct CharacterDefinition *Character_GetDefinition(s32 index) {
  const struct CharacterDefinition *base;

  base = gCharacter_DefinitionTable;
  return &base[index];
}

void Party_InitializeCharacters(void) {
  struct BattleUnit *unit;
  const struct CharacterDefinition *character;
  u16 nameBuf[16];
  s32 unitId;
  const s32 *startingUnitId = gCharacter_StartingEquipUnitIds;
  s32 i;
  s32 slot;
  u8 *name;

  for (unitId = 0; unitId <= PIERS; unitId++) {
    unit = Unit_Get(unitId);
    Message_DecodeRawUnboundedFar(unitId + (s32)MsgCharacterName, nameBuf);
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
  // equipment, ended by -1 (artifact_slots.c).
  if (*startingUnitId != -1) {
    do {
      unit = Unit_Get(*startingUnitId);
      if (unit != 0) {
        unit->enemyId = *startingUnitId;
        character = Character_GetDefinition(unit->enemyId);
        for (i = INVENTORY_SLOTS - 1; i >= 0; i--)
          unit->inventory[i] = 0;

        for (i = 0; i < sizeof(character->items) / sizeof(character->items[0]);
             i++) {
          slot = Inventory_AddItem(*startingUnitId, character->items[i] &
                                                        INVENTORY_ITEM_ID_MASK);
          Inventory_Equip(*startingUnitId, slot);
        }

        Unit_UpdateClass(*startingUnitId);
        unit->ppGauge = UNIT_GAUGE_FULL;
        unit->hpGauge = UNIT_GAUGE_FULL;
        Unit_LevelUpTo(*startingUnitId, character->startingLevel);
        Unit_UpdateClass(*startingUnitId);
        Unit_Calculate(*startingUnitId);
      }
      startingUnitId++;
    } while (*startingUnitId != -1);
  }
}

/// Unused: reserved entry; keeps its place in the far-call table.
UNUSED void Unit_LevelNoOp(void) {}

/// Returns the experience a character needs for `level`: 0 below level 1, and
/// -1 for an enemy or a level past UNIT_LEVEL_MAX.
u32 Unit_GetExperienceForLevel(s32 unitId, s32 level) {
  struct BattleUnit *unit = Unit_Get(unitId);

  if (unit->classId != CLASS_NONE) {
    if (level <= 0) {
      return 0;
    }
    if (level <= UNIT_LEVEL_MAX && unit->enemyId <= PIERS) {
      return gCharacter_LevelExpTable[unit->enemyId * UNIT_LEVEL_MAX + level -
                                      1];
    }
  }
  return -1;
}

u32 __udivsi3(u32 numerator, u32 denominator);

/// Raises a character one level (up to 99), adding to each base stat the
/// growth between the two points of its growth curve around the new level
/// (one point every 20 levels), a twentieth of it per level with a random
/// part. Writes the unit's level, base stats and, if short, its experience,
/// then rebuilds its Psynergy and stats. The gains go to `gains`, which is
/// returned.
/// Raises a character one level, up to 99, each base stat growing along its
/// curve with a random part. Returns gains.
struct LevelUpResult *Unit_LevelUp(s32 unitId, struct LevelUpResult *gains) {
  struct BattleUnit *unit;
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
    if (requiredExperience != (u32)-1 && unit->experience < requiredExperience)
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
    // Each twenty-level band grows by the step between its growth points, a
    // twentieth a level with a random part; one roll per stat, in order.
    growthBand = __divsi3(gains->level, GROWTH_BAND_LEVELS);
    if (growthBand < 0)
      growthBand = 0;
    if (growthBand > GROWTH_POINTS - 2)
      growthBand = GROWTH_POINTS - 2;
    growthDifference =
        work->growth->hp[growthBand + 1] - work->growth->hp[growthBand];
    gains->hp +=
        __udivsi3((Random16() * GROWTH_BAND_LEVELS >> 16) + growthDifference,
                  GROWTH_BAND_LEVELS);
    growthDifference =
        work->growth->pp[growthBand + 1] - work->growth->pp[growthBand];
    gains->pp +=
        __udivsi3((Random16() * GROWTH_BAND_LEVELS >> 16) + growthDifference,
                  GROWTH_BAND_LEVELS);
    growthDifference =
        work->growth->atk[growthBand + 1] - work->growth->atk[growthBand];
    gains->atk +=
        __udivsi3((Random16() * GROWTH_BAND_LEVELS >> 16) + growthDifference,
                  GROWTH_BAND_LEVELS);
    growthDifference =
        work->growth->def[growthBand + 1] - work->growth->def[growthBand];
    gains->def +=
        __udivsi3((Random16() * GROWTH_BAND_LEVELS >> 16) + growthDifference,
                  GROWTH_BAND_LEVELS);
    growthDifference =
        work->growth->agi[growthBand + 1] - work->growth->agi[growthBand];
    gains->agi +=
        __udivsi3((Random16() * GROWTH_BAND_LEVELS >> 16) + growthDifference,
                  GROWTH_BAND_LEVELS);
    growthDifference =
        work->growth->luck[growthBand + 1] - work->growth->luck[growthBand];
    gains->luck +=
        __udivsi3((Random16() * GROWTH_BAND_LEVELS >> 16) + growthDifference,
                  GROWTH_BAND_LEVELS);
    // Commit the six gains before rebuilding Psynergy and derived stats.
    unit->baseHp += gains->hp;
    unit->basePp += gains->pp;
    unit->baseAttack += gains->atk;
    unit->baseDefense += gains->def;
    unit->baseAgility += gains->agi;
    unit->baseLuck += gains->luck;
    // A character's base is one action a turn and no regeneration.
    unit->baseActionCount = 1;
    unit->baseHpRegen = 0;
    unit->basePpRegen = 0;
    ((s32 (*)(s32))Unit_RefreshPsynergy)(unitId);
    Unit_Calculate(unitId);
  }
  Heap_Release(work);
  return gains;
}

/// Levels the character up once if its experience reaches the next level, and
/// returns gains, or NULL.
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains) {
  struct BattleUnit *unit;

  unit = Unit_Get(unitId);
  if ((unit->experience >=
       Unit_GetExperienceForLevel(unitId, unit->level + 1)) &&
      (Unit_LevelUp(unitId, gains) != NULL)) {
    return gains;
  }
  return 0;
}

/// Level up one level at a time until the target, recalculating stats each
/// time. unit: character ID. lv: target level.
void Unit_LevelUpTo(s32 unit, s32 lv) {
  u8 res[16]; // Receives struct LevelUpResult (unused).
  s32 cur = ((struct BattleUnit *)Unit_Get(unit))->level;

  while (cur < lv) {
    Unit_LevelUp(unit, (struct LevelUpResult *)res);
    cur++;
  }
  Unit_Calculate(unit);
}
