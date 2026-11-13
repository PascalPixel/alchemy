/// @file
/// The spoils: coins, experience, items and level ups.
// Part of presentation.c.

#include "../presentation_internal.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "module/battle/presentation.h"

// XXX: what this encodes, and whether item ids here are wider than The Broken
// Seal's 9 bits; its callers would settle it.
u32 BattleSpoils_RankItem(u32 value) {
  u32 bank = 0;
  u32 base = value & 0x1ff;
  if (base == 0)
    return 0;
  {
    u8 flags = Item_Get(base)->flags;

    if (flags & 8)
      bank = 1;
    bank <<= 1;
    if (flags & 4)
      bank++;
    bank <<= 9;
    bank += base;
  }
  return bank;
}

/// Empties the battle's spoils and the party's pending item, as The Broken
/// Seal's BattleSpoils_Clear does. Called by Func_081197f0.
void BattleSpoils_Clear(void) {
  struct BattleWork *battle;
  struct BattleSpoils *spoils;
  u16 *items;
  s32 index;

  battle = Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  gMainRoutineWork.tally25c = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = battle->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}

// The scan looks for the first of FIRST_DIGIT_COUNT character codes from
// FIRST_DIGIT_BASE: the digits 1 to 9 outside Japan, and the codes 65 to 90 in
// the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIRST_DIGIT_BASE 65
#define FIRST_DIGIT_COUNT 26
#else
#define FIRST_DIGIT_BASE 49
#define FIRST_DIGIT_COUNT 9
#endif

s32 Func_08128228(u8 *s) {
  s32 i;

  for (i = 0; i <= 13; i++) {
    if ((u8)(s[i] - FIRST_DIGIT_BASE) <= FIRST_DIGIT_COUNT - 1)
      return s[i] - FIRST_DIGIT_BASE;
  }
  return FIRST_DIGIT_COUNT;
}

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BattleUnit *unit,
                                                      u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 6) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

static __inline__ s32
BattleEnemy_CalculateExperienceReward(struct BattleUnit *unit, u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 4) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

/// Counts a fallen enemy and adds its spoils, The Broken Seal's
/// BattleEnemy_RecordDefeat with the battle work from its heap slot. This game
/// also keeps the last two defeated enemies (their ids and Func_08128228's
/// first digit of their names, alternating slots, with a count up to 2) and
/// sets the seen flag only for enemy ids below 0x200.
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus) {
  struct BattleUnit *unit;
  const struct EnemyDefinition *enemy;
  struct DefeatBattleWork *battle;
  struct DefeatSpoils *spoils;
  s32 itemOrClassIndex;
  s32 dropShift;
  s32 classSlot;
  s32 lowestItemPriority;
  s32 lowestSlot;
  s32 itemPriority;
  s32 slot;
  s32 classIndex;

  unit = Unit_Get__f11f66c(unitId);
  battle = (struct DefeatBattleWork *)Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  if (unitId >= 0 && unitId < 8)
    return -1;
  if (unit->classIndex != 0)
    return -2;
  slot = spoils->lastSlot = (spoils->lastSlot + 1) % 2;
  spoils->lastEnemyIds[slot] = unit->enemyId;
  spoils->lastDigits[slot] = Func_08128228((u8 *)unit);
  if (spoils->lastCount <= 1)
    spoils->lastCount++;
  classSlot = 0;
  classIndex = 0;
  if (battle->enemyClasses[0] != unit->enemyId) {
    do {
      if (++classIndex > 5)
        break;
    } while (battle->enemyClasses[classIndex] != unit->enemyId);
  }
  if (classIndex != 6)
    classSlot = classIndex;
  if (battle->defeatState != 2) {
    if (classSlot < battle->firstDefeated)
      battle->firstDefeated = classSlot;
    if (spoils->defeated)
      battle->defeatState = 1;
  }
  spoils->defeated++;
  if (GameFlag_Test(0x173))
    return 0;
  if (unit->enemyId < 0x200)
    GameFlag_SetBit(unit->enemyId + 0x600);
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (djinnBonus != 0) {
    if (enemy->coins != 0)
      spoils->coins += BattleEnemy_CalculateCoinReward(unit, &enemy->coins);
    if (enemy->experience != 0)
      spoils->experience +=
          BattleEnemy_CalculateExperienceReward(unit, &enemy->experience);
  } else {
    spoils->coins += enemy->coins;
    spoils->experience += enemy->experience;
  }
  if (enemy->item == 0 || enemy->itemChance == 0)
    return 0;
  itemOrClassIndex = 0;
  if (spoils->items[0] != enemy->item) {
    do {
      if (++itemOrClassIndex > 3)
        break;
    } while (spoils->items[itemOrClassIndex] != enemy->item);
  }
  if (itemOrClassIndex == 4) {
    dropShift = enemy->itemChance;
    if (djinnBonus != 0)
      dropShift -= 2;
    if (dropShift < 0)
      dropShift = 0;
    if ((0x20000 >> dropShift) > (Battle_RandomFar() & 0xffff)) {
      lowestSlot = -1;
      lowestItemPriority = 0x40000000;
      for (itemOrClassIndex = 0; itemOrClassIndex <= 3; itemOrClassIndex++) {
        itemPriority = BattleSpoils_RankItem(spoils->items[itemOrClassIndex]);
        if (itemPriority < lowestItemPriority) {
          lowestItemPriority = itemPriority;
          lowestSlot = itemOrClassIndex;
        }
      }
      if ((s32)BattleSpoils_RankItem(enemy->item) > lowestItemPriority)
        spoils->items[lowestSlot] = enemy->item;
    }
  }
  return 0;
}

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 living;
  u16 *list;
  s32 count;
  s32 i;
  s32 unitId;
  struct BattleUnit *unit;
  struct BattleUnit *backup;
  s32 cnt;
  u32 learned;
  s32 j;
  s32 best;
  s32 bestSlot;
  s32 slot;
  u16 *found;
  s32 value;
  s16 gains[8];
  u16 units[10];

  spoils = (struct BattleSpoils *)((u8 *)Ram_HeapSlots->battleWork + 0x550);
  if (spoils->experience != 0) {
    MessageArgument_PushFar(spoils->experience, 5);
    // "You got N experience points."
    BattleMessage_ShowAndWaitFar((s32)MsgGotExperience);
    BattleMessage_WaitForAdvance();
  }
  list = units;
  living = BattleParty_ListLivingUnits(1, list);
  count = BattleUnit_ListLiving(1, list);
  backup = Heap_AllocEwram(sizeof(struct BattleUnit));
  for (i = 0; i < count; i++) {
    unitId = list[i];
    unit = Unit_Get__f11f66c(unitId);
    if (i >= living)
      unit->experience += (u32)spoils->experience >> 1;
    else
      unit->experience += spoils->experience;
    while (Iwram_CopyWordsEntry(backup, unit, sizeof(struct BattleUnit)),
           Unit_TryLevelUpFar(unitId, gains) != 0) {
      Sound_PlayFar(89);
      MessageArgument_ClearAllFar();
      MessageArgument_PushFar(unit->classIndex, 3);
      MessageArgument_PushFar(list[i], 1);
      MessageArgument_PushFar(unit->level, 5);
      // "<name> is now a level N ..."
      BattleMessage_ShowAndWaitFar((s32)MsgLevelUp);
      BattleMessage_WaitForAdvance();
      for (cnt = 0; cnt < 32; cnt++) {
        learned = unit->actionSlots[cnt].encodedAction;
        if ((learned & ACTION_ID_MASK) && (learned >> 15)) {
          for (j = 0; j < 32; j++) {
            if (learned == backup->actionSlots[j].encodedAction)
              break;
          }
          if (j == 32) {
            MessageArgument_ClearAllFar();
            MessageArgument_PushFar(unit->classIndex, 3);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(learned & ACTION_ID_MASK, 4);
            Sound_PlayFar(154);
            // "Mastered the <class>'s <ability>"
            BattleMessage_ShowAndWaitFar((s32)MsgMasteredAbility);
            BattleMessage_WaitForAdvance();
          }
        }
      }
      if (gains[2] != 0) {
        MessageArgument_PushFar(gains[2], 5);
        // "Maximum HP rises by N!"
        BattleMessage_ShowAndWaitFar((s32)MsgMaxHpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[3] != 0) {
        MessageArgument_PushFar(gains[3], 5);
        BattleMessage_ShowAndWaitFar((s32)MsgMaxPpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[4] != 0) {
        MessageArgument_PushFar(gains[4], 5);
        BattleMessage_ShowAndWaitFar((s32)MsgAttackRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[5] != 0) {
        MessageArgument_PushFar(gains[5], 5);
        BattleMessage_ShowAndWaitFar((s32)MsgDefenseRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[6] != 0) {
        MessageArgument_PushFar(gains[6], 5);
        BattleMessage_ShowAndWaitFar((s32)MsgAgilityRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[7] != 0) {
        MessageArgument_PushFar(gains[7], 5);
        // "Luck rises by N!"
        BattleMessage_ShowAndWaitFar((s32)MsgLuckRises);
        BattleMessage_WaitForAdvance();
      }
    }
  }
  Heap_Release(backup);
  if (spoils->coins != 0) {
    MessageArgument_PushFar(spoils->coins, 5);
    // "You got N coins."
    BattleMessage_ShowAndWaitFar((s32)MsgGotCoins);
    Party_AdjustCoinsFar(spoils->coins);
    BattleMessage_WaitForAdvance();
  }
  found = &Data_0200049c;
  for (;;) {
    bestSlot = -1;
    best = -1;
    for (slot = 0; slot <= 3; slot++) {
      if (spoils->items[slot] != 0) {
        value = BattleSpoils_RankItem(spoils->items[slot]);
        if (value >= best) {
          best = value;
          bestSlot = slot;
        }
      }
    }
    if (bestSlot == -1)
      break;
    MessageArgument_PushFar(spoils->items[bestSlot], 2);
    // "You got <item>."
    BattleMessage_ShowAndWaitFar((s32)MsgGotItem);
    BattleMessage_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[bestSlot]) == -1) {
      *found = spoils->items[bestSlot];
      break;
    }
    spoils->items[bestSlot] = 0;
  }
}
#undef FIRST_DIGIT_BASE
#undef FIRST_DIGIT_COUNT
#undef ACTION_ID_MASK
