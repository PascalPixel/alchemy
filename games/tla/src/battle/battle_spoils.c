/// @file
/// The spoils: coins, experience, items and level ups.
// Part of battle.c.

#include "battle_internal.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "battle/presentation.h"

/// What a level up raised, as places in the halfwords Unit_TryLevelUp fills
/// (struct LevelUpResult's fields, read here as an array): each is shown when
/// it is not 0.
enum LevelUpGain {
  GAIN_HP = 2,
  GAIN_PP,
  GAIN_ATTACK,
  GAIN_DEFENSE,
  GAIN_AGILITY,
  GAIN_LUCK,
  GAIN_COUNT
};

/// Returns an item's rank among the spoils, by which the four kept are chosen
/// and handed out best first: its item id (INVENTORY_ITEM_ID_MASK's nine
/// bits), with ITEM_RARE's bit above it and ITEM_INDISPENSABLE's above that;
/// 0 for none.
u32 BattleSpoils_RankItem(u32 inventoryEntry) {
  u32 priority = 0;
  u32 itemId = inventoryEntry & INVENTORY_ITEM_ID_MASK;
  if (itemId == 0)
    return 0;
  {
    u8 flags = Item_GetDirectFar(itemId)->flags;

    if (flags & ITEM_INDISPENSABLE)
      priority = 1;
    priority <<= 1;
    if (flags & ITEM_RARE)
      priority++;
    priority <<= 9;
    priority += itemId;
  }
  return priority;
}

/// Above every item's rank: the search for the lowest of the spoils' four
/// starts here.
#define SPOILS_RANK_CEILING (1 << 30)

/// Empties the battle's spoils and the party's pending item. Called by
/// Battle_Run.
void BattleSpoils_Clear(void) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct BattleSpoils *spoils = &battle->spoils;
  u16 *items;
  s32 index;

  gMainRoutineWork.pendingItem = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = battle->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}

/// The name suffixes the scan reads back (NAME_SUFFIX_COUNT of them): the
/// digits 1 to 9 outside Japan, and the letters A to Z in the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIRST_DIGIT_BASE 'A'
#define FIRST_DIGIT_COUNT 26
#else
#define FIRST_DIGIT_BASE '1'
#define FIRST_DIGIT_COUNT 9
#endif

/// Returns an enemy name's suffix: its first digit less one (its first
/// letter's place in the alphabet in Japanese), or FIRST_DIGIT_COUNT when it
/// has none.
s32 String_FindFirstDigit(u8 *name) {
  s32 i;

  for (i = 0; i < UNIT_NAME_SIZE - 1; i++) {
    if ((u8)(name[i] - FIRST_DIGIT_BASE) <= FIRST_DIGIT_COUNT - 1)
      return name[i] - FIRST_DIGIT_BASE;
  }
  return FIRST_DIGIT_COUNT;
}

/// Returns an enemy's coins with the Djinn bonus: its own, plus a six-sided die
/// for every ten levels and one more, at least 30 percent of its own.
static __inline__ s32 BattleEnemy_CalculateCoinReward(struct Unit *unit,
                                                      u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += (Random_Below(6)) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

/// Returns an enemy's experience with the Djinn bonus: its own plus a
/// four-sided die for every ten levels and one more, at least 30 percent more.
static __inline__ s32 BattleEnemy_CalculateExperienceReward(struct Unit *unit,
                                                            u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += (Random_Below(4)) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

/// Counts a fallen enemy and adds its spoils, The Broken Seal's
/// BattleEnemy_RecordDefeat with the battle work from its heap slot. This game
/// also keeps the last two defeated enemies (their ids and
/// String_FindFirstDigit's first digit of their names, alternating slots, with
/// a count up to 2) and sets the defeated flag only for the enemies that have
/// one, below ENEMY_DEFEATED_FLAG_COUNT.
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus) {
  struct Unit *unit;
  const struct EnemyDefinition *enemy;
  struct BattleWork *battle;
  struct BattleSpoils *spoils;
  s32 itemIndex;
  s32 dropShift;
  s32 classSlot;
  s32 lowestItemPriority;
  s32 lowestSlot;
  s32 itemPriority;
  s32 slot;
  s32 classIndex;

  unit = Unit_Get(unitId);
  battle = Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  if (unitId >= 0 && unitId <= ALLY_MAX)
    return -1;
  if (unit->classId != CLASS_NONE)
    return -2;
  // Queued for Regen Dance: the name letter is the first digit of the name.
  slot = spoils->queueHead = (spoils->queueHead + 1) % 2;
  spoils->queuedSummons[slot] = unit->enemyId;
  spoils->queuedLetters[slot] = String_FindFirstDigit((u8 *)unit);
  if (spoils->queueCount <= 1)
    spoils->queueCount++;
  classSlot = 0;
  classIndex = 0;
  // UB: the search runs to a sixth kind, one past enemyKinds: it reads
  // kindsEnd, the halfword after them.
  if (battle->enemyKinds[0] != unit->enemyId) {
    do {
      if (++classIndex > (ENEMY_BATTLE_SIZE - 1))
        break;
    } while (battle->enemyKinds[classIndex] != unit->enemyId);
  }
  if (classIndex != ENEMY_BATTLE_SIZE)
    classSlot = classIndex;
  // The first class to fall names the victory message.
  // XXX (no evidence): whether DEFEAT_BY_PARTY_MEMBER was meant; no state is 2,
  // so the test never fails.
  if (battle->defeatState != 2) {
    if (classSlot < battle->firstDefeated)
      battle->firstDefeated = classSlot;
    if (spoils->defeated)
      battle->defeatState = DEFEAT_SEVERAL;
  }
  spoils->defeated++;
  if (GameFlag_Get(FLAG_MONSTER_BATTLE))
    return 0;
  if (unit->enemyId < ENEMY_DEFEATED_FLAG_COUNT)
    GameFlag_Set(ENEMY_DEFEATED_FLAG(unit->enemyId));
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
  itemIndex = 0;
  if (spoils->items[0] != enemy->item) {
    do {
      if (++itemIndex > 3)
        break;
    } while (spoils->items[itemIndex] != enemy->item);
  }
  if (itemIndex == 4) {
    dropShift = enemy->itemChance;
    if (djinnBonus != 0)
      dropShift -= 2;
    if (dropShift < 0)
      dropShift = 0;
    // Twice FIXED_ONE >> n against a 16-bit random number: 1 in 2^(n - 1).
    if ((FIXED_ONE * 2 >> dropShift) > (Battle_RandomFar() & 0xffff)) {
      lowestSlot = -1;
      lowestItemPriority = SPOILS_RANK_CEILING;
      for (itemIndex = 0; itemIndex <= 3; itemIndex++) {
        itemPriority = BattleSpoils_RankItem(spoils->items[itemIndex]);
        if (itemPriority < lowestItemPriority) {
          lowestItemPriority = itemPriority;
          lowestSlot = itemIndex;
        }
      }
      if ((s32)BattleSpoils_RankItem(enemy->item) > lowestItemPriority)
        spoils->items[lowestSlot] = enemy->item;
    }
  }
  return 0;
}

/// Hands out the spoils after a victory: experience to each party member still
/// standing, with every level gained, then the coins, then the items best
/// first.
void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 living;
  u16 *list;
  s32 count;
  s32 i;
  s32 unitId;
  struct Unit *unit;
  struct Unit *backup;
  s32 entry;
  u32 learned;
  s32 j;
  s32 best;
  s32 bestSlot;
  s32 slot;
  u16 *found;
  s32 value;
  s16 gains[GAIN_COUNT];
  u16 units[10];

  spoils = &Ram_HeapSlots->battleWork->spoils;
  if (spoils->experience != 0) {
    MessageArgument_PushFar(spoils->experience, MESSAGE_ARGUMENT_NUMBER);
    // "You got N experience points."
    BattleMessage_ShowAndWaitFar((s32)MsgGotExperience);
    BattleMessage_WaitForAdvance();
  }
  list = units;
  living = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, list);
  count = BattleUnit_ListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, list);
  backup = Heap_AllocEwram(sizeof(struct Unit));
  for (i = 0; i < count; i++) {
    unitId = list[i];
    unit = Unit_Get(unitId);
    if (i >= living)
      unit->experience += (u32)spoils->experience >> 1;
    else
      unit->experience += spoils->experience;
    while (Iwram_CopyWordsEntry(backup, unit, sizeof(struct Unit)),
           Unit_TryLevelUpFar(unitId, gains) != 0) {
      Sound_PlayFar(MUS_LEVEL_UP);
      MessageArgument_ClearAllFar();
      MessageArgument_PushFar(unit->classId, MESSAGE_ARGUMENT_CLASS);
      MessageArgument_PushFar(list[i], MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(unit->level, MESSAGE_ARGUMENT_NUMBER);
      // "<name> is now a level N ..."
      BattleMessage_ShowAndWaitFar((s32)MsgLevelUp);
      BattleMessage_WaitForAdvance();
      for (entry = 0; entry < PSYNERGY_SLOTS; entry++) {
        learned = unit->psynergy[entry].encodedAction;
        // A class ability the copy lacks is newly learned.
        if ((learned & ACTION_ID_MASK) &&
            (learned >> PSYNERGY_FROM_CLASS_SHIFT)) {
          for (j = 0; j < PSYNERGY_SLOTS; j++) {
            if (learned == backup->psynergy[j].encodedAction)
              break;
          }
          if (j == PSYNERGY_SLOTS) {
            MessageArgument_ClearAllFar();
            MessageArgument_PushFar(unit->classId, MESSAGE_ARGUMENT_CLASS);
            MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
            MessageArgument_PushFar(learned & ACTION_ID_MASK,
                                    MESSAGE_ARGUMENT_ABILITY);
            Sound_PlayFar(SE_CAST);
            // "Mastered the <class>'s <ability>"
            BattleMessage_ShowAndWaitFar((s32)MsgMasteredAbility);
            BattleMessage_WaitForAdvance();
          }
        }
      }
      if (gains[GAIN_HP] != 0) {
        MessageArgument_PushFar(gains[GAIN_HP], MESSAGE_ARGUMENT_NUMBER);
        // "Maximum HP rises by N!"
        BattleMessage_ShowAndWaitFar((s32)MsgMaxHpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[GAIN_PP] != 0) {
        MessageArgument_PushFar(gains[GAIN_PP], MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar((s32)MsgMaxPpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[GAIN_ATTACK] != 0) {
        MessageArgument_PushFar(gains[GAIN_ATTACK], MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar((s32)MsgAttackRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[GAIN_DEFENSE] != 0) {
        MessageArgument_PushFar(gains[GAIN_DEFENSE], MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar((s32)MsgDefenseRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[GAIN_AGILITY] != 0) {
        MessageArgument_PushFar(gains[GAIN_AGILITY], MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar((s32)MsgAgilityRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[GAIN_LUCK] != 0) {
        MessageArgument_PushFar(gains[GAIN_LUCK], MESSAGE_ARGUMENT_NUMBER);
        // "Luck rises by N!"
        BattleMessage_ShowAndWaitFar((s32)MsgLuckRises);
        BattleMessage_WaitForAdvance();
      }
    }
  }
  Heap_Release(backup);
  if (spoils->coins != 0) {
    MessageArgument_PushFar(spoils->coins, MESSAGE_ARGUMENT_NUMBER);
    // "You got N coins."
    BattleMessage_ShowAndWaitFar((s32)MsgGotCoins);
    Party_AdjustCoinsFar(spoils->coins);
    BattleMessage_WaitForAdvance();
  }
  found = &gMainRoutineWork.pendingItem;
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
    MessageArgument_PushFar(spoils->items[bestSlot], MESSAGE_ARGUMENT_ITEM);
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
