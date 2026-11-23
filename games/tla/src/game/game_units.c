/// @file
/// The party's units and their stats, a new game, HP and PP, and the data
/// transfer from The Broken Seal up to Isaac's party joining at Contigo.
// Part of game.c.

#include "game_internal.h"
#include "flags.h"
#include "macros.h"
#include "field/flag_call.h"
#include "gamepak/far_call/field_sequence.h"
#include "game/game.h"
#include "gamepak/far_call/window.h"
#include "system/ram_buffer.h"
#include "resource_ids.h"
#include "system/save.h"

extern s16 gSaveSlot;

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

/// Returns the definition of enemy `enemyId`; enemy ids start past the eight
/// characters', so that one id names any unit.
const struct EnemyDefinition *Enemy_GetDefinition(s32 enemyId) {
  u32 recordIndex;

  recordIndex = enemyId - ENEMY_ID_FIRST;
  // BUG: the table has ENEMY_COUNT rows, but only rows from ENEMY_ID_MAX on are
  // refused, so those from ENEMY_COUNT read the tables after it. BattleUnit_Set
  // takes the first row for them itself.
#ifdef BUGFIX
  if (recordIndex > ENEMY_COUNT - 1U)
#else
  if (recordIndex > ENEMY_ID_MAX - 1U)
#endif
    recordIndex = 0;
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

/// The distance between `a` and `b`, |a - b|.
#define UNIT_STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

/// A scaling effect's number among the eight, from EQUIP_EFFECT_SCALE_HP.
#define SCALE_EFFECT(kind) ((kind) - EQUIP_EFFECT_SCALE_HP)

/// Works out a unit's stats from its base stats, equipment, set Djinn, class
/// and battle stat levels; its HP and PP keep their share of a new maximum.
void Unit_Calculate(s32 unitId) {
  struct StatWork *stats;
  struct Unit *unit;
  s32 index, effectIndex, elementIndex;
  s32 value;
  s32 lighthouse;
  s32 effectKind;
  s32 effectAmount;
  s32 hpLimit;
  s32 ppLimit;
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
  stats->actionCount = unit->baseActionCount & UNIT_ACTION_COUNT_MASK;
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

  // CURSE_KEEP_BOUND keeps CURSE_BOUND, and CURSE_REMOVABLE clears both again
  // below.
  unit->curse &= ~(CURSE_BOUND | CURSE_ITEM);
  if (unit->curse & CURSE_KEEP_BOUND)
    unit->curse |= CURSE_BOUND;
  if (unit->ready)
    stats->actionCount++;
  unit->equipBonus[0] = 0;

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
      stats->def += stats->item->secondaryBonus;
      for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++) {
        effectKind = stats->item->effects[effectIndex].kind;
        effectAmount = stats->item->effects[effectIndex].amount;
        stats->kind = effectKind;
        stats->amount = effectAmount;
        switch (effectKind) {
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
      const struct ClassDefinition *classDefinition =
          Class_GetDefinition(unit->classId);

      stats->hp = stats->hp * classDefinition->hp / STAT_SCALE_ONE;
      stats->pp = stats->pp * classDefinition->pp / STAT_SCALE_ONE;
      stats->atk = stats->atk * classDefinition->atk / STAT_SCALE_ONE;
      stats->def = stats->def * classDefinition->def / STAT_SCALE_ONE;
      stats->agi = stats->agi * classDefinition->agi / STAT_SCALE_ONE;
      stats->luck = stats->luck * classDefinition->luck / STAT_SCALE_ONE;
    }

    // Then the equipment's multiplying effects, EQUIP_EFFECT_SCALE_HP to
    // EQUIP_EFFECT_SCALE_LUCK, also in tenths, applied after the class.
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
    stats->element[index].resistance +=
        unit->resistanceStage * RESISTANCE_PER_STAGE;

  if (unit->classId != CLASS_NONE) {
    lighthouse = 0;
    // Faster PP recovery inside the lighthouse of the character's own element.
    switch (unit->enemyId) {
    case GARET:
      lighthouse = GameFlag_Get(FLAG_IN_MARS_LIGHTHOUSE);
      break;
    case MIA:
      lighthouse = GameFlag_Get(FLAG_IN_MERCURY_LIGHTHOUSE);
      break;
    case JENNA:
      lighthouse = GameFlag_Get(FLAG_IN_MARS_LIGHTHOUSE);
      break;
    case ISAAC:
    case FELIX:
      lighthouse = GameFlag_Get(FLAG_IN_VENUS_LIGHTHOUSE);
      break;
    case IVAN:
    case SHEBA:
      lighthouse = GameFlag_Get(FLAG_IN_JUPITER_LIGHTHOUSE);
      break;
    case PIERS:
      lighthouse = GameFlag_Get(FLAG_IN_MERCURY_LIGHTHOUSE);
      break;
    }
    if (lighthouse)
      stats->ppRegen += 4;
  }

  CLAMP(stats->atk, 0, UNIT_STAT_MAX);
  CLAMP(stats->def, 0, UNIT_STAT_MAX);
  CLAMP(stats->agi, 0, UNIT_STAT_MAX);
  CLAMP(stats->luck, 0, UNIT_LUCK_MAX);
  // An enemy may act up to four times a turn, a character twice.
  if (unit->classId == CLASS_NONE) {
    CLAMP(stats->actionCount, 0, 4);
  } else {
    CLAMP(stats->actionCount, 0, 2);
  }
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

  if (unit->classId == CLASS_NONE) {
    hpLimit = UNIT_ENEMY_HP_MAX;
    ppLimit = UNIT_ENEMY_PP_MAX;
  } else {
    hpLimit = ppLimit = UNIT_CHARACTER_HP_MAX;
  }

  previousMaximum = unit->hpMax;
  CLAMP(stats->hp, 0, hpLimit);
  unit->hpMax = stats->hp;
  if (previousMaximum != unit->hpMax) {
    value = stats->hp * unit->hpGauge / UNIT_GAUGE_FULL;
    CLAMP(value, 0, hpLimit);
    if (unit->hp != 0 && value == 0)
      value = 1;
    unit->hp = value;
  }

  previousMaximum = unit->ppMax;
  CLAMP(stats->pp, 0, ppLimit);
  unit->ppMax = stats->pp;
  if (previousMaximum != unit->ppMax) {
    value = stats->pp * unit->ppGauge / UNIT_GAUGE_FULL;
    CLAMP(value, 0, ppLimit);
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
      const struct ItemDefinition *item;
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

/// Returns the unit's PP over its maximum PP, in 16.16; 0 with no maximum.
static __inline__ FIXED Owner_GetPpRatio(s32 owner) {
  struct Unit *unit = Unit_Get(owner);

  if (unit->ppMax <= 0)
    return 0;
  return toFIXED(unit->pp) / unit->ppMax;
}

/// Returns the party's PP over its maximum PP, in 16.16; 0 with no members.
FIXED Party_GetPpRatio(void) {
  s32 pp = 0;
  s32 max = 0;
  s32 count = Party_CountMembers();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    struct Unit *unit = Unit_Get(gMainRoutineWork.party[i]);

    pp += unit->pp;
    max += unit->ppMax;
  }
  if (max == 0)
    return 0;
  return toFIXED(pp) / max;
}

/// Takes 1 PP from the member whose PP is fullest for its maximum.
s32 Party_DrainPpFromFullest(void) {
  FIXED best = 0;
  s32 bestOwner = -1;
  s32 count = Party_CountMembers();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    FIXED ratio = Owner_GetPpRatio(gMainRoutineWork.party[i]);

    if (ratio > best) {
      best = ratio;
      bestOwner = gMainRoutineWork.party[i];
    }
  }
  if (bestOwner == -1)
    return 0;
  // Its result taken as a whole word, where the definition returns s16.
  return ((s32 (*)(s32, s32))Unit_AddPp)(bestOwner, -1);
}

/// Takes `count` PP from the party, one at a time from the fullest member.
void Party_DrainPp(s32 count) {
  s32 i;

  for (i = 0; i < count; i++)
    Party_DrainPpFromFullest();
  // The ratio is worked out again and not used.
  Party_GetPpRatio();
}

/// Set in every build stamp's time.
#define BUILD_STAMP_MARK 0x1000
/// Set on top of the time in a debug build's stamp (gDebugMode): bit 15,
/// sign-extended, so that its stamp is above every release build's.
#define BUILD_STAMP_DEBUG 0xffff8000
/// A build stamp's time counts 64 seconds to a minute and 16 minutes to an
/// hour, so the minutes past 15 reach into the hours.
#define BUILD_STAMP_SECONDS_PER_MINUTE 64
#define BUILD_STAMP_MINUTES_PER_HOUR 16

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
  time = (hours * BUILD_STAMP_MINUTES_PER_HOUR + minutes) *
             BUILD_STAMP_SECONDS_PER_MINUTE +
         seconds;
  time |= BUILD_STAMP_MARK;
  stamp = time;

  if (gDebugMode != 0)
    stamp |= BUILD_STAMP_DEBUG;
  return (u16)stamp;
}

/// Waits until the DMA channel's transfer is done.
static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA_BUSY) != 0) {
  }
}

/// The words a new game clears from gSceneLocalState up to gSoundFlags: the
/// scene's state, the actor snapshot, the rare item stock, the scene state and
/// the inventory snapshot.
#define SCENE_STATE_WORDS 1992
/// The coins a new game starts with.
#define NEW_GAME_COINS 200
/// Where gSceneLocalState holds the actor snapshot's slot ids,
/// ACTOR_SNAPSHOT_CAPACITY bytes before gSnapshotObjects.
#define SCENE_STATE_SNAPSHOT_SLOTS 4
/// The two tables a new game fills just after gRareItemStock's
/// ITEM_COUNTER_COUNT counters, as places in gSceneLocalState: 32 rows of eight
/// random numbers 0 to 4, SCENE_STATE_RANDOM_ROW_BYTES apart, then a copy of
/// gNewGame_SceneStateTable.
#define SCENE_STATE_RANDOM_ROWS 4536
#define SCENE_STATE_RANDOM_ROW_COUNT 32
#define SCENE_STATE_RANDOM_ROW_BYTES 12
#define SCENE_STATE_TABLE_COPY                                                 \
  (SCENE_STATE_RANDOM_ROWS +                                                   \
   SCENE_STATE_RANDOM_ROW_COUNT * SCENE_STATE_RANDOM_ROW_BYTES)

/// Starts a new game's state: clears gMainRoutineWork, the scene state and
/// the snapshots, the flags and the characters by DMA, builds the characters,
/// sets the defaults (the button layout, Felix and Jenna in the party, their
/// first Psynergy, NEW_GAME_COINS, the message settings and the play time
/// clock) and fills the two tables after the rare item stock.
void MainRoutineWork_InitializeDefaults(void) {
  volatile s32 zero;
  s32 i;
  s32 j;

  // Clear the state up to the characters, the scene state with its snapshots,
  // and the flags.
  Dma_Fill32Cell(zero, 0, &gMainRoutineWork, sizeof(gMainRoutineWork));
  Dma_Fill32Cell(zero, 0, gSceneLocalState, SCENE_STATE_WORDS * 4);
#if TLA_PATCH_1
  WaitDma(DMA3);
#else
  {
    struct DmaRegisters volatile *dma = DMA3;
    struct DmaRegisters volatile *channel;

    /* FAKEMATCH(barrier): an empty asm copying dma to channel and claiming to
     * change dma. */
    asm volatile("" : "=r"(channel), "+r"(dma) : "0"(dma));
    WaitDma(channel);
  }
#endif
  Dma_Fill32Cell(zero, 0, gGameFlagBytes, 128 * 4);
  // A new game holds no saved actors.
  gSceneLocalState[SCENE_STATE_SNAPSHOT_SLOTS] = ACTOR_SNAPSHOT_EMPTY;
  Dma_Fill32Cell(zero, 0, gCharacterUnits, (PIERS + 1) * sizeof(struct Unit));
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
  gMainRoutineWork.leaderActorId = FELIX;
  Party_AddMember(FELIX);
  Party_AddMember(JENNA);
  Unit_LearnPsynergy(FELIX, ABILITY_RETREAT);
  Unit_LearnPsynergy(FELIX, ABILITY_MOVE);
  Unit_LearnPsynergy(SHEBA, ABILITY_MIND_READ);
  Unit_LearnPsynergy(PIERS, ABILITY_DOUSE);
  Unit_LearnPsynergy(ISAAC, ABILITY_RETREAT);
  Unit_LearnPsynergy(ISAAC, ABILITY_MOVE);
  Unit_LearnPsynergy(GARET, ABILITY_MOVE);
  Unit_LearnPsynergy(IVAN, ABILITY_MIND_READ);
  Unit_LearnPsynergy(IVAN, ABILITY_REVEAL);
  gMainRoutineWork.coins = NEW_GAME_COINS;
  gMainRoutineWork.transferredCoins = 0;
  gMainRoutineWork.forgeItem = 0;
  gMainRoutineWork.weirdAnswers = 0;
  gMainRoutineWork.messageSpeed = 1;
  gMainRoutineWork.speech = 1;
  gMainRoutineWork.unusedAfterSpeech = 1;
  gMainRoutineWork.windowHue = 0;
  gMainRoutineWork.windowBrightness = 8;
  gMainRoutineWork.buildStamp = 0;
  gMainRoutineWork.startStamp = BuildStamp_GetTime();
  gPlayTimeFrames = 0;
  gIdleSleepEnabled = 0;
  gMainRoutineWork.playTime = 0;
  gMainRoutineWork.idleSleepEnabled = gIdleSleepEnabled;
  gIdleFrames = 0;
  gSaveSlot = -1;
  gMainRoutineWork.clearedFileAtStart = gAnyFileCleared;
  // Lemuria's spring's medal prizes (gLemuriaSpring_MedalPrizes): three share
  // each weight, and each three weigh twice the last.
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

  // XXX (no evidence): what reads the two tables; no object of the main
  // image or of any overlay holds their addresses, reads gSceneLocalState or
  // gRareItemStock that far in, or has their places in its literal pools, and
  // nothing in reach watches the memory of a played game.
  for (i = 0; i < SCENE_STATE_RANDOM_ROW_COUNT; i++) {
    for (j = 0; j < 8; j++)
      gSceneLocalState[SCENE_STATE_RANDOM_ROWS +
                       i * SCENE_STATE_RANDOM_ROW_BYTES + j] = Random_Below(5);
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gSceneLocalState[SCENE_STATE_TABLE_COPY + i * 5 + j] =
          gNewGame_SceneStateTable[i][j];
  }
}

/// Makes Jenna the leader.
void Party_SetLeaderJenna(void) { gMainRoutineWork.leaderActorId = JENNA; }

/// Makes Felix the leader, with Jenna and Sheba out of the party.
void Party_SetLeaderFelix(void) {
  gMainRoutineWork.leaderActorId = FELIX;
  Party_AddMember(FELIX);
  Party_RemoveMember(JENNA);
  Party_RemoveMember(SHEBA);
}

/// Gives Piers his two Mercury Djinn, Spring and Shade (Mercury 9 and 10),
/// set, then evens out the party's Djinn and recalculates its members.
/// Kibombo calls it when Piers joins, and the world map
/// (WORLD_MAP_PIERS_DJINN).
void Party_GivePiersDjinn(void) {
  s32 i;
  s32 count;

  // i counts Spring and Shade from 1.
  for (i = DJINNI_INDEX(DJINNI_SPRING) + 1; i <= DJINNI_INDEX(DJINNI_SHADE) + 1;
       i++) {
    GameFlag_Set(FLAG_DJINNI(ELEMENT_MERCURY * DJINN_PER_ELEMENT + i - 1));
    Djinn_Give(PIERS, ELEMENT_MERCURY, i - 1);
    Djinn_Set(PIERS, ELEMENT_MERCURY, i - 1);
  }
  // Called with an argument it does not take.
  ((void (*)(s32))Party_BalanceDjinn)(0);
  count = Party_CountMembers();
  for (i = 0; i < count; i++)
    Unit_Calculate(gMainRoutineWork.party[i]);
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

/// Restores the PP of each party member whose element group's lighthouse
/// the party is inside: Venus or Mars for group 0, Mercury or Jupiter for the
/// others. Nothing calls it or its far-call export.
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
      if (GameFlag_Get(FLAG_IN_VENUS_LIGHTHOUSE) ||
          GameFlag_Get(FLAG_IN_MARS_LIGHTHOUSE)) {
        restorePp = 1;
      }
    } else {
      if (GameFlag_Get(FLAG_IN_MERCURY_LIGHTHOUSE) ||
          GameFlag_Get(FLAG_IN_JUPITER_LIGHTHOUSE)) {
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

/// Writes the element and index of unit's last Djinni and returns 0, or
/// returns -1 when it has none.
s32 Unit_FindLastDjinn(s32 unit, s32 *elementOut, s32 *indexOut) {
  u32 *sets = Unit_Get(unit)->djinnHeld;
  s32 element = -1;
  s32 index = -1;
  s32 i;
  s32 j;

  for (i = 0; i < DJINN_PER_ELEMENT; i++) {
    for (j = 0; j < ELEMENT_COUNT; j++) {
      if (sets[j] & (1 << i)) {
        element = j;
        index = i;
      }
    }
  }
  if (element == -1)
    return element;
  *elementOut = element;
  *indexOut = index;
  return 0;
}

/// More Djinn than a party member can hold: where the search for the fewest
/// starts.
#define DJINN_COUNT_ABOVE_ANY 999

/// Until the Djinn counts of the active owners are within one, moves the last
/// Djinni of the owner holding most to the one holding least.
void Party_BalanceDjinn(void) {
  s32 count = Party_CountMembers();

  for (;;) {
    s32 lowOwner = 0;
    s32 high = 0;
    s32 low = DJINN_COUNT_ABOVE_ANY;
    s32 highOwner = 0;
    s32 i;
    s32 sum;
    s32 element;
    s32 index;

    for (i = 0; i < count; i++) {
      s32 owner = gMainRoutineWork.party[i];
      struct Unit *unit = Unit_Get(owner);
      u8 *counts;
      s32 j;

      sum = 0;
      counts = unit->djinnHeldCounts;
      for (j = 0; j < ELEMENT_COUNT; j++) {
        u8 value = *counts;
        counts++;
        sum += value;
      }
      if (sum > high) {
        high = sum;
        highOwner = owner;
      }
      if (sum < low) {
        low = sum;
        lowOwner = owner;
      }
    }
    if (high - low <= 1)
      break;
    sum = Unit_FindLastDjinn(highOwner, &element, &index);
    if (sum != 0)
      break;
    Djinn_Transfer(highOwner, element, index, lowOwner);
  }
}

/// Returns flag `flag` of flag bytes laid out as gGameFlagBytes: another save's
/// flags, as DataTransfer_Apply reads them.
s32 FlagBits_Test(u8 *bytes, s32 flag) {
  s32 bit;

  bit = flag & 7;
  flag = GAME_FLAG_BYTE(flag);
  return (bytes[flag] >> bit) & 1;
}

/// The data transfer from Golden Sun ("Transfer data from Golden Sun",
/// MsgTransferDataFromGoldenSun): copies a The Broken Seal save's Djinn, six of
/// its flags, its coins, its options (when it came by link rather than by
/// password) and its four units into this game's state.
void DataTransfer_Apply(u8 *save, s32 byPassword) {
  u8 *flags = save + SAVE_IMAGE_GAME_FLAGS;
  struct MainRoutineWork *party =
      (struct MainRoutineWork *)(save + SAVE_IMAGE_MAIN_ROUTINE_WORK);
  struct Unit *units = (struct Unit *)(save + SAVE_IMAGE_UNITS);
  struct MainRoutineWork *state;
  u8 name[16];
  s32 i;
  s32 k;
  s32 index;
  s32 count;
  s32 j;

  // Each element's first seven Djinn, The Broken Seal's, become the transferred
  // Djinn flags.
  for (i = 0, k = 0; i < ELEMENT_COUNT;
       i++, k += TRANSFERRED_DJINN_PER_ELEMENT) {

    for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++) {
      s32 flag = FLAG_TRANSFERRED_DJINN_FIRST + k + j;

      index = i * DJINN_PER_ELEMENT + j;
      if (FlagBits_Test(flags, FLAG_DJINNI(index)))
        GameFlag_Set(flag);
      else
        GameFlag_Clear(flag);
    }
  }
  for (i = 0; i < TRANSFERRED_FLAG_COUNT; i++) {
    if (FlagBits_Test(flags, gDataTransfer_Flags[i]))
      GameFlag_Set(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + i);
    else
      GameFlag_Clear(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + i);
  }
  if (GameFlag_Get(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH))
    GameFlag_Clear(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH);
  else
    GameFlag_Set(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH);

  state = &gMainRoutineWork;
  state->transferredCoins = party->coins;
  if (byPassword == 0) {
    state->windowHue = party->windowHue;
    state->windowBrightness = party->windowBrightness;
    state->messageSpeed = party->messageSpeed;
    state->speech = party->speech;
    state->idleSleepEnabled = party->idleSleepEnabled;
    WindowColor_ApplyFar(state->windowHue, state->windowBrightness);
  }

  // Isaac's party, the units moved (j reused).
  j = MIA + 1;
  for (i = 0; i < (s32)j; i++) {
    struct Unit *unit = Unit_Get(i);

    // The saved name is put aside and written back over the copy, so the
    // unit takes The Broken Seal's name either way.
    memcpy(name, &units[i], sizeof(units->name));
    Iwram_CopyWordsEntry(unit, &units[i], sizeof(struct Unit));
    memcpy(unit, name, sizeof(units->name));
    if (byPassword == 0)
      unit->enemyId = unit->id;
    for (index = INVENTORY_SLOTS - 1; index >= 0; index--) {
      s32 item = unit->inventory[index];

      if (item != 0 && (Item_GetDirect(item)->flags & ITEM_NOT_TRANSFERRED)) {
        while (Inventory_Remove(i, index) == 1)
          ;
      }
    }
    Unit_Calculate(i);
  }
  Djinn_ActivateOfferedForParty();
  Party_RestoreHpPp(1);
  GameFlag_Set((byPassword & 1) + FLAG_TRANSFERRED_BY_LINK);
  GameFlag_Set(FLAG_DATA_TRANSFERRED);
  gPlayTimeFrames = gMainRoutineWork.playTime;
}

/// Carries Clear Data over to a new quest: the options, coins and each unit's
/// base stats, level and experience.
void ClearData_Apply(u8 *save) {
  struct MainRoutineWork *party =
      (struct MainRoutineWork *)(save + SAVE_IMAGE_MAIN_ROUTINE_WORK);
  struct Unit *units = (struct Unit *)(save + SAVE_IMAGE_UNITS);
  u8 unused[16]; // Unused: gives the stack frame its 16 bytes.
  s32 i;
  s32 count;

  gMainRoutineWork.windowHue = party->windowHue;
  gMainRoutineWork.windowBrightness = party->windowBrightness;
  gMainRoutineWork.messageSpeed = party->messageSpeed;
  gMainRoutineWork.speech = party->speech;
  gMainRoutineWork.idleSleepEnabled = party->idleSleepEnabled;
  WindowColor_ApplyFar(gMainRoutineWork.windowHue,
                       gMainRoutineWork.windowBrightness);
  count = PIERS + 1;
  for (i = 0; i < count; i++) {
    struct Unit *unit = Unit_Get(i);
    struct Unit *saved = &units[i];

    unit->baseHp = saved->baseHp;
    unit->basePp = saved->basePp;
    unit->hpGauge = unit->baseHp;
    unit->ppGauge = unit->basePp;
    unit->baseAttack = saved->baseAttack;
    unit->baseDefense = saved->baseDefense;
    unit->baseAgility = saved->baseAgility;
    unit->baseLuck = saved->baseLuck;
    unit->level = saved->level;
    unit->experience = saved->experience;
    Unit_Calculate(i);
  }
  gMainRoutineWork.coins = party->coins;
}

/// Gives owner item unless a party member already has it.
void Party_GiveItemIfMissing(s32 item, s32 owner) {
  if (PartyInventory_FindMember(item) == -1)
    Inventory_AddItem(owner, item);
}

/// Isaac's party joins: the transferred Djinn flags move to their places,
/// Isaac, Garet, Ivan and Mia join with their Djinn evened out and restored,
/// a missing Djinni of each element is picked, and the transferred items and
/// coins are given. Contigo calls it.
void Party_AddIsaacsParty(void) {
  s32 i;
  s32 djinni;
  s8 missing[8];
  s32 count;
  s32 k;

  for (i = 0, k = 0; i < ELEMENT_COUNT;
       i++, k += TRANSFERRED_DJINN_PER_ELEMENT) {
    s32 j;

    for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++) {
      djinni = i * DJINN_PER_ELEMENT + j;
      if (GameFlag_Get(FLAG_TRANSFERRED_DJINN_FIRST + k + j))
        GameFlag_Set(FLAG_DJINNI(djinni));
    }
  }
  PartyInventory_Remove(ITEM_MYTHRIL_BAG_MARS_STAR);
  Party_AddMember(ISAAC);
  Party_AddMember(GARET);
  Party_AddMember(IVAN);
  Party_AddMember(MIA);
  // Called with an argument it does not take.
  ((void (*)(s32))Party_BalanceDjinn)(0);
  Djinn_ActivateOfferedForParty();
  for (i = 0; i <= PIERS; i++)
    Unit_Calculate(i);
  Party_RestoreHpPp(1);
  GameFlag_Set(FLAG_REUNION);

  // One of The Broken Seal's Djinn of each element the party lacks, picked at
  // random, plus one; 0 when it has them all.
  for (i = 0; i < ELEMENT_COUNT; i++) {
    count = 0;
    for (djinni = 0; djinni < TRANSFERRED_DJINN_PER_ELEMENT; djinni++) {
      if (!GameFlag_Get(FLAG_DJINNI(i * DJINN_PER_ELEMENT + djinni)))
        missing[count++] = djinni;
    }
    if (count != 0)
      djinni = missing[(Random16() * count) >> 16] + 1;
    else
      djinni = 0;
    gSceneLocalState[SCENE_STATE_REUNION_DJINN + i] = djinni;
  }
  // The Psynergy items Isaac's party carried in The Broken Seal.
  Party_GiveItemIfMissing(ITEM_FROST_JEWEL, MIA);
  Party_GiveItemIfMissing(ITEM_DOUSE_DROP, MIA);
  Party_GiveItemIfMissing(ITEM_LIFTING_GEM, GARET);
  Party_GiveItemIfMissing(ITEM_CARRY_STONE, ISAAC);
  Party_GiveItemIfMissing(ITEM_CATCH_BEADS, IVAN);
  gMainRoutineWork.coins += gMainRoutineWork.transferredCoins;
#if TLA_PATCH_4
  if ((u32)gMainRoutineWork.coins > COIN_MAX)
    gMainRoutineWork.coins = COIN_MAX;
#endif
}

/// Does nothing; nothing calls Unit_ReservedNoOpFar.
void Unit_ReservedNoOp(void) {}

/// Gives `owner` a Djinni on standby and sets its found flag, as if met in the
/// field.
void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn) {
  s32 flag = element * DJINN_PER_ELEMENT + djinn;

  Djinn_Give(owner, element, djinn);
  flag += FLAG_DJINN_FIRST;
  DjinnRecovery_AddStandby(owner, element, djinn);
  GameFlag_Set(flag);
}

/// The Djinn of The Broken Seal handed out at random, besides Flint and Fizz,
/// when the player transfers none.
#define DEFAULT_TRANSFER_DJINN 16

/// Stands in for transferred data when the player transfers none: unless the
/// transferred Djinn are already set, sets the party aside, makes Isaac's
/// party the party, gives it Flint and Fizz, then hands out
/// DEFAULT_TRANSFER_DJINN random Djinn of The Broken Seal's still unowned,
/// keeping the elements' counts within one, each with its chance in
/// gDataTransfer_DjinnChances; then moves the Djinn flags to the transferred
/// ones and puts the party back.
void DataTransfer_GiveDefaultDjinn(void) {
  s8 saved[8];
  s8 counts[ELEMENT_COUNT];
  s32 count;
  s32 remaining;
  s32 element;
  s32 djinn;
  s32 flag;
  s32 i;
  s32 k;

  if (GameFlag_Get(FLAG_TRANSFERRED_DJINN_FIRST))
    return;

  count = Party_CountMembers();
  for (i = 0; i < count; i++)
    saved[i] = gMainRoutineWork.party[i];
  for (i = 0; i < count; i++)
    Party_RemoveMember(saved[i]);
  for (i = ISAAC; i <= MIA; i++)
    Party_AddMember(i);
  // Isaac's Flint and Mia's Fizz, which The Broken Seal gives at its start.
  Djinn_AddFoundToOwner(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
  Djinn_AddFoundToOwner(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));

  // Each element's Djinn so far: Flint and Fizz.
  counts[ELEMENT_VENUS] = 1;
  counts[ELEMENT_MERCURY] = 1;
  counts[ELEMENT_MARS] = 0;
  counts[ELEMENT_JUPITER] = 0;
  remaining = DEFAULT_TRANSFER_DJINN;
  do {
    element = Random_Below(ELEMENT_COUNT);
    djinn = Random_Below(TRANSFERRED_DJINN_PER_ELEMENT);
    if (!GameFlag_Get(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn))) {
      s8 low;
      s8 high;

      // Would the elements' counts stay within one with this Djinni?
      counts[element]++;
      low = 9; // above any element's count
      high = 0;
      for (i = 0; i < ELEMENT_COUNT; i++) {
        if (low > counts[i])
          low = counts[i];
        if (high < counts[i])
          high = counts[i];
      }
      counts[element]--;
      if (high - low <= 1 &&
          (Random_Below(100)) < gDataTransfer_DjinnChances[element][djinn]) {
        Djinn_GiveToParty(element, djinn);
        counts[element]++;
        remaining--;
      }
    }
  } while (remaining != 0);
  Djinn_ActivateOfferedForParty();

  for (i = 0, k = 0; i < ELEMENT_COUNT;
       i++, k += TRANSFERRED_DJINN_PER_ELEMENT) {
    s32 j;

    for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++) {
      djinn = i * DJINN_PER_ELEMENT + j;
      flag = FLAG_TRANSFERRED_DJINN_FIRST + k + j;
      if (GameFlag_Get(FLAG_DJINNI(djinn)))
        GameFlag_Set(flag);
      GameFlag_Clear(FLAG_DJINNI(djinn));
    }
  }
  for (i = ISAAC; i <= MIA; i++)
    Party_RemoveMember(i);
  for (i = 0; i < count; i++)
    Party_AddMember(saved[i]);
}

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
UNIT_DEFINE_ADD(s16, Unit_AddPp, pp, ppMax)

#undef UNIT_DEFINE_SET
#undef UNIT_DEFINE_ADD

#undef UNIT_STAT_DIFF
#undef SCALE_EFFECT
#undef BUILD_STAMP_MARK
#undef BUILD_STAMP_DEBUG
#undef BUILD_STAMP_SECONDS_PER_MINUTE
#undef BUILD_STAMP_MINUTES_PER_HOUR
#undef SCENE_STATE_WORDS
#undef NEW_GAME_COINS
#undef SCENE_STATE_SNAPSHOT_SLOTS
#undef SCENE_STATE_RANDOM_ROWS
#undef SCENE_STATE_RANDOM_ROW_COUNT
#undef SCENE_STATE_RANDOM_ROW_BYTES
#undef SCENE_STATE_TABLE_COPY
#undef DJINN_COUNT_ABOVE_ANY
#undef DEFAULT_TRANSFER_DJINN
