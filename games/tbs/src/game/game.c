#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "scene.h"
#include "owner_state.h"
#include "character.h"
#include "party_state.h"
#include "runtime_interfaces.h"
#include "item.h"
#include "game_flags.h"
#include "inventory.h"
#include "resource.h"
#include "resource_ids.h"
#include "dma.h"
#include "game_state.h"
#include "frame.h"
#include "save_state.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_unit.h"
#include "lib.h"
#include "edition.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "battle_summon.h"
#include "preset_table.h"
#include "iwram_call.h"
#include "battle_calc.h"
#include "battle_random.h"
#include "battle_type.h"
#include "battle_effect_chance.h"
#include "battle_party.h"
#include "field_event.h"
#include "unit/game/starting_equipment.h"
#include "flags.h"
#include "character_ids.h"
#include "ability_ids.h"
#include "macros.h"

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

#define STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

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
  if (STAT_DIFF(unit->maxHp * unit->hpGauge / 0x4000, unit->hp) > 1 ||
      STAT_DIFF(unit->maxPp * unit->ppGauge / 0x4000, unit->pp) > 1) {
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

// Sets flag 0x167 when any party member has an item with equipment effect 27
// equipped, and clears it otherwise.
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
  START_DMA(&zero, &gGameState, 0x850000b0);
  zero = 0;
  START_DMA(&zero, gSceneState, 0x850003e1);
  WAIT_DMA();
  zero = 0;
  START_DMA(&zero, gGameFlagBytes, 0x85000080);
  ((struct SceneStateTail *)gSceneState)->unk104 = 0xff;
  zero = 0;
  START_DMA(&zero, gSaveStamp + 0x1c, 0x85000298);
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

// Sets flag 0x20 and recomputes characters 0, 1 and 5 (Isaac, Garet and
// Jenna).
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
// Garet healed with item 15 in their inventories replaced by item 16 and
// equipped, four Psynergy entries taught across Isaac, Garet and Ivan, and
// 300 coins.
void Party_EndChildhood(void) {
  s32 owner;

  GameFlag_ClearBit(FLAG_CHILDHOOD);
  GameFlag_ClearBit(33);
  GameFlag_SetBit(0x901);
  Unit_UpdateClass(JENNA);
  UnitCalc(JENNA);
  GameFlag_ClearBit(0x11b);
  GameFlag_SetBit(282);

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

// Restores the PP of each party member whose element group has one of its
// flags set (0x110 or 0x112 for group 0, 0x111 or 0x113 for the others).
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

// Returns item `item`'s definition; an inventory entry may be passed, its
// equipped and quantity bits are ignored.
struct ItemDefinition *Item_GetDirect(s32 item) {
  return gItem_DefinitionTable + (item & ITEM_ID_MASK);
}

// Returns whether the character can equip `item`: bit classId of the item's
// equipMask. Units past the eight characters cannot.
s32 Item_CanOwnerEquipDirect(s32 owner, s32 item) {
  struct BattleUnit *unit = UnitGet(owner);
  struct ItemDefinition *itemDefinition = Item_GetDirect(item);
  s32 equipMask = itemDefinition->equipMask;

  if (unit->classId >= 8) {
    return 0;
  }
  return (equipMask >> unit->classId) & 1;
}

// Returns 1 for an item that is not equipment, and for equipment whether the
// character can equip it.
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item) {
  if (Item_GetEquipmentGroup(item) == 0) {
    return 1;
  }
  return Item_CanOwnerEquipDirect(unit, item);
}

// Returns what kind of thing an item is: 1 a weapon, 2 armor (types 2 to 5
// and 9), 0 anything else.
s32 Item_GetEquipmentGroup(s32 itemId) {
  s32 group;
  struct ItemDefinition *item;

  item = Item_GetDirect(itemId);
  group = 0;
  if (item->type == 1)
    group = 1;
  else if (item->type == 2)
    group = 2;
  else if (item->type == 3)
    group = 2;
  else if (item->type == 4)
    group = 2;
  else if (item->type == 5)
    group = 2;
  else if (item->type == 9)
    group = 2;
  return group;
}

// Returns how many of the item in `slot` the unit carries (0 for an empty
// slot). The entry stores one less than the count.
s32 Inventory_GetQuantity(s32 owner, s32 slot) {
  s32 itemId;

  owner = UnitGet(owner)->inventory[slot];
  itemId = ITEM_ID_MASK;
  itemId &= owner;
  owner = (u32)owner >> INVENTORY_QUANTITY_SHIFT;
  owner++;
  if (itemId == 0) {
    owner = 0;
  }
  return owner;
}

// Counts filled inventory slots up to the first empty slot.
s32 Inventory_Count(s32 owner) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 count = 0;

  if (unit->inventory[count] != 0) {
    do {
      count++;
      if (count > 14)
        break;
    } while (unit->inventory[count] != 0);
  }
  return count;
}

// Returns 1 when the selected character or any party member has a free
// inventory slot.
s32 PartyInventory_HasSpace(void) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;

  if (Inventory_Count(gGameState.selectedActor) != INVENTORY_SLOTS)
    return 1;
  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      if (Inventory_Count(*ownerCursor++) != INVENTORY_SLOTS)
        return 1;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return 0;
}

// Returns the number of free inventory slots across the party.
s32 PartyInventory_CountFreeSlots(void) {
  s16 owners[10];
  s32 ownerCount = Party_ListActiveOwners(owners);
  s32 freeSlots = 0;
  s16 *ownerCursor = owners;

  if (freeSlots < ownerCount) {
    s32 remainingOwners = ownerCount;

    do {
      freeSlots = freeSlots - Inventory_Count(*ownerCursor++) + 15;
      remainingOwners--;
    } while (remainingOwners != 0);
  }
  return freeSlots;
}

// Gives a unit one item, joining its first matching stack (up to 30) or
// taking the first free slot. A full matching stack fails immediately, even
// if another slot is free. Returns the slot, or -1 when there was no room.
s32 Inventory_AddItem(s32 owner, s32 itemId) {
  struct BattleUnit *unit = UnitGet(owner);
  struct ItemDefinition *item = Item_GetDirect(itemId);
  s32 slot;

  if ((item->flags & ITEM_STACKABLE) != 0) {
    slot = 0;
    if (((unit->inventory[slot] ^ itemId) & ITEM_ID_MASK) != 0) {
      do {
        slot++;
        if (slot >= INVENTORY_SLOTS)
          break;
      } while (((unit->inventory[slot] ^ itemId) & ITEM_ID_MASK) != 0);
    }
    if (slot != INVENTORY_SLOTS) {
      s32 entry = unit->inventory[slot];
      u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

      if (count > 29)
        return -1;
      {
        s32 value = INVENTORY_ENTRY_MASK;

        value &= entry;
        value |= count << INVENTORY_QUANTITY_SHIFT;
        unit->inventory[slot] = value;
      }
      return slot;
    }
  }

  slot = 0;
  do {
    if (unit->inventory[slot] == 0) {
      unit->inventory[slot] = itemId;
      return slot;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return -1;
}

// Gives the item to the first party member with room. Returns who took it,
// or -1.
s32 PartyInventory_Add(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;

  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      s16 ownerId = *ownerCursor++;

      if (Inventory_AddItem(ownerId, itemId) >= 0)
        return ownerId;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return -1;
}

// Returns the first slot holding `itemId`, or -1.
s32 Inventory_Find(s32 owner, s32 itemId) {
  struct BattleUnit *inv = UnitGet(owner);
  s32 slot = 0;
  u16 *entry = inv->inventory;

  do {
    if (((*entry++) & ITEM_ID_MASK) == itemId) {
      return slot;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return -1;
}

// Returns who carries `itemId`, asking the selected character first, or -1.
s32 PartyInventory_FindOwner(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;
  s16 owner;

  if (Inventory_Find(gGameState.selectedActor, itemId) != -1)
    return gGameState.selectedActor;
  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      owner = *ownerCursor++;
      if (Inventory_Find(owner, itemId) != -1)
        return owner;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return -1;
}

// Equips the item in `slot`, replacing equipment of the same type. Type 6
// items grant Psynergy and may coexist. Returns 0, -1 when the unit cannot
// equip it, or -2 when the item it would replace is cursed on.
s32 Inventory_Equip(s32 owner, s32 slot) {
  struct BattleUnit *unit = UnitGet(owner);
  u32 inventoryEntry = unit->inventory[slot];
  struct ItemDefinition *item;
  u8 type;
  s32 equippedSlot;

  if (Item_CanOwnerEquipDirect(owner, inventoryEntry) == 0)
    return -1;
  if (inventoryEntry & INVENTORY_EQUIPPED)
    return 0;

  item = Item_GetDirect(inventoryEntry);
  type = item->type;
  if (type != 6) {
    for (equippedSlot = 0; equippedSlot <= 14; equippedSlot++) {
      if ((unit->inventory[equippedSlot] & INVENTORY_EQUIPPED) == 0)
        continue;
      if (Item_GetDirect(unit->inventory[equippedSlot])->type == type)
        break;
    }

    if (equippedSlot != INVENTORY_SLOTS) {
      item = Item_GetDirect(unit->inventory[equippedSlot]);
      if (item->flags & ITEM_CANNOT_UNEQUIP)
        return -2;
      unit->inventory[equippedSlot] &= 0xfdff;
    }
  }

  unit->inventory[slot] |= INVENTORY_EQUIPPED;
  Unit_RefreshPsynergy(owner);
  UnitCalc(owner);
  return 0;
}

// Returns the slot of the equipped item of `type`, or -1.
s32 Inventory_FindEquipped(s32 owner, s32 type) {
  struct BattleUnit *base = UnitGet(owner);
  s32 index;
  struct ItemDefinition *item;

  for (index = 0; index < INVENTORY_SLOTS; index++) {
    if (base->inventory[index] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(base->inventory[index]);
      if (item->type == type)
        break;
    }
  }
  if (index == 15)
    index = -1;
  return index;
}

// Returns the definition of the equipped item of `type`, or NULL.
struct ItemDefinition *Inventory_GetEquippedDefinition(struct BattleUnit *unit,
                                                       s32 type) {
  s32 slot;
  struct ItemDefinition *item;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(unit->inventory[slot]);
      if (item->type == type) {
        return item;
      }
    }
  }
  return 0;
}

// Returns the id of the equipped item of `type`, or 0.
s32 Inventory_GetEquippedItem(struct BattleUnit *unit, s32 type) {
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] & INVENTORY_EQUIPPED) {
      struct ItemDefinition *item = Item_GetDirect(unit->inventory[slot]);

      if (item->type == type) {
        return unit->inventory[slot] & ITEM_ID_MASK;
      }
    }
  }
  return 0;
}

// Keeps occupied slots in order and clears the unused inventory tail.
static inline void Inventory_CompactSlots(s16 *inventory) {
  s32 usedSlots = 0;
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (inventory[slot] != 0)
      inventory[usedSlots++] = inventory[slot];
  }
  for (; usedSlots < INVENTORY_SLOTS; usedSlots++)
    inventory[usedSlots] = 0;
}

// Takes one item out of `slot`: a stack shrinks by one (returns 1), a last
// item empties the slot and the inventory closes up (returns 2). Returns -1
// for an empty slot. The unit's stats are recomputed either way.
s32 Inventory_Remove(s32 owner, s32 slot) {
  struct BattleUnit *unit = UnitGet(owner);
  u16 item = unit->inventory[slot];
  s32 result = -1;

  if (item != 0) {
    if (item & INVENTORY_QUANTITY_MASK) {
      unit->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
      result = 1;
    } else {
      unit->inventory[slot] = 0;
      Inventory_CompactSlots(unit->inventory);
      result = 2;
    }
  }
  UnitCalc(owner);
  return result;
}

// Removes one item as Inventory_Remove does, and puts a rare item back into
// circulation (its counter goes up by one) for the shops to sell.
s32 Inventory_Discard(s32 owner, s32 slot) {
  s32 item = UnitGet(owner)->inventory[slot];
  s32 removalResult = Inventory_Remove(owner, slot);

  if (removalResult != -1) {
    Item_AdjustCounter(item, 1);
    Event_ClearInvalidPackedValuesFar();
  }
  return removalResult;
}

// Returns whether the item in `slot` may be dropped: 0 yes, -1 the slot is
// empty, -4 the item is indispensable, -3 it is equipped and cursed on.
s32 Inventory_CheckDiscard(s32 owner, s32 slot) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirect(itemId);

  if (itemId == 0) {
    return -1;
  }
  if ((item->flags & ITEM_INDISPENSABLE) != 0) {
    return -4;
  }
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
      (item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
    return -3;
  }
  return 0;
}

// Removes one `itemId` from whoever in the party carries it.
s32 PartyInventory_Remove(s32 itemId) {
  s32 owner = PartyInventory_FindOwner(itemId);

  if (owner == -1)
    return 0;
  Inventory_Remove(owner, Inventory_Find(owner, itemId));
  return 0;
}

// Discards one `itemId` from whoever in the party carries it.
s32 PartyInventory_Discard(s32 itemId) {
  s32 owner = PartyInventory_FindOwner(itemId);

  if (owner == -1)
    return 0;
  Inventory_Discard(owner, Inventory_Find(owner, itemId));
  return 0;
}

// Marks the item in `slot` broken. Returns -1 for an empty slot.
s32 Inventory_Break(s32 owner, s32 slot) {
  struct BattleUnit *inv = UnitGet(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] |= INVENTORY_BROKEN;
  return 0;
}

// Repairs the item in `slot`. Returns -1 for an empty slot.
s32 Inventory_Repair(s32 owner, s32 slot) {
  struct BattleUnit *inv = UnitGet(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] &= ~INVENTORY_BROKEN;
  return 0;
}

// Returns how the action an item performs when used picks its targets.
u8 Item_GetTargetMode(s32 itemId) {
  return BattleAction_GetDirect(Item_GetDirect(itemId)->actionId)->targetMode;
}

// Adds `delta` to rare item counter `index`, kept between 0 and 99, and
// returns the new count.
s32 ItemCounter_Adjust(s32 index, s32 delta) {
  s32 counter_slot = index;
  u8 *data = gItemCounters;

  index = 0;
  if (counter_slot <= 127) {
    s32 value = data[counter_slot];

    value += delta;

    if (value < 0) {
      value = 0;
    } else if (value > 99) {
      value = 99;
      index = 99;
    } else {
      index = value;
    }
    data[counter_slot] = value;
  }
  return index;
}

// Adds `delta` to the counter of a rare item; ordinary items have none.
// Returns the new count, or 0.
s32 Item_AdjustCounter(s32 itemId, s32 delta) {
  s32 itemIdMask = ITEM_ID_MASK;
  u8 counter;
  s32 result = 0;

  counter = gItem_ArtifactSlotTable[itemId & itemIdMask];
  if (counter != 0) {
    result = ItemCounter_Adjust(counter - 1, delta);
  }
  return result;
}

// Counts matching nonstackable items, or the quantity in the first matching
// stack.
s32 Inventory_CountItem(s32 owner, s32 itemId) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 count = 0;
  s32 itemIdOnly = itemId & ITEM_ID_MASK;
  s32 slot = 0;

  do {
    if ((unit->inventory[slot] & ITEM_ID_MASK) == itemIdOnly) {
      struct ItemDefinition *item = Item_GetDirect(itemIdOnly);

      if (item->flags & ITEM_STACKABLE) {
        count = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
        break;
      }
      count++;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return count;
}

// Returns how many of `itemId` the party carries.
s32 PartyInventory_CountItem(s32 itemId) {
  u16 owners[16];
  s32 itemCount = 0;
  s32 ownerCount = Party_ListActiveOwners(owners);

  if (itemCount < ownerCount) {
    u16 *ownerCursor = owners;
    s32 remainingOwners = ownerCount;

    do {
      itemCount += Inventory_CountItem(*ownerCursor++, itemId);
      remainingOwners--;
    } while (remainingOwners != 0);
  }
  return itemCount;
}

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
  struct LevelUpResult gains; // Each level's gains are discarded.
  s32 currentLevel = UnitGet(owner)->level;

  while (currentLevel < targetLevel) {
    Owner_LevelUp(owner, &gains);
    currentLevel++;
  }
  UnitCalc(owner);
}

// The game flags: 4096 bits in gGameFlagBytes. A flag number's low three bits
// pick the bit and the next nine the byte; byte and nibble flags use the same
// numbering.

// Returns 1 when `flag` is set.
s32 GameFlag_Test(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;
  s32 flagValue;

  flagMask = 1 << (7 & flag);
  flag = GAME_FLAG_BYTE(flag);
  flagBytes = gGameFlagBytes;
  flagValue = flagBytes[flag] & flagMask;
  return (u32)((0 - flagValue) | flagValue) >> 0x1F;
}

// Sets `flag`. Returns its byte number.
s32 GameFlag_SetBit(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;

  flagMask = 1 << (7 & flag);
  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  flagBytes[flag] |= flagMask;
  return flag;
}

// Clears `flag`.
void GameFlag_ClearBit(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;

  flagMask = ~(1 << (7 & flag));
  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  flagBytes[flag] &= flagMask;
}

// Flips `flag` and returns its new value.
u32 GameFlag_Toggle(s32 flag) {
  s32 flagMask;
  u8 *flagBytes;
  s32 flagValue;

  flagMask = 1 << (7 & flag);
  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  flagBytes[flag] ^= flagMask;
  flagValue = flagBytes[flag] & flagMask;
  return (u32)((0 - flagValue) | flagValue) >> 31;
}

// Returns the whole byte `flag` is in.
s32 GameFlag_GetByte(s32 flag) {
  u32 byteIndex;

  byteIndex = (u32)flag << 20;
  flag = byteIndex >> 23;
  return gGameFlagBytes[flag];
}

// Sets the whole byte `flag` is in.
void GameFlag_SetByte(s32 flag, s32 value) {
  u32 shifted = (u32)flag << 20;
  flag = shifted >> 23;
  gGameFlagBytes[flag] = value;
}

// Adds 1 to the byte `flag` is in, stopping at 255, and returns it.
u8 GameFlag_IncrementByte(s32 flag) {
  u8 *flagBytes;

  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  if (flagBytes[flag] <= 0xFE) {
    flagBytes[flag]++;
  }
  return flagBytes[flag];
}

// Takes 1 from the byte `flag` is in, stopping at 0, and returns it.
u8 GameFlag_DecrementByte(s32 flag) {
  u8 *flagBytes;

  flagBytes = gGameFlagBytes;
  flag = GAME_FLAG_BYTE(flag);
  if (flagBytes[flag] != 0) {
    flagBytes[flag] += 0xFF;
  }
  return flagBytes[flag];
}

// Returns the four bits `flag` is in (bit 2 of the number picks the half).
s32 GameFlag_GetNibble(u32 flag) {
  u32 shifted = (u32)flag << 20;
  u32 shift;
  u32 mask;
  s32 result;

  shift = flag & 4;
  mask = 15;
  flag = shifted >> 23;
  result = gGameFlagBytes[flag];
  result &= mask << shift;
  return result >> shift;
}

// Sets the four bits `flag` is in.
void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 fieldMask = 0xF;
  s32 shift = 4 & flag;
  s32 mask = fieldMask << shift;
  u8 *bytes = gGameFlagBytes;

  flag = GAME_FLAG_BYTE(flag);
  bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}

// The letter that tells apart enemies of one kind: digits from 1 abroad, the
// alphabet in Japan.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define ENEMY_SUFFIX_FIRST '1'
#define ENEMY_SUFFIX_LAST 8
#else
#define ENEMY_SUFFIX_FIRST 'A'
#define ENEMY_SUFFIX_LAST 25
#endif

// Fills battle unit `unitId` (0x80 to 0x86) with enemy `enemyId`: stats,
// name with its suffix letter, the items it carries, rewards and elements.
// Enemies 158 to 171 fight on side 2. Returns 0 for an id out of range.
s32 BattleUnitSet(s32 unitId, s32 enemyId, s32 suffix) {
  struct BattleUnit *unit;
  const struct EnemyDefinition *enemy;
  u16 name[15];
  s32 i;
  u32 index;
  s32 q;
  s32 count;

  index = enemyId - 8;
  if (unitId <= 127)
    return 0;
  if (unitId > 134)
    return 0;
  if (index > 242)
    return 0;
  unit = UnitGet(unitId);
  Iwram_ClearWords(unit, BATTLE_UNIT_SIZE);
  if (index > 164)
    index = 0;
  enemy = &gEnemy_Definitions[index];
  unit->level = enemy->level;
  unit->baseHp = enemy->hp;
  unit->hp = enemy->hp;
  unit->maxHp = enemy->hp;
  unit->basePp = enemy->pp;
  unit->pp = enemy->pp;
  unit->maxPp = enemy->pp;
  unit->hpGauge = 0x4000;
  unit->ppGauge = 0x4000;
  unit->baseAttack = enemy->atk;
  unit->baseDefense = enemy->def;
  unit->baseAgility = enemy->agi;
  unit->baseLuck = enemy->luck;
  unit->baseActionCount = enemy->turns;
  unit->baseHpRegen = enemy->hpRegen;
  unit->basePpRegen = enemy->ppRegen;
  UiText_DecodeMessageFar(index + MsgEnemyName, name, 15);
  for (i = 0; i < 14 && name[i] != 0; i++)
    unit->name[i] = name[i];
  if (suffix <= ENEMY_SUFFIX_LAST) {
    unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
    i++;
  }
  unit->name[i] = 0;
  unit->name[14] = 0;
  count = 0;
  for (i = 0; i < 4; i++) {
    if (enemy->items[i] != 0) {
      for (q = 0; q < enemy->counts[i]; q++) {
        if (count < 15)
          unit->inventory[count++] = enemy->items[i];
      }
    }
  }
  unit->rewards = enemy->rewards;
  unit->classIndex = 0;
  unit->classId = enemyId;
#ifdef UBFIX
  Unit_CalcBaseElements(unitId, unit->baseElement);
#else
  Unit_CalcBaseElements(unitId, (s16 (*)[2])unit->baseElement);
#endif
  UnitCalc(unitId);
  unit->side = 1;
  switch (unit->classId) {
  case 158:
  case 159:
  case 160:
  case 161:
  case 162:
  case 163:
  case 164:
  case 165:
  case 166:
  case 167:
  case 168:
  case 169:
  case 170:
  case 171:
    unit->side = 2;
    break;
  }
  return 1;
}

// The party: flags 0 to 7 say who is in it, and gGameState.activeOwners
// lists them in order.

// Returns the number of party members.
s32 Party_CountActiveOwners(void) {
  s32 owner;
  s32 count;

  count = 0;
  owner = 0;
  do {
    if (GameFlag_Test(owner) != 0)
      count++;
    owner++;
  } while (owner <= 7);
  return count;
}

// Adds character `owner` to the end of the party. Returns the party size.
s32 Party_AddActiveOwner(s32 owner) {
  s32 partySize = Party_CountActiveOwners();
  s32 partyIndex;

  GameFlag_SetBit(owner);
  partyIndex = 0;
  while (partyIndex < partySize) {
    if (gGameState.activeOwners[partyIndex] == owner)
      return partySize;
    partyIndex++;
  }
  gGameState.activeOwners[partyIndex] = owner;
  return partySize + 1;
}

// Takes character `owner` out of the party. Returns the party size.
s32 Party_RemoveActiveOwner(s32 owner) {
  s32 partySize = Party_CountActiveOwners();
  s32 removedIndex;
  s32 partyIndex;

  GameFlag_ClearBit(owner);
  for (removedIndex = 0; removedIndex < partySize; removedIndex++) {
    if (gGameState.activeOwners[removedIndex] == owner)
      break;
  }
  for (partyIndex = removedIndex; partyIndex < partySize - 1; partyIndex++)
    gGameState.activeOwners[partyIndex] =
        gGameState.activeOwners[partyIndex + 1];
  return Party_CountActiveOwners();
}

// Writes the party members to `owners`, ending with 0xff, and returns how
// many there are.
s32 Party_ListActiveOwners(s16 *owners) {
  s32 count = 0;

  if (owners != NULL) {
    s32 index;

    count = Party_CountActiveOwners();
    index = 0;
    if (count != 0) {
      do {
        *owners++ = gGameState.activeOwners[index];
        index++;
      } while (index != count);
    }
    *owners = 0xff;
  }
  return count;
}

// Adds `amount` to the party's coins, kept between 0 and 999999, and returns
// the new total.
s32 Party_AdjustCoins(s32 amount) {
  s32 value;
  struct GameState *work;
  struct GameState *store;

  work = &gGameState;
  value = work->coins;
  value += amount;
  store = work;
  if (value > 999999) {
    value = 999999;
  }
  if (value < 0) {
    value = 0;
  }
  work = store;
  work->coins = value;
  return value;
}

struct TradeFlags {
  u32 flags;
};

struct ElementLevelBonus {
  u16 power;
  u16 res;
};

extern struct ElementLevelBonus gElement_PowerResistByLevel[16];

// Each adds `amount` to a game state counter, kept between 0 and `max`, and
// returns the new value.
#define DEFINE_PARTY_ADJUST(name, field, max)                                  \
  s32 name(s32 amount) {                                                       \
    s32 value;                                                                 \
    struct GameState *progress = &gGameState;                                  \
                                                                               \
    value = progress->field;                                                   \
    value += amount;                                                           \
    if (value > max)                                                           \
      value = max;                                                             \
    if (value < 0)                                                             \
      value = 0;                                                               \
    progress->field = value;                                                   \
    return value;                                                              \
  }

DEFINE_PARTY_ADJUST(Party_AdjustShopCredit, shopCredit, 999999)
DEFINE_PARTY_ADJUST(Party_AdjustShopGifts, shopGifts, 28)

// Writes the summons whose bit is set in the first word of the party's Djinn
// table to `output`, in menu order, ending with 32. Returns how many.
s32 Summon_ListLearned(u8 *output) {
  u8 *source = gSummon_OrderList;
  u8 *end = gSummon_OrderList + 15;
  s32 count = 0;

  do {
    u8 value = *source++;
    if ((((struct TradeFlags *)Djinn_GetRecoveryTable(0))->flags &
         (1 << value)) != 0) {
      *output++ = value;
      count++;
    }
  } while (source <= end);
  *output = 32;
  return count;
}

// Returns summon `summonId`'s definition, or NULL past the sixteenth.
const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId) {
  if (summonId > 15)
    return NULL;
  return &gSummon_DefinitionTable[summonId];
}

// Returns the class series selected by the primary and secondary elements.
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement) {
  return gOwner_FourColumnValues[(primaryElement * 4) + secondaryElement];
}

// Writes ten times each element level to `output`: an enemy's from its
// element preset, a character's from the Djinn set on it (`source` is its
// Djinn bits) plus its own element bonus.
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]) {
  s32 i;
  s32 *cursor;

  if (record > 7) {
    u32 index;

    index = ((const struct EnemyDefinition *)Enemy_GetDefinition(record))
                ->elementPreset;
    if (index > 43)
      index = 0;

    i = 0;
    cursor = output;
    for (; i <= 3; i++)
      *cursor++ = gEnemy_ElementPresetTable[index].digits[i] * 10;
  } else {
    cursor = output;
    source += (u8 *)&((struct BattleUnit *)0)->djinnActiveCounts -
              (u8 *)&((struct BattleUnit *)0)->djinnAvailable;
    for (i = 3; i >= 0; i--) {
      u32 value = *source;
      source++;
      *cursor++ = value * 10;
    }

    if (record <= 7) {
      for (i = 0; i <= 3; i++) {
        *output += Character_GetDefinition(record)->elementBonus[i];
        output++;
      }
    }
  }

  return 0;
}

// Returns a unit's level in `element` (0 to 3).
s32 Unit_GetElementLevel(s32 owner, s32 element) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 elementLevels[4];
  s32 result = 0;

  if (element <= 3) {
    Unit_GetElementLevels(unit->classId, (u8 *)unit->djinnAvailable,
                          elementLevels);
    result = elementLevels[element] / 10;
  }
  return result;
}

// Returns the element an enemy attacks with, from its element preset.
s32 Enemy_GetAttackElement(struct BattleUnit *unit) {
  const struct EnemyDefinition *enemy =
      (const struct EnemyDefinition *)Enemy_GetDefinition(unit->classId);
  u8 presetIndex = enemy->elementPreset;

  if (presetIndex > 43)
    presetIndex = 0;
  return gEnemy_ElementPresetTable[presetIndex].first;
}

// Writes a unit's base power and resistance in each element: an enemy's come
// from its preset, a character's from its element levels.
// Both callers ignore the result. The matching return type preserves r0;
// plain void changes the two return instructions (tbs-en: differ 4).
// Its array view keeps the table-address addition after the first store;
// using the typed fields in that build differs 2 in tbs-en.
#ifdef UBFIX
void Unit_CalcBaseElements(s32 owner, struct BattleElementStats *destination) {
#else
u32 Unit_CalcBaseElements(s32 owner, s16 destination[4][2]) {
#endif
  struct BattleUnit *state = UnitGet(owner);
  u32 index;
  s32 values[4];
  s32 i;

  if (state->classIndex == 0) {
    index =
        ((const struct EnemyDefinition *)Enemy_GetDefinition(state->classId))
            ->elementPreset;
    if (index > 43)
      index = 0;

#ifdef UBFIX
    // Copy the packed pairs without accessing halfword members as words.
    memcpy(destination, gEnemy_ElementPresetTable[index].values,
           sizeof(gEnemy_ElementPresetTable[index].values));
    return;
#else
    i = 0;
    for (;;) {
      ((s32 *)destination)[i] = gEnemy_ElementPresetTable[index].values[i];
      i++;
      if (i > 3) {
        return index;
      }
    }
#endif
  }

  Unit_GetElementLevels(state->classId, (u8 *)state->djinnAvailable, values);
  for (i = 0; i < 4; i++) {
    s32 value;
    s32 ones;
    s32 tens;

    value = values[i];
    ones = value % 10;
    tens = value / 10;

    if (tens > 15)
      tens = 15;
    if (tens < 0)
      tens = 0;

#ifdef UBFIX
    destination[i].power = gElement_PowerResistByLevel[tens].power + ones;
    destination[i].resistance = gElement_PowerResistByLevel[tens].res + ones;
#else
    destination[i][0] = gElement_PowerResistByLevel[tens].power + ones;
    destination[i][1] = gElement_PowerResistByLevel[tens].res + ones;
#endif
  }
}

// Returns the class a character has with `djinn` set: the series is chosen by
// its two strongest elements, and the class is the last of the series whose
// element requirements it meets. Isaac and Garet as children and Jenna have
// fixed classes.
s32 Class_Determine(s32 character, const u8 *djinn) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 j;
  s32 i;

  result = -1;
  if (character > 7)
    return 0;
  Unit_GetElementLevels(character, djinn, levels);
  if (GameFlag_Test(FLAG_CHILDHOOD)) {
    if (character == ISAAC)
      return 200;
    if (character == GARET)
      return 201;
  }
  if (character == JENNA)
    return 202;
  if (result != -1)
    return result;
  best = result;
  primary = result;
  for (i = 0; i < 4; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  j = -1;
  best = -1;
  for (i = 0; i < 4; i++) {
    if (i != primary && best < levels[i]) {
      best = levels[i];
      j = i;
    }
  }
  series = Class_GetSeries(primary, levels[j] > 9 ? j : primary);
  for (i = 202; i >= 0; i--) {
    if (gClass_DefinitionTable[i].series != series)
      continue;
    for (j = 0; j < 4; j++) {
      if (levels[j] < gClass_DefinitionTable[i].required[j] * 10)
        break;
    }
    if (j == 4) {
      result = i;
      break;
    }
  }
  if (result == -1)
    result = 0;
  return result;
}

// Returns the definition of class `classId`.
struct ClassDefinition *Class_GetDefinition(s32 classId) {
  return &gClass_DefinitionTable[classId];
}

// Works out a character's class again after its Djinn change, then its
// Psynergy list and its base elements.
void Unit_UpdateClass(s32 ownerId) {
  struct BattleUnit *owner = UnitGet(ownerId);

  owner->classIndex =
      Class_Determine(owner->classId, (u8 *)owner->djinnAvailable);
  Unit_RefreshPsynergy(ownerId);
#ifdef UBFIX
  Unit_CalcBaseElements(ownerId, owner->baseElement);
#else
  Unit_CalcBaseElements(ownerId, (s16 (*)[2])owner->baseElement);
#endif
}

// Returns the multiplier (256 = 1x) for an element difference `input`, read
// from a five-point line; with `halve` the effect of the difference is half.
s32 Battle_GetElementMultiplier(s32 input, s32 halve) {
  s32 minimumDifference;
  s32 maximumDifference;
  s32 pointCount;
  s32 pointIndex;
  s32 multiplier;
  s32 differenceSpan;
  s32 multiplierSpan;

  maximumDifference = gElement_PowerResistCurve[0][0];
  minimumDifference = gElement_PowerResistCurve[4][0];
  pointCount = 5;
  if (input > maximumDifference)
    input = maximumDifference;
  else if (input < minimumDifference)
    input = minimumDifference;
  for (pointIndex = 0; pointIndex < pointCount; pointIndex++) {
    if (input > gElement_PowerResistCurve[pointIndex][0])
      break;
  }
  if (pointIndex == pointCount) {
    multiplier = gElement_PowerResistCurve[pointIndex - 1][1];
  } else {
    differenceSpan = gElement_PowerResistCurve[pointIndex - 1][0] -
                     gElement_PowerResistCurve[pointIndex][0];
    multiplierSpan = gElement_PowerResistCurve[pointIndex - 1][1] -
                     gElement_PowerResistCurve[pointIndex][1];
    multiplier = (input - gElement_PowerResistCurve[pointIndex][0]) *
                     multiplierSpan / differenceSpan +
                 gElement_PowerResistCurve[pointIndex][1];
  }
  switch (halve) {
  case 0:
    break;
  case 1:
    multiplier /= 2;
    break;
  }
  return multiplier + 0x100;
}

// The battle's random number generator, the same linear congruential one as
// Random16 with its own seed. Returns bits 8 to 23 of the new seed.
s32 BattleRandom(void) {
  u32 seed;

  seed = ((u32)gBattleRandomSeed * 0x41c64e6dU) + 0x3039U;
  gBattleRandomSeed = seed;
  return (seed << 8) >> 16;
}

// Returns a battle random number from 0 to 99.
u32 BattleRandomPercent(void) { return ((u32)BattleRandom() * 100) >> 16; }

// Scales max(attack - defense, 0) + twice the action's added power by the
// elemental multiplier, then halves it. ElementDifference is power minus
// resistance; its effect here is half Psynergy's (128..384 with 256 = 1x).
s32 Battle_CalcAttackDamage(s32 attack, s32 defense, s32 addedPower,
                            s32 elementDifference) {
  s32 damage;
  s32 scaledDamage;

  attack -= defense;
  damage = elementDifference;
  if (attack < 0)
    attack = 0;
  scaledDamage =
      Battle_GetElementMultiplier(damage, 1) * (attack + (addedPower * 2));
  if (scaledDamage < 0)
    scaledDamage += 0x1ff;
  damage = scaledDamage >> 9;
  if (damage < 0)
    damage = 0;
  return damage;
}

// The power of Psynergy: `power` times the multiplier for the element
// difference (power minus resistance, 0..512 with 256 = 1x) and the supplied
// powerMultiplier (also 256 = 1x).
s32 Battle_CalcPsynergyPower(s32 power, s32 elementDifference,
                             s32 powerMultiplier) {
  s32 scaledPower;

  scaledPower = Battle_GetElementMultiplier(elementDifference, 0) * power *
                powerMultiplier;
  if (scaledPower < 0)
    scaledPower = scaledPower + 0xffff;
  return scaledPower >> 16;
}

// The amount healed: `power` and `rate` (256 is 1x) times the multiplier for
// twice the healer's element power above 100.
//   element : the healer's power in the element (100 is 1x)
s32 Battle_CalcRecovery(s32 power, s32 elementPower, s32 powerMultiplier) {
  s32 signedProduct;
  u32 scaledRecovery;

  scaledRecovery =
      Battle_GetElementMultiplier(elementPower * 2 - 200, 0) * power;
  scaledRecovery = powerMultiplier * scaledRecovery;
  signedProduct = scaledRecovery;
  if (signedProduct < 0)
    signedProduct = signedProduct + 0xffff;
  return signedProduct >> 16;
}

// Returns the element the unit attacks with (4 for none): an enemy's comes
// from its kind, a character's from its equipped weapon.
s32 Item_GetEquippedElement(s32 owner) {
  struct ItemDefinition *item;
  struct BattleUnit *unit;

  unit = UnitGet(owner);
  if (unit->classIndex == 0)
    return Enemy_GetAttackElement(unit);
  item = Inventory_GetEquippedDefinition(unit, 1); // 1: weapon
  if (item != NULL)
    return item->element;
  return 4;
}

// Returns the unleash chance, in percent, that a unit's equipment adds
// (equipment effect 23).
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *unit) {
  s32 chanceBonus;
  s32 inventorySlot;
  s32 effectIndex;
  struct ItemDefinition *item;

  chanceBonus = 0;
  for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
    if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(unit->inventory[inventorySlot]);
      for (effectIndex = 0; effectIndex < 4; effectIndex++) {
        if (item->effects[effectIndex].kind == ITEM_EFFECT_ADD_UNLEASH_CHANCE)
          chanceBonus += item->effects[effectIndex].amount;
      }
    }
  }
  if (chanceBonus < 0)
    chanceBonus = 0;
  return chanceBonus;
}

// Rolls whether a character's weapon unleashes. Returns the unleash action,
// or 1 for a plain attack.
s32 RollWeaponUnleash(struct BattleUnit *owner) {
  struct ItemDefinition *item;
  s32 rate;

  if (owner->classIndex == 0) {
    return 1;
  }
  item = Inventory_GetEquippedDefinition(owner, 1);
  if (item == NULL) {
    return 1;
  }
  if (item->unleashAction == 0) {
    return 1;
  }
  rate = ((Equipment_GetUnleashRateBonus(owner) + (item->unleashChance * 5))
          << 0x10) /
         100;
  if (rate > (BattleRandom() & 0xFFFF)) {
    return item->unleashAction;
  }
  return 1;
}

// Returns the base chance, in percent, of effect `effectId`. A negative
// value is a fixed chance that elements and luck do not change.
s32 BattleFx_GetBaseSuccessRate(s32 effectId) {
  s32 fixedChance;
  u32 effectIndex;

  effectIndex = effectId - 8;
  switch (effectIndex) {
  case 4:
  case 5:
    return 70;
  case 8:
  case 9:
    return 75;
  case 14:
    return 30;
  case 15:
    return 40;
  case 16:
    return 45;
  case 10:
  case 11:
  case 17:
    return 55;
  case 18:
    return 25;
  case 19:
    return 20;
  case 12:
  case 23:
    return 65;
  case 13:
  case 26:
    return 35;
  case 27:
    return 50;
  case 48:
    fixedChance = 60;
    break;
  case 49:
    fixedChance = 90;
    break;
  case 0:
  case 1:
  case 20:
  case 24:
    return 60;
  default:
    fixedChance = 100;
    break;
  }
  return 0 - fixedChance;
}

// Returns 1 when the unit is weak to effect `effectId`: one of three listed
// for its enemy kind or class.
s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effectId) {
  const u8 *weakEffects;
  s32 weaknessIndex;

  if (target->classIndex == 0) {
    weakEffects = Enemy_GetDefinition(target->classId) + 0x48;
    for (weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
      if (*weakEffects == effectId)
        return 1;
      weakEffects++;
    }
  } else {
    weakEffects = Class_GetDefinition(target->classIndex)->unk50;
    for (weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
      if (*weakEffects++ == effectId)
        return 1;
    }
  }
  return 0;
}

// Returns 1 for the revive effects, which only work on a downed unit.
s32 Battle_IsReviveEffect(s32 effectId) {
  if ((effectId == 5) || (effectId == 0x38) || (effectId == 0x39)) {
    return 1;
  }
  return 0;
}

// Rolls whether effect `effectId` takes hold on `target`: it fails at once
// when there is nothing for it to do. The chance is the base rate, plus three
// times the caster's lead over the target in the element, less half the
// target's luck, plus 25 against a weakness, times `successScale` percent.
// Some effects get three tries.
s32 BattleFx_RollSuccess(s32 caster, s32 target, s32 resistanceCategory,
                         s32 effectId, s32 successScale) {
  /* FAKEMATCH: three status reads (psySeal after the effect 4 test, confusion
   * in it, haunt for effect 0x40) go through the byte view state[offset], so
   * each builds its address from a fresh offset constant as the native does;
   * read as unit fields, CSE reuses a neighbouring address. All eleven reads as
   * fields differ 120 (184 instructions for 204); every other read is a field
   * at no cost. */
  u8 *state = (u8 *)UnitGet(target);
  struct BattleUnit *unit = (struct BattleUnit *)state;
  s32 attempts = 1;
  s32 score;
  s32 attempt;
  s8 *poison;
  u8 *delusion;
  u8 *confusion;
  u8 *charm;
  u8 *stun;
  u8 *sleep;

  if (Battle_IsReviveEffect(effectId) != 0 && unit->hp != 0) {
    return 0;
  }

  if (effectId == 3 && unit->poison == 0) {
    return 0;
  }

  goto action4_check;
action4_tail:
  if (unit->stun == 0 && unit->sleep == 0 &&
      state[(u32)&((struct BattleUnit *)0)->psySeal] == 0 &&
      unit->grimReaper == 0) {
    return 0;
  }
  goto action4_done;
action4_check:
  if (effectId == 4) {
    if (unit->delusion == 0 &&
        state[(u32)&((struct BattleUnit *)0)->confusion] == 0 &&
        unit->charm == 0) {
      goto action4_tail;
    }
  }

action4_done:
  poison = &unit->poison;
  delusion = &unit->delusion;
  confusion = (u8 *)&unit->confusion;
  charm = &unit->charm;
  stun = &unit->stun;
  sleep = &unit->sleep;
  if (effectId == 0x40 && *poison == 0 && *delusion == 0 && *confusion == 0 &&
      *charm == 0 && *stun == 0 && *sleep == 0 && unit->psySeal == 0 &&
      unit->grimReaper == 0 &&
      state[(u32)&((struct BattleUnit *)0)->haunt] == 0) {
    return 0;
  }

  if (effectId == 0x1C && unit->grimReaper == 1) {
    return 0;
  }

  score = BattleFx_GetBaseSuccessRate(effectId);
  if (score > 0) {
    s32 difference = Unit_GetElementLevel(caster, resistanceCategory) -
                     Unit_GetElementLevel(target, resistanceCategory) -
                     (unit->luck >> 1);
    score += difference * 3;
    if (BattleTarget_IsWeakToEffect(unit, effectId) != 0) {
      score += 25;
    }
  } else {
    score = -score;
  }

  if (effectId == 0x43) {
    attempts = 3;
  }

  for (attempt = 0; attempt < attempts; attempt++) {
    if (score * successScale / 100 >= (s32)BattleRandomPercent()) {
      return 1;
    }
  }
  return 0;
}

// Returns the definition of Djinn `index` (0 to 19) of element `group` (0 to
// 3); anything out of range gives the first.
const struct DjinnDefinition *Djinn_GetDefinition(u32 group, u32 index) {
  s32 entry;

  entry = 0;
  if ((group <= 3U) && (index <= 0x13U)) {
    entry = (group * 0x14) + index;
  }
  return &gDjinn_DefinitionTable[entry];
}

// Gives a newly found Djinn to the party member holding the fewest Djinn who
// has room in that element, puts it on standby and sets its found flag.
// Returns who got it, -1 if it was found before or -2 when nobody has room.
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn) {
  s32 flag = djinn + element * 20 + FLAG_DJINNI_0;
  s32 leastLoadedOwner = 0;
  s32 fewestDjinn = 999;
  s32 partySize;
  s32 partyIndex;
  s32 elementIndex;

  if (GameFlag_Test(flag) != 0)
    return -1;

  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct BattleUnit *unit = UnitGet(gGameState.activeOwners[partyIndex]);

    if (unit->djinnOwnedCounts[element] <= 9) {
      s32 ownedDjinn = 0;

      for (elementIndex = 0; elementIndex < 4; elementIndex++)
        ownedDjinn += unit->djinnOwnedCounts[elementIndex];
      if (fewestDjinn > ownedDjinn) {
        fewestDjinn = ownedDjinn;
        leastLoadedOwner = gGameState.activeOwners[partyIndex];
      }
    }
  }

  if (fewestDjinn == 999)
    return -2;

  Djinn_AddToOwner(leastLoadedOwner, element, djinn);
  DjinnRecovery_AddStandby(leastLoadedOwner, element, djinn);
  GameFlag_SetBit(flag);
  return leastLoadedOwner;
}

// Gives Djinn `djinnIndex` of `element` to a unit. Returns -1 when the unit
// holds ten of the element or has it already.
s32 Djinn_AddToOwner(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);

  if (unit->djinnOwnedCounts[element] > 9)
    return -1;
  if ((unit->djinnAvailable[element] & (1 << djinnIndex)) != 0)
    return -1;
  unit->djinnOwnedCounts[element]++;
  unit->djinnAvailable[element] |= 1 << djinnIndex;
  return 0;
}

// Returns 1 when the unit holds the Djinn and it is neither set nor waiting
// on the standby and recovery list.
s32 Djinn_IsReady(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);
  struct DjinnRecoveryList *recoveryList;
  s32 entryIndex;
  s32 recoveryTurns;

  if (unit->djinnOwnedCounts[element] == 0)
    return 0;
  if (unit->djinnActiveCounts[element] > 9) {
    unit->djinnActiveCounts[element] = 10;
    return 0;
  }
  if ((unit->djinnAvailable[element] & (1 << djinnIndex)) == 0)
    return 0;
  if ((unit->djinnActive[element] & (1 << djinnIndex)) != 0)
    return 0;

  recoveryList = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(
                       (owner < 0 || owner > 7)))
                      ->list;
  for (entryIndex = 0; entryIndex < recoveryList->count; entryIndex++) {
    if (element == recoveryList->entries[entryIndex].element &&
        djinnIndex == recoveryList->entries[entryIndex].index)
      break;
  }
  if (entryIndex == recoveryList->count ||
      ((recoveryTurns = recoveryList->entries[entryIndex].turns) <= 0 &&
       recoveryTurns != -2))
    return 1;
  return 0;
}

// Returns 1 when the Djinn is set on the unit.
u32 Djinn_IsActive(s32 owner, s32 element, s32 djinnIndex) {
  s32 activeBit = UnitGet(owner)->djinnActive[element] & (1 << djinnIndex);

  return (u32)(-activeBit | activeBit) >> 31;
}

// Sets a ready Djinn on the unit and works out its class again. Returns 1
// when it was set.
s32 Djinn_Activate(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 isReady = Djinn_IsReady(owner, element, djinnIndex);

  if (isReady != 0) {
    if (unit->djinnAvailable[element] & (1 << djinnIndex)) {
      unit->djinnActive[element] |= 1 << djinnIndex;
    } else {
      return 0;
    }
    unit->djinnActiveCounts[element]++;
    Unit_UpdateClass(owner);
  }
  return isReady;
}

// Takes a set Djinn off the unit and works out its class again. Returns 1
// when it was set.
u32 Djinn_Deactivate(s32 owner, s32 element, s32 djinnIndex) {
  struct BattleUnit *unit = UnitGet(owner);
  u32 wasActive = Djinn_IsActive(owner, element, djinnIndex);

  if (wasActive != 0) {
    unit->djinnActiveCounts[element]--;
    unit->djinnActive[element] &= ~(1 << djinnIndex);
    Unit_UpdateClass(owner);
  }
  return wasActive;
}

// Takes the Djinn off its side's standby and recovery list. Returns 1 when
// it was there.
s32 DjinnRecovery_Remove(s32 owner, s32 element, s32 djinnIndex) {
  struct DjinnRecoveryList *recoveryList;
  s32 removed = 0;
  s32 entryIndex;

  recoveryList = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(
                       (owner < 0 || owner > 7)))
                      ->list;
  for (entryIndex = 0; entryIndex < recoveryList->count; entryIndex++) {
    if (element == recoveryList->entries[entryIndex].element &&
        djinnIndex == recoveryList->entries[entryIndex].index) {
      recoveryList->count--;
      removed = 1;
      break;
    }
  }
  for (; entryIndex < recoveryList->count; entryIndex++) {
    recoveryList->entries[entryIndex] = recoveryList->entries[entryIndex + 1];
  }
  return removed;
}

// Puts the owner's Djinn at the end of
// its side's list on standby (turns -1). Returns the list.
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinnIndex) {
  struct DjinnRecoveryList *recoveryList;
  s32 entryIndex;

  DjinnRecovery_Remove(owner, element, djinnIndex);
  recoveryList =
      &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTable(owner > 7))->list;
  entryIndex = recoveryList->count;
  recoveryList->entries[entryIndex].element = element;
  recoveryList->entries[entryIndex].index = djinnIndex;
  recoveryList->entries[entryIndex].unitId = owner;
  recoveryList->entries[entryIndex].turns = -1;
  recoveryList->count = entryIndex + 1;
  return (u32 *)&recoveryList->count;
}

// Moves a Djinn from one unit to another, keeping it set or on standby.
// Returns -1 when the source lacks it or the target has no room.
s32 Djinn_Transfer(s32 source, s32 element, s32 djinnIndex, s32 target) {
  struct BattleUnit *sourceUnit = UnitGet(source);
  u32 wasActive;

  if ((sourceUnit->djinnAvailable[element] & (1 << djinnIndex)) != 0) {
    wasActive = Djinn_IsActive(source, element, djinnIndex);
    if (Djinn_AddToOwner(target, element, djinnIndex) == 0) {
      Djinn_Deactivate(source, element, djinnIndex);
      sourceUnit->djinnAvailable[element] &= ~(1 << djinnIndex);
      sourceUnit->djinnOwnedCounts[element]--;

      if (wasActive != 0) {
        Djinn_Activate(target, element, djinnIndex);
      } else {
        DjinnRecovery_Remove(source, element, djinnIndex);
        DjinnRecovery_AddStandby(target, element, djinnIndex);
      }
      return 0;
    }
  }
  return -1;
}

// Counts the party's Djinn on standby, by element into `counts` when it is
// given. Returns the total.
s32 DjinnRecovery_CountStandby(u8 *counts) {
  s32 standbyCount = 0;
  struct DjinnRecoveryTable *recoveryTable = Djinn_GetRecoveryTable(0);
  struct DjinnRecoveryEntry *entry = recoveryTable->list.entries;
  s32 entryIndex;

  if (counts != 0) {
    u8 *slot;

    slot = counts + 3;
    *slot = standbyCount;
    slot = counts + 2;
    *slot = standbyCount;
    slot = counts + 1;
    *slot = standbyCount;
    counts[0] = standbyCount;
  }
  entryIndex = 0;
  if (recoveryTable->list.count != 0) {
    do {
      if (entry->turns == -1) {
        if (counts != 0)
          counts[entry->element]++;
        standbyCount++;
      }
      entryIndex++;
      entry++;
    } while (entryIndex != recoveryTable->list.count);
  }
  return standbyCount;
}

// Returns the ability ID used when the Djinni is unleashed.
s32 Djinn_GetAbilityId(u32 group, u32 index) {
  return Djinn_GetDefinition(group, index)->abilityId;
}

// Returns how many Djinn of `element` the party holds, or of all
// elements for -1.
s32 Party_SumDjinnCounts(s32 element) {
  u16 owners[16];
  s32 djinnCount = 0;
  s32 partySize = Party_ListActiveOwners(owners);

  if (djinnCount < partySize) {
    u16 *ownerCursor = owners;
    s32 remainingOwners = partySize;

    do {
      struct BattleUnit *unit = UnitGet(*ownerCursor++);

      if (element == -1) {
        djinnCount += unit->djinnOwnedCounts[0];
        djinnCount += unit->djinnOwnedCounts[1];
        djinnCount += unit->djinnOwnedCounts[2];
        djinnCount += unit->djinnOwnedCounts[3];
      } else {
        djinnCount += unit->djinnOwnedCounts[element];
      }
      remainingOwners--;
    } while (remainingOwners != 0);
  }
  return djinnCount;
}

// Gives a unit `itemId` and equips every exactly matching inventory entry.
void Inventory_AddAndEquip(s32 owner, s32 itemId) {
  struct BattleUnit *unit = UnitGet(owner);
  u16 *entry;
  s32 slot;

  Inventory_AddItem(owner, itemId);
  slot = 0;
  entry = unit->inventory;
  do {
    if (*entry++ == itemId)
      Inventory_Equip(owner, slot);
    slot++;
  } while (slot < INVENTORY_SLOTS);
}

// The party's equipment, put aside while the Colosso takes it away; the
// sentinel marks the snapshot as taken.
#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

// Saves the first four characters' inventories, shortcuts and party order,
// then keeps only the Psynergy-granting items (type 6), gives Isaac item 16,
// sets flag 0x952 and restores everyone. Nothing happens when a snapshot is
// already saved, apart from the restore.
void InventorySnapshot_Save(void) {
  u16 *save;
  struct BattleUnit *unit;
  s16 *inventory;
  s32 owner;
  s32 slot;
  s16 firstShortcut;
  s16 secondShortcut;

  save = gInventorySnapshot;
  if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
    *save++ = INVENTORY_SNAPSHOT_SENTINEL;
    firstShortcut = gGameState.firstShortcut;
    secondShortcut = gGameState.secondShortcut;
    // Save each inventory before retaining its Psynergy-granting items.
    for (owner = 0; owner < 4; owner++) {
      unit = UnitGet(owner);
      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *save++ = unit->inventory[slot];
      for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if (Item_GetDirect(unit->inventory[slot])->type != 6)
          unit->inventory[slot] = 0;
      }
      inventory = unit->inventory;
      Inventory_CompactSlots(inventory);
      Unit_UpdateClass(owner);
      UnitCalc(owner);
    }
    // Keep shortcuts and party order with the inventory snapshot.
    *save++ = firstShortcut;
    *save++ = secondShortcut;
#ifdef UBFIX
    // Party order is a byte array; pack its two saved halfwords explicitly.
    save[0] = gGameState.activeOwners[0] | (gGameState.activeOwners[1] << 8);
    save[1] = gGameState.activeOwners[2] | (gGameState.activeOwners[3] << 8);
#else
    inventory = (s16 *)gGameState.activeOwners;
    save[0] = inventory[0];
    save[1] = inventory[1];
#endif
    Inventory_AddAndEquip(0, 16);
    GameFlag_SetBit(0x952);
  }
  Party_RestoreHpPp(1);
}

// Gives back the inventories, shortcuts and party order InventorySnapshot_Save
// put aside.
void InventorySnapshot_Restore(void) {
  s32 owner;
  u16 *source = gInventorySnapshot;

  if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
    owner = 0;
    do {
      u16 *inventory = UnitGet(owner)->inventory;
      s32 remaining = 14;

      do {
        *inventory++ = *source++;
        remaining--;
      } while (remaining >= 0);
      Unit_UpdateClass(owner);
      UnitCalc(owner);
      owner++;
    } while (owner <= 3);

    gGameState.firstShortcut = *source++;
    gGameState.secondShortcut = *source++;
#ifdef UBFIX
    gGameState.activeOwners[0] = source[0];
    gGameState.activeOwners[1] = source[0] >> 8;
    gGameState.activeOwners[2] = source[1];
    gGameState.activeOwners[3] = source[1] >> 8;
    source++;
#else
    *(u16 *)gGameState.activeOwners = *source++;
    *(u16 *)((u8 *)gGameState.activeOwners + 2) = *source;
#endif
    source = gInventorySnapshot;
    *source = 0;
    GameFlag_ClearBit(0x952);
  }
}
