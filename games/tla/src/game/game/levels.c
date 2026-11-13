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
  u32 entryIndex = abilityId & 0x3fff;

  if (entryIndex > 0x2de)
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

  entry = unit->actionSlots;
  do {
    s32 current = entry->encodedAction;

    entry++;
    if ((current & mask) == value)
      return 1;
    index++;
  } while (index <= 31);
  return 0;
}

s32 Owner_UpdateClassActions(s32 owner) {
  struct BattleUnit *st;
  u16 *ability;
  struct ClassAbilities *cls;
  struct ItemDefinition *def;
  s32 i;
  s32 j;
  u16 mask;
  struct ClassAbility *cg;

  st = Unit_Get(owner);
  ability = (u16 *)st->actionSlots;
  cls = (struct ClassAbilities *)Class_GetDefinition(st->classIndex);
  if (st->classIndex == 0)
    return 0;
  mask = OWNER_ACTION_CLASS | OWNER_ACTION_ITEM;
  for (j = 0; j < 32; j++) {
    if (ability[j * 2] & mask)
      ability[j * 2] = 0;
  }
  for (i = 31, j = 31; i >= 0;) {
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
  for (j = 0; j < 16; j++) {
    cg = &cls->abilities[j];
    if (cls->abilities[j].ability == 0)
      continue;
    if (st->level < cls->abilities[j].level)
      continue;
    i = 0;
    while (i < 32 && ability[i * 2] != cls->abilities[j].ability)
      i++;
    if (i != 32)
      continue;
    for (i = 0; i < 32; i++) {
      if (ability[i * 2] == 0) {
        ability[i * 2] = cg->ability | OWNER_ACTION_CLASS;
        break;
      }
    }
    if (i == 32)
      break;
  }

  for (j = 0; j < 15; j++) {
    if (st->inventory[j] == 0)
      continue;
    if (!(st->inventory[j] & INVENTORY_EQUIPPED))
      continue;
    def = Item_GetDirect(st->inventory[j]);
    if (def->useType != 3)
      continue;
    for (i = 0; i < 32; i++) {
      if ((ability[i * 2] & OWNER_ACTION_ID_MASK) == def->actionId)
        break;
    }
    if (i != 32)
      continue;
    for (i = 0; i < 32; i++) {
      if (ability[i * 2] == 0) {
        ability[i * 2] = def->actionId | OWNER_ACTION_ITEM;
        break;
      }
    }
    if (i == 32)
      break;
  }

  for (i = 0, j = 0; i < 32;) {
    if (ability[i * 2] == 0) {
      i++;
    } else {
      ability[j * 2] = ability[i * 2];
      i++;
      j++;
    }
  }
  for (; j < 32; j++)
    ability[j * 2] = 0;
  return 0;
}

s32 Unit_LearnPsynergy(s32 stateIndex, s32 value) {
  struct OwnerActionState *state =
      (struct OwnerActionState *)Unit_Get(stateIndex);
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
    for (index = 0; index <= 30; index++) {
      s32 action = state->actionSlots[index].encodedAction;

      if (action == 0) {
        state->actionSlots[index].encodedAction = key;
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

struct LevelUpResult;

/// Levels the character up once if it has the experience. Returns `gains`
/// filled with the gains, or NULL. Only a far-call veneer reaches it.
struct LevelUpResult *Unit_CheckLevelUp(s32 unitId,
                                        struct LevelUpResult *gains) {
  return Unit_TryLevelUp(unitId, gains);
}

/* VIEW(game): variable gCharacter_DefinitionTable as struct OwnerGrowthRecord
 * gCharacter_DefinitionTable[8]; game/character_table has const struct
 * CharacterDefinition gCharacter_DefinitionTable[] = {; tried as the
 * definition's extern const struct CharacterDefinition
 * gCharacter_DefinitionTable[], it does not compile in tla-ja:
 * games/tla/src/game/game.c:2199: `gCharacter_DefinitionTable' undeclared
 * (first use in this function). */
extern struct OwnerGrowthRecord
    gCharacter_DefinitionTable__game[8] __asm__("gCharacter_DefinitionTable");
#define gCharacter_DefinitionTable gCharacter_DefinitionTable__game

struct OwnerGrowthRecord *Character_GetDefinition(s32 index) {
  s32 stride = sizeof(struct OwnerGrowthRecord);
  u8 *base;

  /* FAKEMATCH(barrier): the row width used by an empty asm. */
  asm("" : : "r"(stride));
  base = (u8 *)gCharacter_DefinitionTable;
  /* FAKEMATCH(barrier): the row width and the table held at an empty asm. */
  asm("" : "+r"(stride), "+r"(base));
  /* FAKEMATCH(signature): the address summed as an integer. */
  return (struct OwnerGrowthRecord *)(index * stride + (u32)base);
}

/* VIEW(game): variable gData_080b2340 as s32 gData_080b2340[];
 * game/artifact_slots has const s32 gData_080b2340[9] = {0, 1, 2, 3, 4, 5, 6,
 * 7, -1}; tried as the definition's extern const s32 gData_080b2340[9], it does
 * not compile in tla-ja: games/tla/src/game/game.c:2221: `gData_080b2340'
 * undeclared (first use in this function). */
extern s32 gData_080b2340__game[] __asm__("gData_080b2340");
#define gData_080b2340 gData_080b2340__game

void Party_InitializeCharacters(void) {
  struct OwnerInventoryState *state;
  struct OwnerStart *start;
  u16 nameBuf[16];
  s32 owner;
  s32 *remote = gData_080b2340;
  s32 i;
  s32 slot;
  u8 *name;

  for (owner = 0; owner <= 7; owner++) {
    state = Unit_Get(owner);
    Message_DecodeRawUnboundedFar(owner + (s32)MsgCharacterName, nameBuf);
    name = ((struct OwnerName *)state)->name;
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
    ((struct OwnerName *)state)->nameFlags = 0;
  }

  if (*remote != -1) {
    do {
      state = Unit_Get(*remote);
      if (state != 0) {
        state->classId = *remote;
        start = (struct OwnerStart *)Character_GetDefinition(state->classId);
        for (i = 14; i >= 0; i--)
          state->inventory[i] = 0;

        for (i = 0; i < sizeof(start->items) / sizeof(start->items[0]); i++) {
          slot = Inventory_AddItem(*remote, start->items[i] & 0x1ff);
          Inventory_Equip(*remote, slot);
        }

        Unit_UpdateClass(*remote);
        ((struct OwnerName *)state)->ppRatio = 0x4000;
        ((struct OwnerName *)state)->hpRatio = 0x4000;
        Unit_LevelUpTo(*remote, start->level);
        Unit_UpdateClass(*remote);
        Unit_Calculate(*remote);
      }
      remote++;
    } while (*remote != -1);
  }
}

/// Unused: reserved entry; keeps its place in the far-call table.
void Unit_LevelNoOp(void) {}

/* VIEW(game): variable gCharacter_LevelExpTable as u32
 * gCharacter_LevelExpTable[]; game/experience_table has const u32
 * gCharacter_LevelExpTable[] = {; tried as the definition's extern const u32
 * gCharacter_LevelExpTable[], it does not compile in tla-ja:
 * games/tla/src/game/game.c:2285: `gCharacter_LevelExpTable' undeclared (first
 * use in this function). */
extern u32 gCharacter_LevelExpTable__game[] __asm__("gCharacter_LevelExpTable");
#define gCharacter_LevelExpTable gCharacter_LevelExpTable__game

/// Experience required per level: eight characters, 99 levels each.
u32 Unit_GetExperienceForLevel(s32 owner, s32 level) {
  struct BattleUnit *state = (struct BattleUnit *)Unit_Get(owner);

  if (state->classIndex != 0) {
    if (level <= 0) {
      return 0;
    }
    if (level <= 99 && state->enemyId <= 7) {
      u32 *table = gCharacter_LevelExpTable;
      s32 row = state->enemyId * 99 + level;
      s32 offset;

      offset = row * 4 - 4;
      return *(u32 *)((u8 *)table + offset);
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
struct LevelUpResult *Unit_LevelUp(s32 owner, struct LevelUpResult *res) {
  struct OwnerLevelState *st;
  struct LevelUpWork *work;
  u32 threshold;
  s16 band;
  s32 diff;
  s32 level;

  st = Unit_Get(owner);
  work = Heap_AllocEwram(sizeof(struct LevelUpWork));
  work->classId = st->classId;
  level = st->level;
  work->level = level;
  res->level = level;
  res->ability = 0xffff;
  res->hp = 0;
  res->pp = 0;
  res->atk = 0;
  res->def = 0;
  res->agi = 0;
  res->luck = 0;
  if (level < 99) {
    st->level++;
    res->level = level + 1;
    threshold = Unit_GetExperienceForLevel(owner, st->level);
    if (threshold != (u32)-1 && st->experience < threshold)
      st->experience = threshold;
    work->growth = (struct OwnerGrowth *)Character_GetDefinition(owner);
    if (res->level == 1) {
      res->hp += work->growth->hp[0];
      res->pp += work->growth->pp[0];
      res->atk += work->growth->atk[0];
      res->def += work->growth->def[0];
      res->agi += work->growth->agi[0];
      res->luck += work->growth->luck[0];
    }
    band = __divsi3(res->level, 20);
    if (band < 0)
      band = 0;
    if (band > 4)
      band = 4;
    diff = work->growth->hp[band + 1] - work->growth->hp[band];
    res->hp += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->pp[band + 1] - work->growth->pp[band];
    res->pp += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->atk[band + 1] - work->growth->atk[band];
    res->atk += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->def[band + 1] - work->growth->def[band];
    res->def += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->agi[band + 1] - work->growth->agi[band];
    res->agi += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->luck[band + 1] - work->growth->luck[band];
    res->luck += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    st->baseHp += res->hp;
    st->basePp += res->pp;
    st->baseAttack += res->atk;
    st->baseDefense += res->def;
    st->baseAgility += res->agi;
    st->baseLuck += res->luck;
    st->baseTurns = 1;
    st->base20 = 0;
    st->base21 = 0;
    ((s32 (*)(s32))Unit_RefreshPsynergy)(owner);
    Unit_Calculate(owner);
  }
  Heap_Release(work);
  return res;
}

/// Levels the character up once if its experience reaches the next level.
/// Returns `gains` filled with the gains, or NULL. Called after a battle by
/// Battle_AwardSpoils, once for each level a party member reaches.
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains) {
  struct BattleUnit *unit;

  unit = (struct BattleUnit *)Unit_Get(unitId);
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
