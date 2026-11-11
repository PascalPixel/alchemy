/// @file
/// Actions and Psynergy lists, characters, experience and levelling up.
// Part of game.c.

#include "../game_internal.h"

s32 Unit_KnowsAction(s32 owner, s32 value);

struct AbilityDefinition *BattleAction_GetDirect(s32 actionId) {
  u32 entryIndex = actionId & 0x3fff;

  if (entryIndex > 0x2de)
    entryIndex = 0;
  return (struct AbilityDefinition *)&gBattleAction_DefinitionTable[entryIndex];
}

s32 Party_FindOwnerWithAction(s32 action) {
  s16 owners[10];
  s32 count = Party_ListActiveOwners(owners);
  s32 i;

  for (i = 0; i < count; i++) {
    if (Unit_KnowsAction(owners[i], action) != 0)
      break;
  }
  if (i == count)
    return -1;
  return owners[i];
}

s32 Unit_KnowsAction(s32 owner, s32 value) {
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

struct ClassAbilities {
  u8 unk00[0x10];
  struct ClassAbility abilities[16];
};

enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

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
    /* FAKEMATCH(temporary): each slot's address walked as a byte offset of its
     * own, which keeps the halfword operands and the four-byte-longer free-slot
     * loop; as state->actionSlots[index].encodedAction, 20 of 77 instructions
     * differ in tla-en. */
    for (index = 0; index <= 30; index++) {
      s32 offset = index * (s32)sizeof(state->actionSlots[0]) +
                   (s32)&((struct OwnerActionState *)0)->actionSlots;

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

// Called with one argument, as its bytes need: no prototype is in scope.
struct LevelUpResult *Func_080afb80();

void Func_080af794(s32 owner) { Func_080afb80(owner); }

struct OwnerGrowthRecord {
  u8 unk00[0x50];
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
  u8 unk92[0x22];
};

extern struct OwnerGrowthRecord gOwner_GrowthRecords[8];

struct OwnerGrowthRecord *Character_GetDefinition(s32 index) {
  s32 stride = sizeof(struct OwnerGrowthRecord);
  u8 *base;

  /* FAKEMATCH(barrier): the row width used by an empty asm, so it is built
   * before the table's address is loaded from the pool; as no asm, 2 of 5
   * instructions differ in tla-en. */
  asm("" : : "r"(stride));
  base = (u8 *)gOwner_GrowthRecords;
  /* FAKEMATCH(barrier): the row width and the table held at an empty asm, so
   * both are live before the row offset is multiplied; as no asm, 2 of 5
   * instructions differ in tla-en. */
  asm("" : "+r"(stride), "+r"(base));
  /* FAKEMATCH(signature): the address summed as an integer, which keeps the
   * multiply's and the add's lifetimes; as pointer addition, base + index *
   * stride, 7 of 5 instructions differ in tla-en (the draft has 6 and shares
   * 2). */
  return (struct OwnerGrowthRecord *)(index * stride + (u32)base);
}

struct OwnerStart {
  u8 unk00[150];
  u8 level;
  u8 unk97;
  u16 items[13];
};

struct OwnerName {
  u8 name[14];
  u8 nameFlags;
  u8 unk0f[5];
  u16 hpRatio;
  u16 ppRatio;
};

extern u8 MsgCharacterName[];
extern s32 gData_080b2340[];

s32 Inventory_AddItem(s32 owner, s32 item);

void Message_DecodeRawUnboundedFar(s32, u16 *);
void Unit_Calculate(s32 owner);

void Func_080af7ac(void) {
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

void Func_080af8cc(void) {}

extern u32 gOwner_ExperienceThresholds[];

/// Experience required per level: eight characters, 99 levels each.
u32 Unit_GetExperienceForLevel(s32 owner, s32 level) {
  struct BattleUnit *state = (struct BattleUnit *)Unit_Get(owner);

  if (state->classIndex != 0) {
    if (level <= 0) {
      return 0;
    }
    if (level <= 99 && state->enemyId <= 7) {
      u32 *table = gOwner_ExperienceThresholds;
      s32 row = state->enemyId * 99 + level;
      s32 offset;

      /* FAKEMATCH(barrier): the table held at an empty asm, which loads it
       * between the add and the scaling; as
       * gOwner_ExperienceThresholds[state->enemyId * 99 + level - 1], 2
       * instructions differ in tla-en. */
      asm volatile("" : "+l"(table));
      offset = row * 4 - 4;
      return *(u32 *)((u8 *)table + offset);
    }
  }
  return -1;
}

struct OwnerLevelState {
  u8 unk000[0x0f];
  u8 level;
  u16 baseHp;
  u16 basePp;
  u8 unk014[4];
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 baseTurns;
  u8 base20;
  u8 base21;
  u8 unk022[0x102];
  u32 experience;
  u8 character;
  u8 classId;
};

struct OwnerGrowth {
  u8 unk00[0x50];
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
};

struct LevelUpResult {
  s16 level;
  u16 ability;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u16 luck;
};

struct LevelUpWork {
  s32 classId;
  s32 level;
  struct OwnerGrowth *growth;
  u8 unused0c[0x20];
};

void Heap_Release(void *buffer);
u32 Unit_GetExperienceForLevel(s32 owner, s32 level);

void Unit_Calculate(s32 owner);
u32 Random16(void);
s32 __divsi3(s32 numerator, s32 denominator);
u32 __udivsi3(u32 numerator, u32 denominator);

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res) {
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
    // Called as returning a value, where the definition returns nothing: the
    // stores before it are scheduled as they are only then.
    ((s32 (*)(s32))Unit_RefreshPsynergy)(owner);
    Unit_Calculate(owner);
  }
  Heap_Release(work);
  return res;
}

struct LevelUpResult;
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *result);
void Unit_Calculate(s32);
u32 Unit_GetExperienceForLevel(s32 owner, s32 level);

/// Levels the owner up once its experience reaches the next level's threshold;
/// returns the result record then, and 0 otherwise.
struct LevelUpResult *Func_080afb80(s32 owner_id, struct LevelUpResult *value) {
  struct BattleUnit *owner;

  owner = (struct BattleUnit *)Unit_Get(owner_id);
  /* FAKEMATCH(barrier): an empty asm, which loads the level before the
   * experience field's offset is started; as no asm, the offset comes first
   * (one reordered pair) and 2 instructions differ. */
  asm("");
  if ((owner->experience >=
       Unit_GetExperienceForLevel(owner_id, owner->level + 1)) &&
      (Owner_LevelUp(owner_id, value) != NULL)) {
    return value;
  }
  return 0;
}

/// Level up one level at a time until the target, recalculating stats each time.
/// unit: character ID.
/// lv: target level.
void Unit_LevelUpTo(s32 unit, s32 lv) {
  u8 res[16]; // Receives struct LevelUpResult (unused).
  s32 cur = ((struct BattleUnit *)Unit_Get(unit))->level;

  while (cur < lv) {
    Owner_LevelUp(unit, (struct LevelUpResult *)res);
    cur++;
  }
  Unit_Calculate(unit);
}
