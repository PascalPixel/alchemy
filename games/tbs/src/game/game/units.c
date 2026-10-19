// Part of game.c, the game object, compiled only through it.
// The party's units and their stats, a new game, the prologue, HP and PP.

#include "../game_internal.h"

// The running totals UnitCalc builds a unit's stats in.
struct StatWork {
  s32 hp;
  s32 pp;
  s32 atk;
  s32 def;
  s32 agi;
  s32 unk14;
  s32 luck;
  s32 turns;
  s32 hpRegen;
  s32 ppRegen;
  s32 element[4][2];
  s32 kind;
  s32 unk4c[2];
  s32 amount;
  struct ItemDefinition *item;
  s32 unk5c;
};

#define UNIT_STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

// Starts a new game: clears the game state and restarts at the opening.
void Game_Restart(void) {
  GameState_InitDefaults();
  Game_ResetForNewGameFar(0);
}

// Returns the party recovery bank, or battle unit 0x83 as the enemy-side
// recovery bank. The bank also stores the summon flags read by
// Summon_ListLearned.
void *Djinn_GetRecoveryTable(s32 enemySide) {
  if (enemySide != 0) {
    return UnitGet(0x83);
  }
  return &gGameState.unk008[4];
}

// Returns the average level of the party members, rounded down (0 with no
// members).
u32 Party_GetAverageLevel(void) {
  s32 partySize;
  s32 totalLevels;
  s32 partyIndex;

  totalLevels = 0;
  partySize = Party_CountActiveOwners();
  if (partySize == 0) {
    return 0;
  }
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    totalLevels += UnitGet(gGameState.activeOwners[partyIndex])->level;
  }
  totalLevels = totalLevels / partySize;
  return totalLevels;
}

// Returns the unit record of `owner`: 0 to 7 are the playable characters in
// the game state, 0x80 to 0x85 the enemies of the current battle. Anything
// else, or an enemy outside a battle, is NULL.
struct BattleUnit *UnitGet(u32 owner) {
  struct BattleUnit *states;
  struct BattleUnit *unit;

  {
    void *caller = __builtin_return_address(0);

    /* Native copies the unused return
     * address from lr into r3 at entry. This existing empty asm use
     * requires a Thumb low register, so allocation chooses r3 without
     * a pin (28/28 instructions, differ 0). With an unrestricted "r"
     * operand or no use, only that copy is absent (28/27, differ 1,
     * same 0x44-byte English extent with a final alignment nop). */
    /* FAKEMATCH: needed to match: low-register use preserves mov r3,lr; without
     * it, 28/27 instructions, differ 1. 2026-10-11: arm_return_addr emits the
     * lr copy after the argument copies and flow deletes it without a codeless
     * use; every plain use (compare, store, fold) either emits code or folds
     * away (differ 7). */
    asm("" : : "l"(caller));
  }
  states = gGameState.owners;
  if (owner < 8) {
    unit = &states[owner];
    return unit;
  }
  if (owner - 0x80 < 6 && gBattleOwnerStates != NULL) {
    unit = &((struct BattleUnit *)gBattleOwnerStates)[owner - 0x80];
    return unit;
  }
  return NULL;
}

// Returns the definition of enemy `selector - 8`; an id outside the 250
// enemies gives the first.
const u8 *Enemy_GetDefinition(s32 selector) {
  u32 recordIndex;

  recordIndex = selector - 8;
  if (recordIndex > 0xF9U) {
    recordIndex = 0;
  }
  return (const u8 *)&gEnemy_Definitions[recordIndex];
}

// Copies `count` bytes from `first` to `second` when `direction` is set, and
// from `second` to `first` otherwise.
void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
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

// Recomputes a unit's stats from its base stats, equipment, set Djinn, class
// and battle stat levels, and clamps each to its range. A change of maximum
// HP or PP keeps the unit's current HP and PP at the same fraction of it.
void UnitCalc(s32 owner) {
  struct StatWork *stats;
  struct BattleUnit *unit;
  s32 index, effectIndex, elementIndex;
  s32 value;
  s32 lighthouseLit;
  s32 effectKind;
  s32 effectAmount;
  s32 statLimit;
  s16 previousMaximum;

  // Start with the unit's base stats and elemental power/resistance.
  stats = Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
  unit = UnitGet(owner);
  stats->hp = unit->baseHp;
  stats->pp = unit->basePp;
  stats->atk = unit->baseAttack;
  stats->def = unit->baseDefense;
  stats->agi = unit->baseAgility;
  stats->luck = unit->baseLuck;
  stats->turns = unit->baseActionCount & 15;
  stats->hpRegen = unit->baseHpRegen;
  stats->ppRegen = unit->basePpRegen;
  for (index = 0; index < 4; index++) {
    stats->element[index][0] = unit->baseElement[index].power;
    stats->element[index][1] = unit->baseElement[index].resistance;
  }

  // Repair stale gauges before preserving their fractions of the new maxima.
  if (UNIT_STAT_DIFF(unit->maxHp * unit->hpGauge / 0x4000, unit->hp) > 1 ||
      UNIT_STAT_DIFF(unit->maxPp * unit->ppGauge / 0x4000, unit->pp) > 1) {
    unit->hpGauge = 0x4000;
    unit->ppGauge = 0x4000;
    unit->hp = unit->maxHp;
    unit->pp = unit->maxPp;
  }

  unit->restraint &= ~3;
  if (unit->restraint & 4)
    unit->restraint |= 1;
  if (unit->ready)
    stats->turns++;
  unit->equipBonus[0] = 0;
  unit->equipBonus[1] = 0;

  // Apply additive equipment and set-Djinn bonuses before class scaling.
  if (unit->classIndex) {
    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        continue;
      stats->item = Item_GetDirect(unit->inventory[index]);
      if (stats->item->flags & ITEM_CURSED)
        unit->restraint |= 3;
      /* FAKEMATCH: the one-pass block keeps the defense load after the
       * item's second bonus load, as the ROM schedules it. Measured:
       * plain, the scheduler lifts stats->def above that load (differ 2). */
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
        case ITEM_EFFECT_NONE:
          break;
        case ITEM_EFFECT_ADD_HP:
          stats->hp += stats->amount;
          break;
        case ITEM_EFFECT_ADD_HP_REGEN:
          stats->hpRegen += stats->amount;
          break;
        case ITEM_EFFECT_ADD_PP:
          stats->pp += stats->amount;
          break;
        case ITEM_EFFECT_ADD_PP_REGEN:
          stats->ppRegen += stats->amount;
          break;
        case ITEM_EFFECT_ADD_AGILITY:
          stats->agi += stats->amount;
          break;
        case ITEM_EFFECT_ADD_LUCK:
          stats->luck += stats->amount;
          break;
        case ITEM_EFFECT_ADD_VENUS_POWER:
          stats->element[0][0] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_MERCURY_POWER:
          stats->element[1][0] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_MARS_POWER:
          stats->element[2][0] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_JUPITER_POWER:
          stats->element[3][0] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_VENUS_RESIST:
          stats->element[0][1] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_MERCURY_RESIST:
          stats->element[1][1] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_MARS_RESIST:
          stats->element[2][1] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_JUPITER_RESIST:
          stats->element[3][1] += stats->amount;
          break;
        case ITEM_EFFECT_ADD_UNLEASH_CHANCE:
          unit->equipBonus[0] += stats->amount;
          break;
        case ITEM_EFFECT_24:
          unit->equipBonus[1] += stats->amount;
          break;
        case ITEM_EFFECT_25:
          unit->restraint |= 8;
          break;
        case ITEM_EFFECT_ADD_TURNS:
          stats->turns += stats->amount;
          break;
        }
      }
    }
    if (unit->restraint & 8)
      unit->restraint &= ~9;

    // Only set Djinn contribute their stat bonuses.
    for (elementIndex = 0; elementIndex < 4; elementIndex++) {
      u32 activeDjinn = unit->djinnActive[elementIndex];

      for (index = 0; index < 20; index++) {
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

    {
      struct ClassDefinition *classDefinition =
          Class_GetDefinition(unit->classIndex);

      stats->hp = stats->hp * classDefinition->hp / 10;
      stats->pp = stats->pp * classDefinition->pp / 10;
      stats->atk = stats->atk * classDefinition->atk / 10;
      stats->def = stats->def * classDefinition->def / 10;
      stats->agi = stats->agi * classDefinition->agi / 10;
      stats->luck = stats->luck * classDefinition->luck / 10;
    }

    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        continue;
      stats->item = Item_GetDirect(unit->inventory[index]);
      for (effectIndex = 0; effectIndex < 4; effectIndex++) {
        effectKind = stats->item->effects[effectIndex].kind;
        effectAmount = stats->item->effects[effectIndex].amount;
        stats->kind = effectKind;
        effectKind -= ITEM_EFFECT_SCALE_HP;
        stats->amount = effectAmount;
        switch (effectKind) {
        case 0:
          stats->hp = stats->hp * stats->amount / 10;
          break;
        case 1:
          stats->hpRegen = stats->hpRegen * stats->amount / 10;
          break;
        case 2:
          stats->pp = stats->pp * stats->amount / 10;
          break;
        case 3:
          stats->ppRegen = stats->ppRegen * stats->amount / 10;
          break;
        case 4:
          stats->atk = stats->atk * stats->amount / 10;
          break;
        case 5:
          stats->def = stats->def * stats->amount / 10;
          break;
        case 6:
          stats->agi = stats->agi * stats->amount / 10;
          break;
        case 7:
          stats->luck = stats->luck * stats->amount / 10;
          break;
        }
      }
    }
  }

  stats->atk = stats->atk * (unit->attackStage + 8) / 8;
  stats->def = stats->def * (unit->defenseStage + 8) / 8;
  stats->agi = stats->agi * (unit->agilityStage + 8) / 8;
  for (index = 0; index < 4; index++)
    stats->element[index][0] +=
        (unit->elementLevels[index] * unit->elementLevels[index] +
         unit->elementLevels[index]) *
        5;
  for (index = 0; index < 4; index++)
    stats->element[index][1] += unit->resistanceStage * 20;

  if (unit->classIndex) {
    lighthouseLit = 0;
    // Faster PP recovery once the character's own element's lighthouse is lit
    switch (unit->classId) {
    case ISAAC:
      lighthouseLit = GameFlag_Test(FLAG_VENUS_LIGHTHOUSE_LIT);
      break;
    case GARET:
      lighthouseLit = GameFlag_Test(FLAG_MARS_LIGHTHOUSE_LIT);
      break;
    case IVAN:
      lighthouseLit = GameFlag_Test(FLAG_JUPITER_LIGHTHOUSE_LIT);
      break;
    case MIA:
      lighthouseLit = GameFlag_Test(FLAG_MERCURY_LIGHTHOUSE_LIT);
      break;
    case JENNA:
      lighthouseLit = GameFlag_Test(FLAG_MARS_LIGHTHOUSE_LIT);
      break;
    }
    if (lighthouseLit)
      stats->ppRegen += 4;
  }

  CLAMP(stats->atk, 0, 999);
  CLAMP(stats->def, 0, 999);
  CLAMP(stats->agi, 0, 999);
  CLAMP(stats->luck, 0, 99);
  CLAMP(stats->turns, 0, 2);
  CLAMP(stats->hpRegen, 0, 10000);
  CLAMP(stats->ppRegen, 0, 200);
  for (index = 0; index < 4; index++) {
    CLAMP(stats->element[index][0], 0, 200);
    CLAMP(stats->element[index][1], 0, 200);
  }

  unit->atk = stats->atk;
  unit->def = stats->def;
  unit->agi = stats->agi;
  unit->luck = stats->luck;
  unit->actionEntryCount = stats->turns;
  unit->hpRegen = stats->hpRegen;
  unit->ppRegen = stats->ppRegen;
  for (index = 0; index < 4; index++) {
    unit->element[index].power = stats->element[index][0];
    unit->element[index].resistance = stats->element[index][1];
  }

  statLimit = 9999;
  if (unit->classIndex)
    statLimit = 1999;

  previousMaximum = unit->maxHp;
  CLAMP(stats->hp, 0, statLimit);
  unit->maxHp = stats->hp;
  if (previousMaximum != unit->maxHp) {
    value = stats->hp * unit->hpGauge / 0x4000;
    CLAMP(value, 0, statLimit);
    if (unit->hp != 0 && value == 0)
      value = 1;
    unit->hp = value;
  }

  previousMaximum = unit->maxPp;
  CLAMP(stats->pp, 0, statLimit);
  unit->maxPp = stats->pp;
  if (previousMaximum != unit->maxPp) {
    value = stats->pp * unit->ppGauge / 0x4000;
    CLAMP(value, 0, statLimit);
    if (unit->pp != 0 && value == 0)
      value = 1;
    unit->pp = value;
  }

  MemFree(stats);
}

// Sets flag 0x167 when any party member has the Lure Cap (item 164, the one
// item with equipment effect 27) equipped, and clears it otherwise.
void GameFlag_RefreshLureCap(void) {
  s32 partySize;
  s32 partyIndex;

  GameFlag_ClearBit(0x167);
  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct BattleUnit *unit;
    s32 inventorySlot;

    unit = UnitGet(gGameState.activeOwners[partyIndex]);
    for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        struct ItemEffect *effect;
        s32 effectIndex;

        effect = Item_GetDirect(unit->inventory[inventorySlot])->effects;
        for (effectIndex = 0; effectIndex < 4; effectIndex++) {
          u8 effectKind;

          effectKind = effect->kind;
          effect++;
          if (effectKind == ITEM_EFFECT_27) {
            GameFlag_SetBit(0x167);
          }
        }
      }
    }
  }
}

// Encodes the build-stamp time as ((hours * 16 + minutes) * 64 + seconds)
// with bit 12 set. Debug mode also sets bits 15..31 before the halfword cast.
// A new game stores this value as its start stamp.
u32 Runtime_GetBuildStampTime(void) {
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

// The end of the scene state that a new game sets up.
struct SceneStateTail {
  u8 unk000[0x100];
  u32 playTime;
  u8 unk104;
};

// Clears the game-state header, scene state, flags and save stamp; sets a new
// game's defaults: Isaac alone in the party, the standard button layout,
// the message speed, the play time clock and the slot machine prize odds.
void GameState_InitDefaults(void) {
  volatile s32 zero;

  // Clear the state header, scene work, flags and save metadata in DMA order.
  zero = 0;
  START_DMA(&zero, &gGameState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0xb0);
  zero = 0;
  START_DMA(&zero, gSceneState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x3e1);
  WAIT_DMA();
  zero = 0;
  START_DMA(&zero, gGameFlagBytes,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x80);
  ((struct SceneStateTail *)gSceneState)->unk104 = 0xff;
  zero = 0;
  START_DMA(&zero, gSaveStamp + 0x1c,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x298);
  // Build the characters, then install input and message defaults.
  Party_InitCharacters();
  gGameState.aKeys = KEY_A;
  gGameState.bKeys = KEY_B;
  gGameState.selectKeys = KEY_SELECT;
  gGameState.startKeys = KEY_START;
  gGameState.firstShortcutKeys = KEY_L;
  gGameState.secondShortcutKeys = KEY_R;
  gGameState.dashKeys = KEY_B;
  gGameState.firstShortcut = 0;
  gGameState.secondShortcut = 0;
  gGameState.selectedActor = 0;
  Party_AddActiveOwner(0);
  gGameState.coins = 0;
  gGameState.messageSpeed = 1;
  gGameState.speech = 1;
  gGameState.unk20b = 1;
  gGameState.paletteGlow[0] = 0;
  gGameState.paletteGlow[1] = 8;
  gGameState.buildStamp = 0;
  gGameState.startStamp = Runtime_GetBuildStampTime();
  gPlayTimeFrames = 0;
  gGameState.playTime = 0;
  gIdleSleepEnabled = 0;
  gGameState.idleSleepEnabled = gIdleSleepEnabled;
  gIdleFrames = 0;
  gSaveSlot = -1;
  // Three prizes share each successive slot-machine weight.
  gGameState.prizeWeights[0] = 4;
  gGameState.prizeWeights[1] = 4;
  gGameState.prizeWeights[2] = 4;
  gGameState.prizeWeights[3] = 8;
  gGameState.prizeWeights[4] = 8;
  gGameState.prizeWeights[5] = 8;
  gGameState.prizeWeights[6] = 16;
  gGameState.prizeWeights[7] = 16;
  gGameState.prizeWeights[8] = 16;
  gGameState.prizeWeights[9] = 32;
  gGameState.prizeWeights[10] = 32;
  gGameState.prizeWeights[11] = 32;
  gGameState.prizeWeights[12] = 64;
  gGameState.prizeWeights[13] = 64;
  gGameState.prizeWeights[14] = 64;
}

// Sets FLAG_CHILDHOOD and recomputes Isaac, Garet and Jenna.
void Party_BeginChildhood(void) {
  GameFlag_SetBit(FLAG_CHILDHOOD);
  Unit_UpdateClass(ISAAC);
  Unit_UpdateClass(GARET);
  Unit_UpdateClass(JENNA);
  UnitCalc(ISAAC);
  UnitCalc(GARET);
  UnitCalc(JENNA);
}

// The party set-up after the prologue: flags, Jenna recomputed, Isaac and
// Garet healed with the Machete (item 15) in their inventories replaced by
// the Short Sword (item 16) and equipped, four Psynergy entries taught across
// Isaac, Garet and Ivan, and 300 coins.
void Party_EndChildhood(void) {
  s32 owner;

  GameFlag_ClearBit(FLAG_CHILDHOOD);
  GameFlag_ClearBit(0x21);
  GameFlag_SetBit(0x901);
  Unit_UpdateClass(JENNA);
  UnitCalc(JENNA);
  GameFlag_ClearBit(0x11b);
  GameFlag_SetBit(0x11a);

  for (owner = 0; owner <= 1; owner++) {
    struct BattleUnit *unit = UnitGet(owner);
    s32 slot;

    unit->hp = unit->maxHp;
    unit->pp = unit->maxPp;
    Unit_UpdateGauges(unit);

    for (slot = 0; slot <= 14; slot++) {
      if ((unit->inventory[slot] & ITEM_ID_MASK) == 15) {
        unit->inventory[slot] = 16;
        Inventory_Equip(owner, slot);
        break;
      }
    }

    Unit_UpdateClass(owner);
    UnitCalc(owner);
  }

  Unit_LearnPsynergy(ISAAC, ABILITY_MOVE);
  Unit_LearnPsynergy(ISAAC, ABILITY_RETREAT);
  Unit_LearnPsynergy(GARET, ABILITY_MOVE);
  Unit_LearnPsynergy(IVAN, ABILITY_MIND_READ);

  gGameState.coins += 300;
}

// Restores every party member's HP and PP; with `cureConditions` 1 it cures
// poison and haunting.
void Party_RestoreHpPp(s32 cureConditions) {
  s32 partySize;
  s32 partyIndex;
  struct BattleUnit *unit;

  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    unit = UnitGet(gGameState.activeOwners[partyIndex]);

    unit->hp = unit->maxHp;
    unit->pp = unit->maxPp;
    Unit_UpdateGauges(unit);

    if (cureConditions == 1) {
      unit->poison = 0;
      unit->haunt = 0;
    }
  }
}

// Restores the PP of each party member whose element group has a lit
// lighthouse: Venus or Mars for group 0, Mercury or Jupiter for the others.
void Party_RestorePpOfLitElements(void) {
  s32 partySize;
  s32 partyIndex;
  u8 ownerId;
  s32 elementGroup;
  s32 restorePp;
  struct BattleUnit *unit;

  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    ownerId = gGameState.activeOwners[partyIndex];
    elementGroup = gCharacter_ElementGroupTable[ownerId];
    restorePp = 0;
    if (elementGroup == 0) {
      if (GameFlag_Test(FLAG_VENUS_LIGHTHOUSE_LIT) ||
          GameFlag_Test(FLAG_MARS_LIGHTHOUSE_LIT)) {
        restorePp = 1;
      }
    } else {
      if (GameFlag_Test(FLAG_MERCURY_LIGHTHOUSE_LIT) ||
          GameFlag_Test(FLAG_JUPITER_LIGHTHOUSE_LIT)) {
        restorePp = 1;
      }
    }

    if (restorePp == 0) {
      continue;
    }

    unit = UnitGet(ownerId);
    unit->pp = unit->maxPp;
    Unit_UpdateGauges(unit);
  }
}

// Unused reserved entry; keep its place in the far-call table.
UNUSED void Owner_RatioNoOp(void) {}

// Recomputes the HP and PP gauges of the selected unit from its current values.
void UnitRatio(s32 owner) {
  struct BattleUnit *unit;

  unit = UnitGet(owner);
  Unit_UpdateGauges(unit);
}

// Restricts an HP or PP value to the unit's current maximum.
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

// Sets the unit's HP, clamped to 0..maxHp, and recomputes its HP and PP
// gauges.
void Unit_SetHp(struct BattleUnit *unit, s32 hp) {
  s32 clamped;

  clamped = Unit_ClampGaugeValue(hp, unit->maxHp);
  unit->hp = clamped;
  Unit_UpdateGauges(unit);
}

// Sets the unit's PP, clamped to 0..maxPp, and recomputes its HP and PP
// gauges.
void Unit_SetPp(struct BattleUnit *unit, s32 pp) {
  s32 clamped;

  clamped = Unit_ClampGaugeValue(pp, unit->maxPp);
  unit->pp = clamped;
  Unit_UpdateGauges(unit);
}

// Adds delta to the HP of unit owner, clamped to 0..maxHp, recomputes its
// gauges and returns the new HP.
s32 Unit_AddHp(s32 owner, s32 delta) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 currentHp = unit->hp;
  s32 maximumHp = unit->maxHp;
  s32 value = currentHp + delta;
  s32 result;

  result = Unit_ClampGaugeValue(value, maximumHp);
  unit->hp = result;
  UnitRatio(owner);
  return unit->hp;
}

// Adds delta to the PP of unit owner, clamped to 0..maxPp, recomputes its
// gauges and returns the new PP.
s16 Unit_AddPp(s32 owner, s32 delta) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 currentPp = unit->pp;
  s32 maximumPp = unit->maxPp;
  s32 value = currentPp + delta;
  s32 result;

  result = Unit_ClampGaugeValue(value, maximumPp);
  unit->pp = result;
  UnitRatio(owner);
  return unit->pp;
}

#undef UNIT_STAT_DIFF
