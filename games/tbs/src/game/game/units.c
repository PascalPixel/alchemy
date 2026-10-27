/// @file
/// The party's units and their stats, a new game, the prologue, HP and PP.
// Part of game.c.

#include "../game_internal.h"

/// The running totals Unit_Calculate builds a unit's stats in. Each call takes
/// a block from EWRAM first (AllocEwram) and frees it
/// with Free before returning; only Unit_Calculate reads it.
struct StatWork {
  s32 hp;                 ///< maximum HP
  s32 pp;                 ///< maximum PP
  s32 atk;                ///< Attack, 0 to UNIT_STAT_MAX once clamped
  s32 def;                ///< Defense, 0 to UNIT_STAT_MAX once clamped
  s32 agi;                ///< Agility, 0 to UNIT_STAT_MAX once clamped
  s32 unused;             ///< never written; a gap between Agility and Luck
  s32 luck;               ///< Luck, 0 to UNIT_LUCK_MAX once clamped
  s32 turns;              ///< actions a round, 0 to 2
  s32 hpRegen;            ///< HP regained each round
  s32 ppRegen;            ///< PP regained each round
  s32 element[4][2];      ///< [element][0] power and [1] resistance
  s32 kind;               ///< the item effect being applied
  s32 unusedAfterKind[2]; ///< never written
  s32 amount;             ///< the amount of the item effect being applied
  struct ItemDefinition *item; ///< the equipped item being applied
  s32 unusedAfterItem;         ///< never written
};

/// The distance between `a` and `b`, |a - b|.
#define UNIT_STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

/// Starts a new game: clears the persistent state and restarts at the opening.
/// Only GAME.S's bare veneer reaches it; nothing calls it by name.
void Game_RestartQuest(void) {
  PersistentState_InitializeDefaults();
  Monitor_RunFar(0);
}

/// Returns the party's Djinn recovery bank (the standby Djinn and their turns
/// to recover), or for the enemy side battle unit 0x83 itself, read as a
/// bank. The bank also holds the summon flags Summon_ListLearned reads.
/// Called by the battle code that sets, recovers and links Djinn
/// (DjinnRecovery_Tick, BattleSummon_UpdateAvailability, the link battle).
void *Djinn_GetRecoveryTable(s32 enemySide) {
  if (enemySide != 0) {
    return Unit_Get(0x83);
  }
  return &gPersistentState.summonMask;
}

/// Returns the average level of the party members, rounded down (0 with no
/// party members). Called by Encounter_CountStepsAndSelectFormation to pick a
/// random battle's enemies, and by the Lucky Dice game in Tolbi to set its
/// prizes.
u32 Party_GetAverageLevel(void) {
  s32 partySize;
  s32 totalLevels;
  s32 partyIndex;

  totalLevels = 0;
  partySize = Party_CountMembers();
  if (partySize == 0) {
    return 0;
  }
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    totalLevels += Unit_Get(gPersistentState.party[partyIndex])->level;
  }
  totalLevels = totalLevels / partySize;
  return totalLevels;
}

/// Returns the unit of `unitId`: ISAAC to PIERS are the playable
/// characters in the persistent state, 0x80 to 0x85 the enemies of the current
/// battle. Anything else, or an enemy outside a battle, is NULL. Nearly every
/// bank calls it, through GAME.S as Unit_GetFar.
struct Unit *Unit_Get(u32 unitId) {
  struct Unit *states;
  struct Unit *unit;

  {
    void *caller = __builtin_return_address(0);

    /* FAKEMATCH(barrier): an empty asm reading the return address in a low
     * register, as the ROM's mov r3, lr at entry; as no asm, 1 of 28
     * instructions differ in tbs-en. */
    __asm__("" : : "l"(caller));
  }
  states = gPersistentState.units;
  if (unitId <= PIERS) {
    unit = &states[unitId];
    return unit;
  }
  if (unitId - 0x80 < 6 && gEnemyUnits != NULL) {
    unit = &((struct Unit *)gEnemyUnits)[unitId - 0x80];
    return unit;
  }
  return NULL;
}

/// Returns the definition of enemy `selector`, in row `selector -
/// ENEMY_ID_FIRST`; an id past the first 250 gives the first. Enemy ids start
/// past the eight characters' so that one id names any unit: Unit.id holds
/// either, and gBattleSprites, indexed by it, keeps the characters' rows (their
/// attack animation and hop) before the enemies'. Called by the
/// battle code that builds and scores enemies (the formations, the enemy AI,
/// the spoils) and by the element and weakness reads in this object.
const u8 *Enemy_GetDefinition(s32 selector) {
  u32 recordIndex;

  recordIndex = selector - ENEMY_ID_FIRST;
  // BUG: the table has 165 rows, but only rows past 249 are refused, so 165 to
  // 249 read the tables after it. BattleUnit_Set refuses them itself.
#ifdef BUGFIX
  if (recordIndex > 164U) {
#else
  if (recordIndex > 0xF9U) {
#endif
    recordIndex = 0;
  }
  return (const u8 *)&gEnemy_Definitions[recordIndex];
}

/// Copies `count` bytes from `first` to `second` when `direction` is set, and
/// from `second` to `first` otherwise.
void Memory_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                 s32 direction) {
  s32 index;

  if (direction != 0) {
    index = 0;
    if (index < count) {
      do {
        *second++ = *first++;
        index++;
      } while (index < count);
    }
  } else if (count > 0) {
    index = count;
    do {
      *first++ = *second++;
      index--;
    } while (index != 0);
  }
}

/// Recomputes a unit's stats from its base stats, equipment, set Djinn, class
/// and battle stat levels, clamps each to its range and writes them to the
/// unit. A change of maximum HP or PP keeps the unit's current HP and PP at
/// the same fraction of it. Also sets the unit's curse bits and
/// equipment bonuses. Called after anything that changes those inputs:
/// equipping, setting Djinn, levelling up, a class change, battle stat
/// changes.
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
  stats = AllocEwram(sizeof(struct StatWork));
  unit = Unit_Get(unitId);
  stats->hp = unit->baseHp;
  stats->pp = unit->basePp;
  stats->atk = unit->baseAttack;
  stats->def = unit->baseDefense;
  stats->agi = unit->baseAgility;
  stats->luck = unit->baseLuck;
  stats->turns = unit->baseActionCount & 15;
  stats->hpRegen = unit->baseHpRegen;
  stats->ppRegen = unit->basePpRegen;
  for (index = 0; index < ELEMENT_COUNT; index++) {
    stats->element[index][0] = unit->baseElement[index].power;
    stats->element[index][1] = unit->baseElement[index].resistance;
  }

  // A gauge is HP or PP as a fraction of its maximum, 0x4000 full. When
  // either no longer gives its stat to within 1 (the stat was written
  // without Unit_RecalculateGauges), both gauges are reset to full and the
  // unit's HP and PP refilled, before the fractions carry over to the new
  // maxima.
  if (UNIT_STAT_DIFF(unit->hpMax * unit->hpGauge / 0x4000, unit->hp) > 1 ||
      UNIT_STAT_DIFF(unit->ppMax * unit->ppGauge / 0x4000, unit->pp) > 1) {
    unit->hpGauge = 0x4000;
    unit->ppGauge = 0x4000;
    unit->hp = unit->hpMax;
    unit->pp = unit->ppMax;
  }

  // CURSE_KEEP_BOUND keeps CURSE_BOUND, and CURSE_REMOVABLE clears
  // both again below.
  unit->curse &= ~(CURSE_BOUND | CURSE_ITEM);
  if (unit->curse & CURSE_KEEP_BOUND)
    unit->curse |= CURSE_BOUND;
  if (unit->ready)
    stats->turns++;
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
      /* FAKEMATCH(one-pass): the Attack bonus in a once-only block keeps the
       * Defense load after the item's second bonus; as plain statements, 1 of
       * 864 instructions differ in tbs-en. */
      do {
        stats->atk += stats->item->primaryBonus;
      } while (0);
      stats->def += stats->item->secondaryBonus;
      for (effectIndex = 0; effectIndex < 4; effectIndex++) {
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
          stats->element[0][0] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MERCURY_POWER:
          stats->element[1][0] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MARS_POWER:
          stats->element[2][0] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_JUPITER_POWER:
          stats->element[3][0] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_VENUS_RESIST:
          stats->element[0][1] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MERCURY_RESIST:
          stats->element[1][1] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_MARS_RESIST:
          stats->element[2][1] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_JUPITER_RESIST:
          stats->element[3][1] += stats->amount;
          break;
        case EQUIP_EFFECT_ADD_UNLEASH_CHANCE:
          unit->equipBonus[0] += stats->amount;
          break;
        case EQUIP_EFFECT_24:
          unit->equipBonus[1] += stats->amount;
          break;
        case EQUIP_EFFECT_CURSE_REMOVABLE:
          unit->curse |= CURSE_REMOVABLE;
          break;
        case EQUIP_EFFECT_ADD_TURNS:
          stats->turns += stats->amount;
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

    // The class's multipliers are in tenths: 10 keeps a stat as it is
    {
      struct ClassDefinition *classDefinition =
          Class_GetDefinition(unit->classId);

      stats->hp = stats->hp * classDefinition->hp / 10;
      stats->pp = stats->pp * classDefinition->pp / 10;
      stats->atk = stats->atk * classDefinition->atk / 10;
      stats->def = stats->def * classDefinition->def / 10;
      stats->agi = stats->agi * classDefinition->agi / 10;
      stats->luck = stats->luck * classDefinition->luck / 10;
    }

    // Then the equipment's multiplying effects, EQUIP_EFFECT_SCALE_HP to
    // EQUIP_EFFECT_SCALE_LUCK, also in tenths, applied after the class
    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        continue;
      stats->item = Item_GetDirect(unit->inventory[index]);
      for (effectIndex = 0; effectIndex < 4; effectIndex++) {
        effectKind = stats->item->effects[effectIndex].kind;
        effectAmount = stats->item->effects[effectIndex].amount;
        stats->kind = effectKind;
        effectKind -= EQUIP_EFFECT_SCALE_HP;
        stats->amount = effectAmount;
        // The kind less EQUIP_EFFECT_SCALE_HP, as the switch table begins
        // there.
        switch (effectKind) {
        case EQUIP_EFFECT_SCALE_HP - EQUIP_EFFECT_SCALE_HP:
          stats->hp = stats->hp * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_HP_REGEN - EQUIP_EFFECT_SCALE_HP:
          stats->hpRegen = stats->hpRegen * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_PP - EQUIP_EFFECT_SCALE_HP:
          stats->pp = stats->pp * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_PP_REGEN - EQUIP_EFFECT_SCALE_HP:
          stats->ppRegen = stats->ppRegen * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_ATTACK - EQUIP_EFFECT_SCALE_HP:
          stats->atk = stats->atk * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_DEFENSE - EQUIP_EFFECT_SCALE_HP:
          stats->def = stats->def * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_AGILITY - EQUIP_EFFECT_SCALE_HP:
          stats->agi = stats->agi * stats->amount / 10;
          break;
        case EQUIP_EFFECT_SCALE_LUCK - EQUIP_EFFECT_SCALE_HP:
          stats->luck = stats->luck * stats->amount / 10;
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
    stats->element[index][0] +=
        (unit->summonBoost[index] * unit->summonBoost[index] +
         unit->summonBoost[index]) *
        5;
  for (index = 0; index < ELEMENT_COUNT; index++)
    stats->element[index][1] += unit->resistanceStage * 20;

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
  CLAMP(stats->turns, 0, 2);
  CLAMP(stats->hpRegen, 0, UNIT_HP_REGEN_MAX);
  CLAMP(stats->ppRegen, 0, UNIT_PP_REGEN_MAX);
  for (index = 0; index < ELEMENT_COUNT; index++) {
    CLAMP(stats->element[index][0], 0, UNIT_ELEMENT_MAX);
    CLAMP(stats->element[index][1], 0, UNIT_ELEMENT_MAX);
  }

  unit->atk = stats->atk;
  unit->def = stats->def;
  unit->agi = stats->agi;
  unit->luck = stats->luck;
  unit->actionsPerRound = stats->turns;
  unit->hpRegen = stats->hpRegen;
  unit->ppRegen = stats->ppRegen;
  for (index = 0; index < ELEMENT_COUNT; index++) {
    unit->element[index].power = stats->element[index][0];
    unit->element[index].resistance = stats->element[index][1];
  }

  statLimit = UNIT_HP_MAX;
  if (unit->classId)
    statLimit = UNIT_CHARACTER_HP_MAX;

  previousMaximum = unit->hpMax;
  CLAMP(stats->hp, 0, statLimit);
  unit->hpMax = stats->hp;
  if (previousMaximum != unit->hpMax) {
    value = stats->hp * unit->hpGauge / 0x4000;
    CLAMP(value, 0, statLimit);
    if (unit->hp != 0 && value == 0)
      value = 1;
    unit->hp = value;
  }

  previousMaximum = unit->ppMax;
  CLAMP(stats->pp, 0, statLimit);
  unit->ppMax = stats->pp;
  if (previousMaximum != unit->ppMax) {
    value = stats->pp * unit->ppGauge / 0x4000;
    CLAMP(value, 0, statLimit);
    if (unit->pp != 0 && value == 0)
      value = 1;
    unit->pp = value;
  }

  Free(stats);
}

/// Sets FLAG_LURE_CAP when any party member has the Lure Cap (the one item
/// with EQUIP_EFFECT_LURE) equipped, and clears it otherwise. Called on
/// entering a scene (Scene_Arrive, FieldLoop_Run) and at the end of
/// an event, so the encounter rate follows the cap.
void GameFlag_RefreshLureCap(void) {
  s32 partySize;
  s32 partyIndex;

  GameFlag_Clear(FLAG_LURE_CAP);
  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct Unit *unit;
    s32 inventorySlot;

    unit = Unit_Get(gPersistentState.party[partyIndex]);
    for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        struct EquipEffect *effect;
        s32 effectIndex;

        effect = Item_GetDirect(unit->inventory[inventorySlot])->effects;
        for (effectIndex = 0; effectIndex < 4; effectIndex++) {
          u8 effectKind;

          effectKind = effect->kind;
          effect++;
          if (effectKind == EQUIP_EFFECT_LURE) {
            GameFlag_Set(FLAG_LURE_CAP);
          }
        }
      }
    }
  }
}

/// Encodes the build-stamp time as ((hours * 16 + minutes) * 64 + seconds)
/// with bit 12 set. Debug mode also sets bits 15..31 before the halfword cast.
/// PersistentState_InitializeDefaults stores it as a new game's start stamp,
/// and the file list (FileList_SelectFile, Save_BuildFileSummary) and the main
/// menu compare saves with it.
u32 BuildStamp_GetTime(void) {
  u8 *digits;
  s32 hourTens;
  s32 hourUnits;
  s32 minuteTens;
  s32 minuteUnits;
  s32 secondTens;
  s32 secondUnits;
  s32 hours;
  s32 minutes;
  s32 seconds;
  s32 packed;
  s32 shifted;
  s32 result;

  // The stamp is six ASCII digits, hhmmss.
  digits = Resource_GetTableEntry((s32)&ResourceId_BuildStamp);
  hourTens = *digits;
  hours = (hourTens - '0') * 10;
  digits++;
  hourUnits = *digits;
  digits++;
  hours += hourUnits - '0';
  minuteTens = *digits;
  minutes = (minuteTens - '0') * 10;
  digits++;
  minuteUnits = *digits;
  digits++;
  minutes += minuteUnits - '0';
  secondTens = digits[0];
  seconds = (secondTens - '0') * 10;
  secondUnits = digits[1];
  seconds += secondUnits - '0';
  packed = (((hours << 4) + minutes) << 6) + seconds;
  shifted = 0x80 << 21;
  shifted |= packed << 16;
  result = shifted >> 16;
  if (gDebugMode != 0) {
    result |= 0xffff8000;
  }
  return (u16)result;
}

/// A view of EWRAM from gSceneLocalState on, which
/// PersistentState_InitializeDefaults writes one byte through.
/// gSceneLocalState's 0x78 bytes and gInventorySnapshot's 0x8c come first, so
/// firstSnapshotSlot is the first byte of gActorSnapshot.
struct MapLocalStateSnapshotView {
  u8 mapAndInventory[0x100]; ///< gSceneLocalState, then most of
                             ///< gInventorySnapshot
  /// gInventorySnapshot's last word, past the 65 halfwords the snapshot
  /// uses; nothing reads or writes it through this view.
  u32 playTime;
  /// gActorSnapshot.slots[0]: set to 0xff, ACTOR_SNAPSHOT_EMPTY,
  /// so a new game holds no saved actors.
  u8 firstSnapshotSlot;
};

/// Clears the game-state header, the scene state with the Colosso's inventory
/// snapshot and the actor snapshot after it, the flags and most of the
/// save stamp; sets a new game's defaults: Isaac alone in the party, the
/// standard button layout, the message speed, the play time clock and the Lucky
/// Wheels prize odds. Called by the File Screen's new game, by
/// Game_RestartQuest and Monitor_Run, and by the debug battle menu.
void PersistentState_InitializeDefaults(void) {
  volatile s32 zero;

  // Clear the state header, scene state, flags and save metadata in DMA order:
  // 0xb0 words are gPersistentState up to the characters (units, which
  // Party_InitializeCharacters sets up); 0x3e1 words run from gSceneLocalState
  // through gInventorySnapshot and gActorSnapshot; 0x80 words are all
  // 4096 flags.
  zero = 0;
  START_DMA(&zero, &gPersistentState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0xb0);
  zero = 0;
  START_DMA(&zero, gSceneLocalState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x3e1);
  WAIT_DMA();
  zero = 0;
  START_DMA(&zero, gGameFlagBytes,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x80);
  ((struct MapLocalStateSnapshotView *)gSceneLocalState)->firstSnapshotSlot =
      0xff;
  zero = 0;
  START_DMA(&zero, gBattleArenaRecord + 0x1c,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x298);
  // Build the characters, then install input and message defaults.
  Party_InitializeCharacters();
  gPersistentState.aKeys = KEY_A;
  gPersistentState.bKeys = KEY_B;
  gPersistentState.selectKeys = KEY_SELECT;
  gPersistentState.startKeys = KEY_START;
  gPersistentState.firstShortcutKeys = KEY_L;
  gPersistentState.secondShortcutKeys = KEY_R;
  gPersistentState.dashKeys = KEY_B;
  gPersistentState.firstShortcut = 0;
  gPersistentState.secondShortcut = 0;
  gPersistentState.leaderActorId = 0;
  Party_AddMember(0);
  gPersistentState.coins = 0;
  gPersistentState.messageSpeed = 1;
  gPersistentState.speech = 1;
  gPersistentState.unusedAfterSpeech = 1;
  gPersistentState.windowColor[0] = 0;
  gPersistentState.windowColor[1] = 8;
  gPersistentState.buildStamp = 0;
  gPersistentState.startStamp = BuildStamp_GetTime();
  gPlayTimeFrames = 0;
  gPersistentState.playTime = 0;
  gIdleSleepEnabled = 0;
  gPersistentState.idleSleepEnabled = gIdleSleepEnabled;
  gIdleFrames = 0;
  gSaveSlot = -1;
  // Three prizes share each successive slot-machine weight.
  gPersistentState.prizeWeights[0] = 4;
  gPersistentState.prizeWeights[1] = 4;
  gPersistentState.prizeWeights[2] = 4;
  gPersistentState.prizeWeights[3] = 8;
  gPersistentState.prizeWeights[4] = 8;
  gPersistentState.prizeWeights[5] = 8;
  gPersistentState.prizeWeights[6] = 16;
  gPersistentState.prizeWeights[7] = 16;
  gPersistentState.prizeWeights[8] = 16;
  gPersistentState.prizeWeights[9] = 32;
  gPersistentState.prizeWeights[10] = 32;
  gPersistentState.prizeWeights[11] = 32;
  gPersistentState.prizeWeights[12] = 64;
  gPersistentState.prizeWeights[13] = 64;
  gPersistentState.prizeWeights[14] = 64;
}

/// Sets FLAG_CHILDHOOD and recomputes Isaac, Garet and Jenna, whose classes
/// then take their childhood forms. Called by the File Screen when a
/// new game starts the prologue.
void Party_StartChildhood(void) {
  GameFlag_Set(FLAG_CHILDHOOD);
  Unit_UpdateClass(ISAAC);
  Unit_UpdateClass(GARET);
  Unit_UpdateClass(JENNA);
  Unit_Calculate(ISAAC);
  Unit_Calculate(GARET);
  Unit_Calculate(JENNA);
}

/// The party set-up after the prologue: flags, Jenna recomputed, Isaac and
/// Garet healed with the Machete in their inventories replaced by the Short
/// Sword and equipped, Move and Retreat taught to Isaac, Move to Garet, Mind
/// Read to Ivan, and 300 coins. Called by Title_Run when the title shown after
/// the prologue hands over to Vale, three years later.
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

  gPersistentState.coins += 300;
}

/// Restores every party member's HP and PP; with `cureConditions` 1 it also
/// cures poison and haunting. Called by the events that heal the party
/// (leaving Vale, meeting Flint, the Colosso rounds, Faran in Lalivero), by
/// the Battle Arena after a round, and by InventorySnapshot_Save.
void Party_RestoreHpPp(s32 cureConditions) {
  s32 partySize;
  s32 partyIndex;
  struct Unit *unit;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    unit = Unit_Get(gPersistentState.party[partyIndex]);

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
/// Called by the events at the top of the Mercury and Venus Lighthouses.
void Party_RestorePpOfLitElements(void) {
  s32 partySize;
  s32 partyIndex;
  u8 unitId;
  s32 elementGroup;
  s32 restorePp;
  struct Unit *unit;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    unitId = gPersistentState.party[partyIndex];
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
/// that Unit_Calculate keeps the same fractions. Called after anything that
/// writes HP or PP directly: Unit_AddHp, the inn, the sanctum, item use,
/// battle.
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

/// Sets the unit's HP, clamped to 0..hpMax, and recomputes its HP and PP
/// gauges. Called by BattleObject_DestroyTemporaryObject.
void Unit_SetHp(struct Unit *unit, s32 hp) {
  s32 clamped;

  clamped = Unit_ClampGaugeValue(hp, unit->hpMax);
  unit->hp = clamped;
  Unit_UpdateGauges(unit);
}

/// Sets the unit's PP, clamped to 0..ppMax, and recomputes its HP and PP
/// gauges. Only GAME.S's bare veneer reaches it; nothing calls it by name.
void Unit_SetPp(struct Unit *unit, s32 pp) {
  s32 clamped;

  clamped = Unit_ClampGaugeValue(pp, unit->ppMax);
  unit->pp = clamped;
  Unit_UpdateGauges(unit);
}

/// Adds delta to the HP of unit unitId, clamped to 0..hpMax, recomputes its
/// gauges and returns the new HP. Called by battle's round end, poison and
/// regeneration, and by the field's damage floors and status damage.
s32 Unit_AddHp(s32 unitId, s32 delta) {
  struct Unit *unit = Unit_Get(unitId);
  s32 currentHp = unit->hp;
  s32 maximumHp = unit->hpMax;
  s32 value = currentHp + delta;
  s32 result;

  result = Unit_ClampGaugeValue(value, maximumHp);
  unit->hp = result;
  Unit_RecalculateGauges(unitId);
  return unit->hp;
}

/// Adds delta to the PP of unit unitId, clamped to 0..ppMax, recomputes its
/// gauges and returns the new PP. Called when Psynergy is used in the field,
/// in battle and from the menu, by PP recovery at the end of a battle round,
/// and by the debug menu.
s16 Unit_AddPp(s32 unitId, s32 delta) {
  struct Unit *unit = Unit_Get(unitId);
  s32 currentPp = unit->pp;
  s32 maximumPp = unit->ppMax;
  s32 value = currentPp + delta;
  s32 result;

  result = Unit_ClampGaugeValue(value, maximumPp);
  unit->pp = result;
  Unit_RecalculateGauges(unitId);
  return unit->pp;
}

#undef UNIT_STAT_DIFF
