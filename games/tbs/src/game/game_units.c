/// @file
/// The party's units and their stats, a new game, the prologue, HP and PP.
// Part of game.c.

#include "game_internal.h"

/// A view of EWRAM from gSceneLocalState on, which
/// MainRoutineWork_InitializeDefaults writes one byte through.
struct MapLocalStateSnapshotView {
  u8 mapAndInventory[256]; ///< gSceneLocalState, then most of
                           ///< gInventorySnapshot
  /// gInventorySnapshot's last word, past the 65 halfwords the snapshot
  /// uses; nothing reads or writes it through this view.
  u32 playTime;
  /// gActorSnapshot.slots[0]: set to ACTOR_SNAPSHOT_EMPTY,
  /// so a new game holds no saved actors.
  u8 firstSnapshotSlot;
};

/// The running totals Unit_Calculate builds a unit's stats in.
struct StatWork {
  s32 hp;          ///< maximum HP
  s32 pp;          ///< maximum PP
  s32 atk;         ///< Attack, 0 to UNIT_STAT_MAX once clamped
  s32 def;         ///< Defense, 0 to UNIT_STAT_MAX once clamped
  s32 agi;         ///< Agility, 0 to UNIT_STAT_MAX once clamped
  s32 unused;      ///< never written; a gap between Agility and Luck
  s32 luck;        ///< Luck, 0 to UNIT_LUCK_MAX once clamped
  s32 actionCount; ///< actions a turn, 0 to 2
  s32 hpRegen;     ///< HP regained each turn
  s32 ppRegen;     ///< PP regained each turn
  struct {
    s32 power;
    s32 resistance;
  } element[ELEMENT_COUNT];    ///< each element's power and resistance
  s32 kind;                    ///< the item effect being applied
  s32 unusedAfterKind[2];      ///< never written
  s32 amount;                  ///< the amount of the item effect being applied
  struct ItemDefinition *item; ///< the equipped item being applied
  s32 unusedAfterItem;         ///< never written
};

/// The distance between `a` and `b`, |a - b|.
#define UNIT_STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

/// Starts a new game: clears the main routine's work and restarts at the
/// opening.
void Game_RestartQuest(void) {
  MainRoutineWork_InitializeDefaults();
  MainRoutine_RunFar(0);
}

/// Returns the party's Djinn recovery bank, the standby Djinn and their turns
/// to recover, or for the enemy side a battle unit read as one.
struct DjinnRecoveryTable *Djinn_GetRecoveryTable(s32 enemySide) {
  if (enemySide != 0)
    return (struct DjinnRecoveryTable *)Unit_Get(FIRST_ENEMY_UNIT + 3);
  return (struct DjinnRecoveryTable *)&gMainRoutineWork.summonMask;
}

/// Returns the average level of the party members, rounded down (0 with no
/// party members).
u32 Party_GetAverageLevel(void) {
  s32 totalLevels = 0;
  s32 partySize = Party_CountMembers();
  s32 i;

  if (partySize == 0)
    return 0;
  for (i = 0; i < partySize; i++)
    totalLevels += Unit_Get(gMainRoutineWork.party[i])->level;
  totalLevels /= partySize;
  return totalLevels;
}

/// Returns the unit of unitId: ISAAC to PIERS the characters, the six from
/// FIRST_ENEMY_UNIT the enemies of the current battle; NULL for anything else.
struct Unit *Unit_Get(u32 unitId) {
  struct Unit *states;
  struct Unit *unit;
  // DEAD: the return address, read and never used; the ROM keeps its copy of
  // lr into r3.
  UNUSED void *caller = __builtin_return_address(0);

  states = gCharacterUnits;
  /* FAKEMATCH(dead): the party return repeated under a test of the return
   * address. */
  if (caller == NULL) {
    if (unitId <= PIERS) {
      unit = &states[unitId];
      return unit;
    }
  }
  if (unitId <= PIERS) {
    unit = &states[unitId];
    return unit;
  }
  if (unitId - FIRST_ENEMY_UNIT < 6 && gEnemyUnits != NULL) {
    unit = &((struct Unit *)gEnemyUnits)[unitId - FIRST_ENEMY_UNIT];
    return unit;
  }
  return NULL;
}

/// Returns the definition of enemy selector; enemy ids start past the eight
/// characters', so that one id names any unit.
const struct EnemyDefinition *Enemy_GetDefinition(s32 selector) {
  u32 recordIndex = selector - ENEMY_ID_FIRST;

  // BUG: the table has 165 rows, but only rows past 249 are refused, so 165 to
  // 249 read the tables after it. BattleUnit_Set refuses them itself.
#ifdef BUGFIX
  if (recordIndex > 164U) {
#else
  if (recordIndex > 249U) {
#endif
    recordIndex = 0;
  }
  return &gEnemy_Definitions[recordIndex];
}

/// Copies `count` bytes from `first` to `second` when `direction` is set, and
/// from `second` to `first` otherwise.
void Memory_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                 s32 direction) {
  s32 i;

  if (direction != 0) {
    for (i = 0; i < count; i++)
      *second++ = *first++;
  } else {
    for (i = 0; i < count; i++)
      *first++ = *second++;
  }
}

/// A scaling effect's number among the eight, from EQUIP_EFFECT_SCALE_HP.
#define SCALE_EFFECT(kind) ((kind) - EQUIP_EFFECT_SCALE_HP)

/// Works out a unit's stats from its base stats, equipment, set Djinn, class
/// and battle stat levels; its HP and PP keep their share of a new maximum.
void Unit_Calculate(s32 unitId) {
  struct StatWork *stats;
  struct Unit *unit;
  s32 index, effectIndex, elementIndex;
  s32 value;
  s32 lighthouseLit;
  s32 effectKind;
  s32 effectAmount;
  s32 statLimit;
  s16 previousMaximum;

  // Start with the unit's base stats and elemental power/resistance.
  stats = Heap_AllocEwram(sizeof(struct StatWork));
  unit = Unit_Get(unitId);
  stats->hp = unit->baseHp;
  stats->pp = unit->basePp;
  stats->atk = unit->baseAttack;
  stats->def = unit->baseDefense;
  stats->agi = unit->baseAgility;
  stats->luck = unit->baseLuck;
  stats->actionCount = unit->baseActionCount & 15;
  stats->hpRegen = unit->baseHpRegen;
  stats->ppRegen = unit->basePpRegen;
  for (index = 0; index < ELEMENT_COUNT; index++) {
    stats->element[index].power = unit->baseElement[index].power;
    stats->element[index].resistance = unit->baseElement[index].resistance;
  }

  // HP and PP keep their share of the maximum; gauges that no longer match are
  // reset to full first.
  if (UNIT_STAT_DIFF(unit->hpMax * unit->hpGauge / UNIT_GAUGE_FULL, unit->hp) >
          1 ||
      UNIT_STAT_DIFF(unit->ppMax * unit->ppGauge / UNIT_GAUGE_FULL, unit->pp) >
          1) {
    unit->hpGauge = UNIT_GAUGE_FULL;
    unit->ppGauge = UNIT_GAUGE_FULL;
    unit->hp = unit->hpMax;
    unit->pp = unit->ppMax;
  }

  // CURSE_KEEP_BOUND keeps CURSE_BOUND, and CURSE_REMOVABLE clears
  // both again below.
  unit->curse &= ~(CURSE_BOUND | CURSE_ITEM);
  if (unit->curse & CURSE_KEEP_BOUND)
    unit->curse |= CURSE_BOUND;
  if (unit->ready)
    stats->actionCount++;
  unit->equipBonus[0] = 0;
  unit->equipBonus[1] = 0;

  // Apply additive equipment and set-Djinn bonuses before class scaling.
  // An enemy (CLASS_NONE) skips equipment, Djinn and class entirely.
  if (unit->classId != CLASS_NONE) {
    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        continue;
      stats->item = Item_GetDirect(unit->inventory[index]);
      if (stats->item->flags & ITEM_CURSED)
        unit->curse |= CURSE_BOUND | CURSE_ITEM;
      stats->atk += stats->item->primaryBonus;
      /* FAKEMATCH(dead): an empty once-only loop after the Attack bonus. */
      do {
      } while (0);
      stats->def += stats->item->secondaryBonus;
      for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++) {
        effectKind = stats->item->effects[effectIndex].kind;
        effectAmount = stats->item->effects[effectIndex].amount;
        stats->kind = effectKind;
        stats->amount = effectAmount;
        switch (stats->kind) {
        case EQUIP_EFFECT_NONE:
          break;
        case EQUIP_EFFECT_ADD_HP:
          stats->hp += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_HP_REGEN:
          stats->hpRegen += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_PP:
          stats->pp += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_PP_REGEN:
          stats->ppRegen += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_AGILITY:
          stats->agi += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_LUCK:
          stats->luck += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_VENUS_POWER:
          stats->element[ELEMENT_VENUS].power += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MERCURY_POWER:
          stats->element[ELEMENT_MERCURY].power += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MARS_POWER:
          stats->element[ELEMENT_MARS].power += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_JUPITER_POWER:
          stats->element[ELEMENT_JUPITER].power += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_VENUS_RESIST:
          stats->element[ELEMENT_VENUS].resistance += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MERCURY_RESIST:
          stats->element[ELEMENT_MERCURY].resistance += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MARS_RESIST:
          stats->element[ELEMENT_MARS].resistance += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_JUPITER_RESIST:
          stats->element[ELEMENT_JUPITER].resistance += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_UNLEASH_CHANCE:
          unit->equipBonus[0] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_COUNTERSTRIKE_RATE:
          unit->equipBonus[1] += stats->amount;
          break;
        case EQUIP_EFFECT_CURSE_REMOVABLE:
          unit->curse |= CURSE_REMOVABLE;
          break;
        case EQUIP_EFFECT_ADD_ACTIONS:
          stats->actionCount += stats->amount;
          break;
        }
      }
    }
    if (unit->curse & CURSE_REMOVABLE)
      unit->curse &= ~(CURSE_REMOVABLE | CURSE_BOUND);

    // Only set Djinn contribute their stat bonuses.
    for (elementIndex = 0; elementIndex < ELEMENT_COUNT; elementIndex++) {
      u32 activeDjinn = unit->djinnSet[elementIndex];

      for (index = 0; index < DJINN_PER_ELEMENT; index++) {
        if (activeDjinn & (1 << index)) {
          const struct DjinnDefinition *djinn =
              Djinn_GetDefinition(elementIndex, index);

          stats->hp += djinn->hp;
          stats->pp += djinn->pp;
          stats->atk += djinn->atk;
          stats->def += djinn->def;
          stats->agi += djinn->agi;
          stats->luck += djinn->luck;
        }
      }
    }

    // The class's multipliers are in tenths (STAT_SCALE_ONE keeps a stat)
    {
      struct ClassDefinition *classDefinition =
          Class_GetDefinition(unit->classId);

      stats->hp = stats->hp * classDefinition->hp / STAT_SCALE_ONE;
      stats->pp = stats->pp * classDefinition->pp / STAT_SCALE_ONE;
      stats->atk = stats->atk * classDefinition->atk / STAT_SCALE_ONE;
      stats->def = stats->def * classDefinition->def / STAT_SCALE_ONE;
      stats->agi = stats->agi * classDefinition->agi / STAT_SCALE_ONE;
      stats->luck = stats->luck * classDefinition->luck / STAT_SCALE_ONE;
    }

    // Then the equipment's multiplying effects, EQUIP_EFFECT_SCALE_HP to
    // EQUIP_EFFECT_SCALE_LUCK, also in tenths, applied after the class
    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        continue;
      stats->item = Item_GetDirect(unit->inventory[index]);
      for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++) {
        effectKind = stats->item->effects[effectIndex].kind;
        effectAmount = stats->item->effects[effectIndex].amount;
        stats->kind = effectKind;
        effectKind = SCALE_EFFECT(effectKind);
        stats->amount = effectAmount;
        switch (effectKind) {
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_HP):
          stats->hp = stats->hp * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_HP_REGEN):
          stats->hpRegen = stats->hpRegen * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_PP):
          stats->pp = stats->pp * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_PP_REGEN):
          stats->ppRegen = stats->ppRegen * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_ATTACK):
          stats->atk = stats->atk * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_DEFENSE):
          stats->def = stats->def * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_AGILITY):
          stats->agi = stats->agi * stats->amount / STAT_SCALE_ONE;
          break;
        case SCALE_EFFECT(EQUIP_EFFECT_SCALE_LUCK):
          stats->luck = stats->luck * stats->amount / STAT_SCALE_ONE;
          break;
        }
      }
    }
  }

  // Then the battle's changes apply: the Attack, Defense and Agility stages,
  // a summon's boost to each element's power, and the Resistance stage.
  stats->atk = stats->atk * (unit->attackStage + 8) / 8;
  stats->def = stats->def * (unit->defenseStage + 8) / 8;
  stats->agi = stats->agi * (unit->agilityStage + 8) / 8;
  for (index = 0; index < ELEMENT_COUNT; index++)
    stats->element[index].power +=
        (unit->summonBoost[index] * unit->summonBoost[index] +
         unit->summonBoost[index]) *
        5;
  for (index = 0; index < ELEMENT_COUNT; index++)
    stats->element[index].resistance += unit->resistanceStage * 20;

  if (unit->classId != CLASS_NONE) {
    lighthouseLit = 0;
    // Faster PP recovery once the character's own element's lighthouse is lit
    switch (unit->id) {
    case ISAAC:
      lighthouseLit = GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_LIT);
      break;
    case GARET:
      lighthouseLit = GameFlag_Get(FLAG_MARS_LIGHTHOUSE_LIT);
      break;
    case IVAN:
      lighthouseLit = GameFlag_Get(FLAG_JUPITER_LIGHTHOUSE_LIT);
      break;
    case MIA:
      lighthouseLit = GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_LIT);
      break;
    case JENNA:
      lighthouseLit = GameFlag_Get(FLAG_MARS_LIGHTHOUSE_LIT);
      break;
    }
    if (lighthouseLit)
      stats->ppRegen += 4;
  }

  CLAMP(stats->atk, 0, UNIT_STAT_MAX);
  CLAMP(stats->def, 0, UNIT_STAT_MAX);
  CLAMP(stats->agi, 0, UNIT_STAT_MAX);
  CLAMP(stats->luck, 0, UNIT_LUCK_MAX);
  CLAMP(stats->actionCount, 0, 2);
  CLAMP(stats->hpRegen, 0, UNIT_HP_REGEN_MAX);
  CLAMP(stats->ppRegen, 0, UNIT_PP_REGEN_MAX);
  for (index = 0; index < ELEMENT_COUNT; index++) {
    CLAMP(stats->element[index].power, 0, UNIT_ELEMENT_MAX);
    CLAMP(stats->element[index].resistance, 0, UNIT_ELEMENT_MAX);
  }

  unit->atk = stats->atk;
  unit->def = stats->def;
  unit->agi = stats->agi;
  unit->luck = stats->luck;
  unit->actionsPerTurn = stats->actionCount;
  unit->hpRegen = stats->hpRegen;
  unit->ppRegen = stats->ppRegen;
  for (index = 0; index < ELEMENT_COUNT; index++) {
    unit->element[index].power = stats->element[index].power;
    unit->element[index].resistance = stats->element[index].resistance;
  }

  statLimit = UNIT_HP_MAX;
  if (unit->classId)
    statLimit = UNIT_CHARACTER_HP_MAX;

  previousMaximum = unit->hpMax;
  CLAMP(stats->hp, 0, statLimit);
  unit->hpMax = stats->hp;
  if (previousMaximum != unit->hpMax) {
    value = stats->hp * unit->hpGauge / UNIT_GAUGE_FULL;
    CLAMP(value, 0, statLimit);
    if (unit->hp != 0 && value == 0)
      value = 1;
    unit->hp = value;
  }

  previousMaximum = unit->ppMax;
  CLAMP(stats->pp, 0, statLimit);
  unit->ppMax = stats->pp;
  if (previousMaximum != unit->ppMax) {
    value = stats->pp * unit->ppGauge / UNIT_GAUGE_FULL;
    CLAMP(value, 0, statLimit);
    if (unit->pp != 0 && value == 0)
      value = 1;
    unit->pp = value;
  }

  Heap_Release(stats);
}

/// Sets FLAG_LURE_CAP when any party member has the Lure Cap (the one item with
/// EQUIP_EFFECT_LURE) equipped, and clears it otherwise.
void GameFlag_RefreshLureCap(void) {
  s32 partySize;
  s32 member;

  GameFlag_Clear(FLAG_LURE_CAP);
  partySize = Party_CountMembers();
  for (member = 0; member < partySize; member++) {
    struct Unit *unit = Unit_Get(gMainRoutineWork.party[member]);
    s32 slot;

    for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
      struct ItemDefinition *item;
      s32 effect;

      if (!(unit->inventory[slot] & INVENTORY_EQUIPPED))
        continue;
      item = Item_GetDirect(unit->inventory[slot]);
      for (effect = 0; effect < EQUIP_EFFECT_SLOTS; effect++) {
        if (item->effects[effect].kind == EQUIP_EFFECT_LURE)
          GameFlag_Set(FLAG_LURE_CAP);
      }
    }
  }
}

/// Encodes the build stamp's time, which a new game takes as its start stamp
/// and the file list compares saves with.
u32 BuildStamp_GetTime(void) {
  // The stamp is six ASCII digits, hhmmss.
  const u8 *digits = Resource_GetTableEntry(RESOURCE_BUILD_STAMP);
  s32 hours;
  s32 minutes;
  s32 seconds;
  s16 time;
  s32 stamp;

  hours = (*digits++ - '0') * 10;
  hours += *digits++ - '0';
  minutes = (*digits++ - '0') * 10;
  minutes += *digits++ - '0';
  seconds = (digits[0] - '0') * 10;
  seconds += digits[1] - '0';
  time = (hours * 16 + minutes) * 64 + seconds;
  time |= 0x1000;
  stamp = time;

  if (gDebugMode != 0)
    stamp |= 0xFFFF8000;
  return (u16)stamp;
}

/// Clears the game's state and sets a new game's defaults: Isaac alone in the
/// party, the standard buttons, the message speed and the Lucky Wheels odds.
void MainRoutineWork_InitializeDefaults(void) {
  volatile s32 zero;

  // Clear the state up to the characters, the scene state with its snapshots,
  // and the flags.
  zero = 0;
  START_DMA(&zero, &gMainRoutineWork,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 176);
  zero = 0;
  START_DMA(&zero, gSceneLocalState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 993);
  WAIT_DMA();
  zero = 0;
  START_DMA(&zero, gGameFlagBytes,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 128);
  ((struct MapLocalStateSnapshotView *)gSceneLocalState)->firstSnapshotSlot =
      ACTOR_SNAPSHOT_EMPTY;
  zero = 0;
  START_DMA(&zero, gCharacterUnits,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 664);
  // Build the characters, then install input and message defaults.
  Party_InitializeCharacters();
  gMainRoutineWork.aKeys = KEY_A;
  gMainRoutineWork.bKeys = KEY_B;
  gMainRoutineWork.selectKeys = KEY_SELECT;
  gMainRoutineWork.startKeys = KEY_START;
  gMainRoutineWork.firstShortcutKeys = KEY_L;
  gMainRoutineWork.secondShortcutKeys = KEY_R;
  gMainRoutineWork.dashKeys = KEY_B;
  gMainRoutineWork.firstShortcut = 0;
  gMainRoutineWork.secondShortcut = 0;
  gMainRoutineWork.leaderActorId = 0;
  Party_AddMember(ISAAC);
  gMainRoutineWork.coins = 0;
  gMainRoutineWork.messageSpeed = 1;
  gMainRoutineWork.speech = 1;
  gMainRoutineWork.unusedAfterSpeech = 1;
  gMainRoutineWork.windowHue = 0;
  gMainRoutineWork.windowBrightness = 8;
  gMainRoutineWork.buildStamp = 0;
  gMainRoutineWork.startStamp = BuildStamp_GetTime();
  gPlayTimeFrames = 0;
  gMainRoutineWork.playTime = 0;
  gIdleSleepEnabled = 0;
  gMainRoutineWork.idleSleepEnabled = gIdleSleepEnabled;
  gIdleFrames = 0;
  gSaveSlot = -1;
  // Three prizes share each successive slot-machine weight.
  gMainRoutineWork.prizeWeights[0] = 4;
  gMainRoutineWork.prizeWeights[1] = 4;
  gMainRoutineWork.prizeWeights[2] = 4;
  gMainRoutineWork.prizeWeights[3] = 8;
  gMainRoutineWork.prizeWeights[4] = 8;
  gMainRoutineWork.prizeWeights[5] = 8;
  gMainRoutineWork.prizeWeights[6] = 16;
  gMainRoutineWork.prizeWeights[7] = 16;
  gMainRoutineWork.prizeWeights[8] = 16;
  gMainRoutineWork.prizeWeights[9] = 32;
  gMainRoutineWork.prizeWeights[10] = 32;
  gMainRoutineWork.prizeWeights[11] = 32;
  gMainRoutineWork.prizeWeights[12] = 64;
  gMainRoutineWork.prizeWeights[13] = 64;
  gMainRoutineWork.prizeWeights[14] = 64;
}

/// Sets FLAG_CHILDHOOD and recomputes Isaac, Garet and Jenna, whose classes
/// then take their childhood forms.
void Party_StartChildhood(void) {
  GameFlag_Set(FLAG_CHILDHOOD);
  Unit_UpdateClass(ISAAC);
  Unit_UpdateClass(GARET);
  Unit_UpdateClass(JENNA);
  Unit_Calculate(ISAAC);
  Unit_Calculate(GARET);
  Unit_Calculate(JENNA);
}

/// The party after the prologue: Isaac and Garet healed and given the Short
/// Sword, Move and Retreat for Isaac, Move for Garet, Mind Read for Ivan, and
/// 300 coins.
void Party_EndChildhood(void) {
  s32 unitId;

  GameFlag_Clear(FLAG_CHILDHOOD);
  GameFlag_Clear(FLAG_ISAAC_WEARS_TUNIC);
  GameFlag_Set(FLAG_VALE_THREE_YEARS_LATER);
  Unit_UpdateClass(JENNA);
  Unit_Calculate(JENNA);
  GameFlag_Clear(FLAG_KEEP_MUSIC_HELD);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);

  for (unitId = 0; unitId <= 1; unitId++) {
    struct Unit *unit = Unit_Get(unitId);
    s32 slot;

    unit->hp = unit->hpMax;
    unit->pp = unit->ppMax;
    Unit_UpdateGauges(unit);

    for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
      if ((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == ITEM_MACHETE) {
        unit->inventory[slot] = ITEM_SHORT_SWORD;
        Inventory_Equip(unitId, slot);
        break;
      }
    }

    Unit_UpdateClass(unitId);
    Unit_Calculate(unitId);
  }

  Unit_LearnPsynergy(ISAAC, ABILITY_MOVE);
  Unit_LearnPsynergy(ISAAC, ABILITY_RETREAT);
  Unit_LearnPsynergy(GARET, ABILITY_MOVE);
  Unit_LearnPsynergy(IVAN, ABILITY_MIND_READ);

  gMainRoutineWork.coins += 300;
}

/// Restores every party member's HP and PP; with `cureConditions` 1 it also
/// cures poison and haunting.
void Party_RestoreHpPp(s32 cureConditions) {
  s32 partySize;
  s32 partyIndex;
  struct Unit *unit;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    unit = Unit_Get(gMainRoutineWork.party[partyIndex]);

    unit->hp = unit->hpMax;
    unit->pp = unit->ppMax;
    Unit_UpdateGauges(unit);

    if (cureConditions == 1) {
      unit->poison = 0;
      unit->haunt = 0;
    }
  }
}

/// Restores the PP of each party member whose element group has a lit
/// lighthouse: Venus or Mars for group 0, Mercury or Jupiter for the others.
void Party_RestorePpOfLitElements(void) {
  s32 partySize;
  s32 partyIndex;
  u8 unitId;
  s32 elementGroup;
  s32 restorePp;
  struct Unit *unit;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    unitId = gMainRoutineWork.party[partyIndex];
    elementGroup = gCharacter_ElementGroupTable[unitId];
    restorePp = 0;
    if (elementGroup == 0) {
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_LIT) ||
          GameFlag_Get(FLAG_MARS_LIGHTHOUSE_LIT)) {
        restorePp = 1;
      }
    } else {
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_LIT) ||
          GameFlag_Get(FLAG_JUPITER_LIGHTHOUSE_LIT)) {
        restorePp = 1;
      }
    }

    if (restorePp == 0) {
      continue;
    }

    unit = Unit_Get(unitId);
    unit->pp = unit->ppMax;
    Unit_UpdateGauges(unit);
  }
}

/// Unused: reserved entry; keeps its place in the far-call table.
UNUSED void Unit_GaugeNoOp(void) {}

/// Recomputes the HP and PP gauges of `unitId` from its current HP and PP, so
/// that Unit_Calculate keeps the same fractions.
void Unit_RecalculateGauges(s32 unitId) {
  struct Unit *unit;

  unit = Unit_Get(unitId);
  Unit_UpdateGauges(unit);
}

/// Clamps a new HP or PP value to 0 and the maximum.
static inline s32 Unit_ClampGaugeValue(s32 value, s32 maximum) {
  s32 result;

  if (value > maximum)
    result = maximum;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  return result;
}

/// Each sets the unit's HP or PP, clamped to 0 and its maximum, and recomputes
/// its HP and PP gauges.
#define UNIT_DEFINE_SET(name, field, maximum)                                  \
  void name(struct Unit *unit, s32 value) {                                    \
    s32 clamped;                                                               \
                                                                               \
    clamped = Unit_ClampGaugeValue(value, unit->maximum);                      \
    unit->field = clamped;                                                     \
    Unit_UpdateGauges(unit);                                                   \
  }

UNIT_DEFINE_SET(Unit_SetHp, hp, hpMax)
/// Only game.s's bare veneer reaches it; nothing calls it by name.
UNIT_DEFINE_SET(Unit_SetPp, pp, ppMax)

/// Each adds delta to the HP or PP of unit unitId, clamped to 0 and its
/// maximum, recomputes its gauges and returns the new value, as `type`.
#define UNIT_DEFINE_ADD(type, name, field, maximum)                            \
  type name(s32 unitId, s32 delta) {                                           \
    struct Unit *unit = Unit_Get(unitId);                                      \
    s32 current = unit->field;                                                 \
    s32 limit = unit->maximum;                                                 \
    s32 value = current + delta;                                               \
    s32 result;                                                                \
                                                                               \
    result = Unit_ClampGaugeValue(value, limit);                               \
    unit->field = result;                                                      \
    Unit_RecalculateGauges(unitId);                                            \
    return unit->field;                                                        \
  }

UNIT_DEFINE_ADD(s32, Unit_AddHp, hp, hpMax)
/// Called when Psynergy is used in the field, in battle and from the menu, by
/// PP recovery at the end of a battle turn, and by the debug menu.
UNIT_DEFINE_ADD(s16, Unit_AddPp, pp, ppMax)

#undef UNIT_DEFINE_SET
#undef UNIT_DEFINE_ADD
#undef UNIT_STAT_DIFF

#undef SCALE_EFFECT
