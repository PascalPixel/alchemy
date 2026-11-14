/// @file
/// The party's units and their stats, a new game, the prologue, HP and PP.
// Part of game.c.

#include "../game_internal.h"
#include "flags.h"
#include "macros.h"
#include "flag_call.h"
#include "gamepak/far_call/field_sequence.h"
#include "module/game/game.h"
#include "gamepak/far_call/window.h"
#include "system/ram_buffer.h"
#include "resource_ids.h"

extern u16 gData_02003860;

extern s16 gSaveSlot;

/// Starts a new game: clears the main routine's work and restarts at the
/// opening.
void Game_RestartQuest(void) {
  MainRoutineWork_InitializeDefaults();
  MainRoutine_RunFar(0);
}

struct TradeOfferState *Djinn_GetRecoveryTable(s32 which) {
  if (which != 0) {
    return Unit_Get(0x83);
  }
  return (struct TradeOfferState *)&gMainRoutineWork.summonMask;
}

u32 Party_GetAverageLevel(void) {
  s32 count;
  s32 total;
  s32 i;

  total = 0;
  count = Party_CountMembers();
  if (count == 0) {
    return 0;
  }
  for (i = 0; i < count; i++) {
    total += ((struct BattleUnit *)Unit_Get(gMainRoutineWork.party[i]))->level;
  }
  total = total / count;
  return total;
}

/// Returns the definition of enemy selector; enemy ids start past the eight
/// characters', so that one id names any unit.
const struct EnemyDefinition *Enemy_GetDefinition(s32 selector) {
  u32 recordIndex;

  recordIndex = selector - ENEMY_ID_FIRST;
  // BUG: the table has ENEMY_COUNT rows, but only rows past 999 are refused, so
  // the rows from ENEMY_COUNT read the tables after it.
#ifdef BUGFIX
  if (recordIndex > ENEMY_COUNT - 1U)
#else
  if (recordIndex > 999U)
#endif
    recordIndex = 0;
  return &gEnemy_DefinitionTable[recordIndex];
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
  struct BattleUnit *unit;
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
  stats->actionCount = unit->baseActionCount & 15;
  stats->hpRegen = unit->baseHpRegen;
  stats->ppRegen = unit->basePpRegen;
  {
    s16 *base = &unit->baseElement[0].power;
    s32 *total = &stats->element[0].power;

    for (index = 0; index < ELEMENT_COUNT; index++) {
      total[0] = base[0];
      total[1] = base[1];
      base += 2;
      total += 2;
    }
  }

  // HP and PP keep their share of the maximum; gauges that no longer match are
  // reset to full first.
  if (UNIT_STAT_DIFF(unit->maxHp * unit->hpGauge / UNIT_GAUGE_FULL, unit->hp) >
          1 ||
      UNIT_STAT_DIFF(unit->maxPp * unit->ppGauge / UNIT_GAUGE_FULL, unit->pp) >
          1) {
    unit->hpGauge = UNIT_GAUGE_FULL;
    unit->ppGauge = UNIT_GAUGE_FULL;
    unit->hp = unit->maxHp;
    unit->pp = unit->maxPp;
  }

  // CURSE_KEEP_BOUND keeps CURSE_BOUND, and CURSE_REMOVABLE clears both again
  // below.
  unit->restraint &= ~(CURSE_BOUND | CURSE_ITEM);
  if (unit->restraint & CURSE_KEEP_BOUND)
    unit->restraint |= CURSE_BOUND;
  if (unit->ready)
    stats->actionCount++;
  unit->equipBonus[0] = 0;

  // Apply additive equipment and set-Djinn bonuses before class scaling.
  // An enemy (CLASS_NONE) skips equipment, Djinn and class entirely.
  if (unit->classIndex != CLASS_NONE) {
    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        continue;
      stats->item = Item_GetDirect(unit->inventory[index]);
      if (stats->item->flags & ITEM_CURSED)
        unit->restraint |= CURSE_BOUND | CURSE_ITEM;
      stats->atk += stats->item->primaryBonus;
      /* FAKEMATCH(barrier): an empty asm after the Attack bonus. */
      asm("");
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
        case EQUIP_EFFECT_CURSE_REMOVABLE:
          unit->restraint |= CURSE_REMOVABLE;
          break;
        case EQUIP_EFFECT_ADD_ACTIONS:
          stats->actionCount += stats->amount;
          break;
        }
      }
    }
    if (unit->restraint & CURSE_REMOVABLE)
      unit->restraint &= ~(CURSE_REMOVABLE | CURSE_BOUND);

    // Only set Djinn contribute their stat bonuses.
    for (elementIndex = 0; elementIndex < ELEMENT_COUNT; elementIndex++) {
      u32 setDjinn = unit->djinnActive[elementIndex];

      for (index = 0; index < DJINN_PER_ELEMENT; index++) {
        if (setDjinn & (1 << index)) {
          const struct DjinnDefinition *djinn =
              (const struct DjinnDefinition *)Djinn_GetDefinition(elementIndex,
                                                                  index);

          stats->hp += djinn->hp;
          stats->pp += djinn->pp;
          stats->atk += djinn->atk;
          stats->def += djinn->def;
          stats->agi += djinn->agi;
          stats->luck += djinn->luck;
        }
      }
    }

    // The class's multipliers are in tenths (STAT_SCALE_ONE keeps a stat).
    {
      /* FAKEMATCH(signature): the class read through ClassScaleView, which has
       * no word member, so the multipliers load ahead of the stores. */
      struct ClassScaleView *scale =
          (struct ClassScaleView *)Class_GetDefinition(unit->classIndex);

      stats->hp = stats->hp * scale->hp / STAT_SCALE_ONE;
      stats->pp = stats->pp * scale->pp / STAT_SCALE_ONE;
      stats->atk = stats->atk * scale->atk / STAT_SCALE_ONE;
      stats->def = stats->def * scale->def / STAT_SCALE_ONE;
      stats->agi = stats->agi * scale->agi / STAT_SCALE_ONE;
      stats->luck = stats->luck * scale->luck / STAT_SCALE_ONE;
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
        (unit->elementLevels[index] * unit->elementLevels[index] +
         unit->elementLevels[index]) *
        5;
  for (index = 0; index < ELEMENT_COUNT; index++)
    stats->element[index].resistance += unit->resistanceStage * 20;

  if (unit->classIndex != CLASS_NONE) {
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
  if (unit->classIndex == CLASS_NONE) {
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
  unit->actionEntryCount = stats->actionCount;
  unit->hpRegen = stats->hpRegen;
  unit->ppRegen = stats->ppRegen;
  for (index = 0; index < ELEMENT_COUNT; index++) {
    unit->element[index].power = stats->element[index].power;
    unit->element[index].resistance = stats->element[index].resistance;
  }

  if (unit->classIndex == CLASS_NONE) {
    hpLimit = UNIT_ENEMY_HP_MAX;
    ppLimit = UNIT_HP_MAX;
  } else {
    hpLimit = ppLimit = UNIT_CHARACTER_HP_MAX;
  }

  previousMaximum = unit->maxHp;
  CLAMP(stats->hp, 0, hpLimit);
  unit->maxHp = stats->hp;
  if (previousMaximum != unit->maxHp) {
    value = stats->hp * unit->hpGauge / UNIT_GAUGE_FULL;
    CLAMP(value, 0, hpLimit);
    if (unit->hp != 0 && value == 0)
      value = 1;
    unit->hp = value;
  }

  previousMaximum = unit->maxPp;
  CLAMP(stats->pp, 0, ppLimit);
  unit->maxPp = stats->pp;
  if (previousMaximum != unit->maxPp) {
    value = stats->pp * unit->ppGauge / UNIT_GAUGE_FULL;
    CLAMP(value, 0, ppLimit);
    if (unit->pp != 0 && value == 0)
      value = 1;
    unit->pp = value;
  }

  Heap_Release(stats);
}

void GameFlag_RefreshLureCap(void) {
  s32 partySize;
  s32 partyIndex;

  GameFlag_Clear(0x167);
  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct OwnerInventoryState *unit;
    s32 inventorySlot;

    unit = Unit_Get(gMainRoutineWork.party[partyIndex]);
    for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        struct EquipEffect *effect;
        s32 effectIndex;

        effect = Item_GetDirect(unit->inventory[inventorySlot])->effects;
        for (effectIndex = 0; effectIndex < 4; effectIndex++) {
          u8 effectKind;

          effectKind = effect->kind;
          effect++;
          if (effectKind == 27) {
            GameFlag_Set(0x167);
          }
        }
      }
    }
  }
}

static __inline__ s32 Owner_GetPpRatio(s32 owner) {
  struct BattleUnit *unit = Unit_Get(owner);

  if (unit->maxPp <= 0)
    return 0;
  return (unit->pp << 16) / unit->maxPp;
}

/// Returns the party's PP over its maximum PP, in 16.16; 0 with no members.
s32 Party_GetPpRatio(void) {
  s32 pp = 0;
  s32 max = 0;
  s32 count = Party_CountMembers();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(gMainRoutineWork.party[i]);

    pp += unit->pp;
    max += unit->maxPp;
  }
  if (max == 0)
    return 0;
  return (pp << 16) / max;
}

/// Takes 1 PP from the member whose PP is fullest for its maximum.
s32 Party_DrainPpFromFullest(void) {
  s32 best = 0;
  s32 bestOwner = -1;
  s32 count = Party_CountMembers();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    s32 ratio = Owner_GetPpRatio(gMainRoutineWork.party[i]);

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

/// Takes count PP from the party, one at a time from the fullest member.
void Party_DrainPp(s32 count) {
  s32 i;

  for (i = 0; i < count; i++)
    Party_DrainPpFromFullest();
  Party_GetPpRatio();
}

u16 BuildStamp_GetTime(void) {
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
    result |= ~0x7fff;
  }
  return result;
}

#define DMA3 ((volatile struct DmaRegisters *)&REG_DMA3SAD)
#define DMA3_BUSY (DMA_ENABLE << 16)
#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

extern s8 gSceneLocalState[];
#ifndef MODERN
// A current GCC refuses the volatile redeclaration and reads frame.h's.
extern volatile u8 gIdleSleepEnabled;
#endif

/// Starts a new game's state: clears gMainRoutineWork, the work block, the
/// flags and the owners by DMA, builds the characters, sets the defaults (the
/// button layout, Felix and Jenna in the party, their first Psynergy, 200
/// coins, the message settings and the play time clock) and fills the work
/// block's random tables.
void MainRoutineWork_InitializeDefaults(void) {
  volatile s32 zero;
  s32 i;
  s32 j;

  zero = 0;
  START_DMA(&zero, &gMainRoutineWork,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 184);
  zero = 0;
#if TLA_PATCH_1
  START_DMA(&zero, gSceneLocalState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 1992);
  WaitDma(DMA3);
#else
  {
    /* FAKEMATCH(pin): the channel kept in r3 through the transfer and copied
     * from there for the wait. */
    register u32 held asm("r3") = (u32)DMA3;
    struct DmaRegisters volatile *channel;

    Dma_Set((const void *)&zero, gSceneLocalState,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 1992,
            (volatile u32 *)held);
    channel = (struct DmaRegisters volatile *)held;
    held = DMA3_BUSY;
    while ((channel->control & held) != 0) {
    }
  }
#endif
  zero = 0;
  START_DMA(&zero, gGameFlagBytes,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 128);
  gSceneLocalState[4] = 0xff;
  zero = 0;
  START_DMA(&zero, gOwnerStates,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 664);
  Party_InitializeCharacters();
  gMainRoutineWork.aKeys = 1;
  gMainRoutineWork.bKeys = 2;
  gMainRoutineWork.selectKeys = 4;
  gMainRoutineWork.startKeys = 8;
  gMainRoutineWork.firstShortcutKeys = 0x200;
  gMainRoutineWork.secondShortcutKeys = 0x100;
  gMainRoutineWork.dashKeys = 2;
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gMainRoutineWork.leaderActorId = 4;
  Party_AddMember(4);
  Party_AddMember(5);
  Unit_LearnPsynergy(4, 149);
  Unit_LearnPsynergy(4, 140);
  Unit_LearnPsynergy(6, 141);
  Unit_LearnPsynergy(7, 33);
  Unit_LearnPsynergy(0, 149);
  Unit_LearnPsynergy(0, 140);
  Unit_LearnPsynergy(1, 140);
  Unit_LearnPsynergy(2, 141);
  Unit_LearnPsynergy(2, 144);
  gMainRoutineWork.coins = 200;
  *(u32 *)&gMainRoutineWork.unk284[8] = 0;
  gMainRoutineWork.forgeItem = 0;
  gMainRoutineWork.countedAnswers = 0;
  gMainRoutineWork.messageSpeed = 1;
  gMainRoutineWork.speech = 1;
  gMainRoutineWork.unusedAfterSpeech = 1;
  gMainRoutineWork.paletteGlow[0] = 0;
  gMainRoutineWork.paletteGlow[1] = 8;
  gMainRoutineWork.buildStamp = 0;
  // Its result taken as a whole word, where the definition returns u16.
  gMainRoutineWork.startStamp = ((u32 (*)(void))BuildStamp_GetTime)();
  gPlayTimeFrames = 0;
  gIdleSleepEnabled = 0;
  gMainRoutineWork.playTime = 0;
  /* FAKEMATCH(volatile): the auto-sleep flag. */
  gMainRoutineWork.unk244[6] = gIdleSleepEnabled;
  gIdleFrames = 0;
  gSaveSlot = -1;
  gMainRoutineWork.unk284[2] = gData_02003860;
  gMainRoutineWork.unk13d[0] = 4;
  gMainRoutineWork.unk13d[1] = 4;
  gMainRoutineWork.unk13d[2] = 4;
  gMainRoutineWork.unk13d[3] = 8;
  gMainRoutineWork.unk13d[4] = 8;
  gMainRoutineWork.unk13d[5] = 8;
  gMainRoutineWork.unk13d[6] = 16;
  gMainRoutineWork.unk13d[7] = 16;
  gMainRoutineWork.unk13d[8] = 16;
  gMainRoutineWork.unk13d[9] = 32;
  gMainRoutineWork.unk13d[10] = 32;
  gMainRoutineWork.unk13d[11] = 32;
  gMainRoutineWork.unk13d[12] = 64;
  gMainRoutineWork.unk13d[13] = 64;
  gMainRoutineWork.unk13d[14] = 64;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++)
      gSceneLocalState[0x11b8 + i * 12 + j] = (Random16() * 5) >> 16;
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gSceneLocalState[0x1338 + i * 5 + j] = Data_080b1f2c[i][j];
  }
}

void Party_SetLeaderJenna(void) { gMainRoutineWork.leaderActorId = 5; }

void Party_SetLeaderFelix(void) {
  gMainRoutineWork.leaderActorId = 4;
  Party_AddMember(4);
  Party_RemoveMember(5);
  Party_RemoveMember(6);
}

/// Gives Piers his two Mercury Djinn, Spring and Shade (Mercury 9 and 10),
/// set, then evens out the party's Djinn and recalculates its members.
/// Kibombo calls it when Piers joins.
void Party_GivePiersDjinn(void) {
  s32 i;
  s32 count;

  for (i = 10; i < 12; i++) {
    GameFlag_Set(i + 67);
    Djinn_Give(7, 1, i - 1);
    Djinn_Set(7, 1, i - 1);
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
  struct BattleUnit *unit;

  partySize = Party_CountMembers();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    unit = Unit_Get(gMainRoutineWork.party[partyIndex]);

    unit->hp = unit->maxHp;
    unit->pp = unit->maxPp;
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
  struct BattleUnit *unit;

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
    unit->pp = unit->maxPp;
    Unit_UpdateGauges(unit);
  }
}

/// Writes the element and index of unit's last Djinni and returns 0, or
/// returns -1 when it has none.
s32 Unit_FindLastDjinn(s32 unit, s32 *element_out, s32 *index_out) {
  u32 *sets = ((struct BattleUnit *)Unit_Get(unit))->djinnAvailable;
  s32 element = -1;
  s32 index = -1;
  s32 i;
  s32 j;

  for (i = 0; i < 20; i++) {
    for (j = 0; j < 4; j++) {
      if (sets[j] & (1 << i)) {
        element = j;
        index = i;
      }
    }
  }
  if (element == -1)
    return element;
  *element_out = element;
  *index_out = index;
  return 0;
}

/// Until the Djinn counts of the active owners are within one, moves the last
/// Djinni of the owner holding most to the one holding least.
void Party_BalanceDjinn(void) {
  s32 count = Party_CountMembers();

  for (;;) {
    s32 lowNo = 0;
    s32 high = 0;
    s32 low = 999;
    s32 highNo = 0;
    s32 i;
    s32 sum;
    s32 element;
    s32 index;

    for (i = 0; i < count; i++) {
      s32 owner = gMainRoutineWork.party[i];
      struct OwnerDjinnState *unit = Unit_Get(owner);
      u8 *counts;
      s32 j;

      sum = 0;
      counts = unit->counts.banks.owned;
      j = 3;
      do {
        u8 value = *counts;
        counts++;
        sum += value;
        j--;
      } while (j >= 0);
      if (sum > high) {
        high = sum;
        highNo = owner;
      }
      if (sum < low) {
        low = sum;
        lowNo = owner;
      }
    }
    if (high - low <= 1)
      break;
    sum = Unit_FindLastDjinn(highNo, &element, &index);
    if (sum != 0)
      break;
    Djinn_Transfer(highNo, element, index, lowNo);
  }
}

s32 FlagBits_Test(u8 *bytes, s32 flag) {
  s32 bit;
  u32 index;

  bit = flag & 7;
  index = (u32)flag << 20;
  flag = index >> 23;
  return (bytes[flag] >> bit) & 1;
}

/// The data transfer from Golden Sun ("Transfer data from Golden Sun",
/// MsgTransferDataFromGoldenSun): copies a The Broken Seal save's Djinn, six of
/// its flags, its coins, its options (when it came by link rather than by
/// password) and its four units into this game's state.
void DataTransfer_Apply(u8 *save, s32 byPassword) {
  u8 *flags = save + 0x40;
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  struct MainRoutineWork *state;
  u8 name[16];
  s32 i;
  s32 k;
  s32 slot;
  s32 count;
  s32 j;

  // Each element's first seven Djinn, The Broken Seal's, become the transferred
  // Djinn flags.
  for (i = 0, k = 0; i < ELEMENT_COUNT;
       i++, k += TRANSFERRED_DJINN_PER_ELEMENT) {

    for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++) {
      s32 flag = FLAG_TRANSFERRED_DJINN_FIRST + k + j;

      if (FlagBits_Test(flags, FLAG_DJINNI(i * DJINN_PER_ELEMENT + j)))
        GameFlag_Set(flag);
      else
        GameFlag_Clear(flag);
      /* FAKEMATCH(barrier): an empty asm reading j. */
      asm("" : : "r"(j));
    }
  }
  for (i = 0; i < 6; i++) {
    if (FlagBits_Test(flags, gDataTransfer_Flags[i]))
      GameFlag_Set(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + i);
    else
      GameFlag_Clear(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + i);
  }
  if (GameFlag_Get(FLAG_TRANSFERRED_HSU_RESCUED_BY_PARTY))
    GameFlag_Clear(FLAG_TRANSFERRED_HSU_RESCUED_BY_PARTY);
  else
    GameFlag_Set(FLAG_TRANSFERRED_HSU_RESCUED_BY_PARTY);

  state = &gMainRoutineWork;
  *(s32 *)&state->unk284[8] = party->coins;
  if (byPassword == 0) {
    state->paletteGlow[0] = party->paletteGlow[0];
    state->paletteGlow[1] = party->paletteGlow[1];
    state->messageSpeed = party->messageSpeed;
    state->speech = party->speech;
    state->unk244[6] = party->unk244[6];
    WindowColor_ApplyFar(state->paletteGlow[0], state->paletteGlow[1]);
  }

  j = 4; // A local reused for a second value: the unit loop's bound.
  for (i = 0; i < (s32)j; i++) {
    struct BattleUnit *unit = Unit_Get(i);

    memcpy(name, &units[i], 15);
    Iwram_CopyWordsEntry(unit, &units[i], sizeof(struct BattleUnit));
    memcpy(unit, name, 15);
    if (byPassword == 0)
      unit->enemyId = unit->classId;
    for (slot = INVENTORY_SLOTS - 1; slot >= 0; slot--) {
      s32 item = unit->inventory[slot];

      if (item != 0 && (Item_GetDirect(item)->flags & ITEM_NOT_TRANSFERRED)) {
        while (Inventory_Remove(i, slot) == 1)
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
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  u8 unused[16]; // Unused: gives the stack frame its 16 bytes.
  s32 i;
  s32 count;

  gMainRoutineWork.paletteGlow[0] = party->paletteGlow[0];
  gMainRoutineWork.paletteGlow[1] = party->paletteGlow[1];
  gMainRoutineWork.messageSpeed = party->messageSpeed;
  gMainRoutineWork.speech = party->speech;
  gMainRoutineWork.unk244[6] = party->unk244[6];
  WindowColor_ApplyFar(gMainRoutineWork.paletteGlow[0],
                       gMainRoutineWork.paletteGlow[1]);
  count = 8;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(i);
    struct BattleUnit *saved = &units[i];

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
      if (GameFlag_Get(FLAG_TRANSFERRED_DJINN_FIRST + k + j))
        GameFlag_Set(FLAG_DJINNI(i * DJINN_PER_ELEMENT + j));
      /* FAKEMATCH(barrier): an empty asm reading j. */
      asm("" : : "r"(j));
    }
    /* FAKEMATCH(barrier): an empty asm on i. */
    asm("" : "+r"(i));
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
  GameFlag_Set(FLAG_REUNITED);

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
    gSceneLocalState[0x1950 + i] = djinni;
  }
  // The Psynergy items Isaac's party carried in The Broken Seal.
  Party_GiveItemIfMissing(ITEM_FROST_JEWEL, MIA);
  Party_GiveItemIfMissing(ITEM_DOUSE_DROP, MIA);
  Party_GiveItemIfMissing(ITEM_LIFTING_GEM, GARET);
  Party_GiveItemIfMissing(ITEM_CARRY_STONE, ISAAC);
  Party_GiveItemIfMissing(ITEM_CATCH_BEADS, IVAN);
  gMainRoutineWork.coins += *(s32 *)&gMainRoutineWork.unk284[8];
#if TLA_PATCH_4
  if ((u32)gMainRoutineWork.coins > COINS_MAX)
    gMainRoutineWork.coins = COINS_MAX;
#endif
}

/// Does nothing; nothing calls Unit_ReservedNoOpFar.
void Unit_ReservedNoOp(void) {}

void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn) {
  s32 flag = element * 20 + djinn;

  Djinn_Give(owner, element, djinn);
  flag += 0x30;
  DjinnRecovery_AddStandby(owner, element, djinn);
  GameFlag_Set(flag);
}

/// Stands in for transferred data when the player transfers none: unless the
/// transferred Djinn are already set, sets the party aside, makes Isaac's
/// party the party, gives it Flint and Fizz, then hands out 16 random Djinn of
/// The Broken Seal's still unowned, keeping the elements' counts within one,
/// each with its chance in gDataTransfer_DjinnChances; then moves the Djinn
/// flags to the transferred ones and puts the party back.
void DataTransfer_GiveDefaultDjinn(void) {
  s8 saved[8];
  s8 counts[4];
  s32 count;
  s32 remaining;
  s32 element;
  s32 djinn;
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
  Djinn_AddFoundToOwner(ISAAC, ELEMENT_VENUS, 0);
  Djinn_AddFoundToOwner(MIA, ELEMENT_MERCURY, 0);

  counts[0] = 1;
  counts[1] = 1;
  counts[2] = 0;
  counts[3] = 0;
  remaining = 16;
  do {
    element = (Random16() * ELEMENT_COUNT) >> 16;
    djinn = (Random16() * TRANSFERRED_DJINN_PER_ELEMENT) >> 16;
    if (!GameFlag_Get(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn))) {
      s8 low;
      s8 high;

      counts[element]++;
      low = 9;
      high = 0;
      for (i = 0; i < 4; i++) {
        if (low > counts[i])
          low = counts[i];
        if (high < counts[i])
          high = counts[i];
      }
      counts[element]--;
      if (high - low <= 1 && ((Random16() * 100) >> 16) <
                                 gDataTransfer_DjinnChances[element][djinn]) {
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
    s32 flag;

    for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++) {
      /* FAKEMATCH(temporary): the flag number held in a local. */
      flag = FLAG_TRANSFERRED_DJINN_FIRST + k + j;
      // element carries the flag to read after the one to set.
      element = FLAG_DJINNI(i * DJINN_PER_ELEMENT + j);
      if (GameFlag_Get(element))
        GameFlag_Set(flag);
      GameFlag_Clear(element);
    }
  }
  for (i = ISAAC; i <= MIA; i++)
    Party_RemoveMember(i);
  for (i = 0; i < count; i++)
    Party_AddMember(saved[i]);
}

/// Recomputes the HP and PP gauges of `unitId` from its current HP and PP, so
/// that Unit_Calculate keeps the same fractions. Called after anything that
/// writes HP or PP directly: Unit_AddHp, the inn, the sanctum, item use,
/// battle.
void Unit_RecalculateGauges(s32 ownerId) {
  s32 first;
  s32 second;
  s32 firstValue;
  s32 secondValue;
  struct BattleUnit *owner;

  owner = Unit_Get(ownerId);
  first = (owner->hp << 14) / owner->maxHp;
  firstValue = 0x4000;
  if (first <= 0x4000) {
    firstValue = 0;
    if (first >= 0) {
      firstValue = first;
    }
  }
  owner->hpGauge = firstValue;
  if (owner->hpGauge == 0 && owner->hp != 0) {
    firstValue = 1;
    owner->hpGauge = firstValue;
  }
  second = (owner->pp << 14) / owner->maxPp;
  secondValue = 0x4000;
  if (second <= 0x4000) {
    secondValue = 0;
    if (second >= 0) {
      secondValue = second;
    }
  }
  owner->ppGauge = secondValue;
  if (owner->ppGauge == 0 && owner->pp != 0) {
    secondValue = 1;
    owner->ppGauge = secondValue;
  }
}

/// Sets the unit's HP, clamped to 0..maxHp, and recomputes its HP and PP
/// gauges inline.
void Unit_SetHp(struct BattleUnit *unit, s32 hp) {
  s32 value;

  if (hp > unit->maxHp) {
    value = unit->maxHp;
  } else {
    value = 0;
    if (hp >= 0) {
      value = hp;
    }
  }
  unit->hp = value;
  value = ((value << 16) >> 2) / unit->maxHp;

  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    unit->hpGauge = output;
    if ((output << 16) == 0 && unit->hp != 0) {
      unit->hpGauge = 1;
    }
  }

  value = (unit->pp << 14) / unit->maxPp;
  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    unit->ppGauge = output;
    if ((output << 16) == 0 && unit->pp != 0) {
      unit->ppGauge = 1;
    }
  }
}

/// Sets the unit's PP, clamped to 0..maxPp, and recomputes its HP and PP
/// gauges.
void Unit_SetPp(struct BattleUnit *unit, s32 pp) {
  s32 clamped;

  if (pp > unit->maxPp) {
    clamped = unit->maxPp;
  } else {
    clamped = 0;
    if (pp >= 0)
      clamped = pp;
  }
  unit->pp = clamped;
  Unit_UpdateGauges(unit);
}

/// Adds delta to the HP of unit owner, clamped to 0..maxHp, recomputes its
/// gauges and returns the new HP.
s32 Unit_AddHp(s32 owner, s32 delta) {
  struct BattleUnit *state = Unit_Get(owner);
  s32 cur = state->hp;
  s32 max = state->maxHp;
  s32 value = cur + delta;
  s32 result;

  if (value > max)
    result = max;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  state->hp = result;
  Unit_RecalculateGauges(owner);
  return state->hp;
}

/// Adds delta to the PP of unit owner, clamped to 0..maxPp, recomputes its
/// gauges and returns the new PP.
s16 Unit_AddPp(s32 owner, s32 delta) {
  struct BattleUnit *state = Unit_Get(owner);
  s32 cur = state->pp;
  s32 max = state->maxPp;
  s32 value = cur + delta;
  s32 result;

  if (value > max)
    result = max;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  state->pp = result;
  Unit_RecalculateGauges(owner);
  return state->pp;
}

#undef STAT_DIFF
#undef DMA3
#undef DMA3_BUSY
#undef START_DMA
